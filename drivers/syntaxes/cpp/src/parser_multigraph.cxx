/*parser_multigraph.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::c_pp::parser::process_multigraphs( core::buffer & buf, const core::stringslice & src_buf )
{
	const char * start = src_buf.data();
	const char * s = start;
	const char * end_s = src_buf.data_end();
	uint32_t ch = 0;
	size_t count = 1;
	const char * region_start = s;
	const char * region_end = nullptr;
	bool warn_digraph	= !!(m_c_message_flags & c_message_flags_t::fl_warn_digraphs);
	bool warn_trigraph	= !!(m_c_message_flags & c_message_flags_t::fl_warn_trigraphs);
	bool warn_ucn		= !!(m_c_message_flags & c_message_flags_t::fl_warn_ucn_chars);

	if( s >= end_s )
		return STATUS_SUCCESS;
	__debugbreak_if( nullcstr == start )
		return STATUS_ERROR_INTERNAL;
	__debugbreak_if( nullcstr == end_s )
		return STATUS_ERROR_INTERNAL;

	#define PARSE_DIGRAPH( ch1, ch2, res )	\
		else if( ch1 == s[0] && ch2 == s[1] )	\
		{\
			if( warn_digraph )\
				message( syntax::IMSG_WARNING_DIGRAPHS_FOUND ),\
				warn_digraph = false;\
			if( !(m_c_flags & c_flags_t::fl_process_digraphs) )\
				goto ignore;\
			region_end = s;\
			s += 2;\
			ch = res;\
		}
	#define PARSE_TRIGRAPH( ch3, res )	\
		else if( ch3 == s[2] )	\
			ch = res
	buf.set_reserved_size( src_buf.length() );
	while( s < end_s - 1 )
	{
		if( '?' == s[0] && '?' == s[1] )
		{
			if( warn_trigraph )
				message( syntax::IMSG_WARNING_TRIGRAPHS_FOUND ),
				warn_trigraph = false;
			if( !(m_c_flags & c_flags_t::fl_process_trigraphs) )
				goto ignore;

			region_end = s;
			if( m_c_flags & c_flags_t::fl_standard_trigraphs )
			{
				#pragma warning(push)
				#pragma warning(disable:4127)	//: conditional expression is constant
				if(0);
				#pragma warning(pop)
				PARSE_TRIGRAPH( '=', '#' );
				PARSE_TRIGRAPH( '/', '\\' );
				PARSE_TRIGRAPH( '\'', '^' );
				PARSE_TRIGRAPH( '(', '[' );
				PARSE_TRIGRAPH( ')', ']' );
				PARSE_TRIGRAPH( '!', '|' );
				PARSE_TRIGRAPH( '<', '{' );
				PARSE_TRIGRAPH( '>', '}' );
				PARSE_TRIGRAPH( '-', '~' );
			}
			else
				get_trigraph( s[2], * (char *) &ch );

			s += 3;
			if( 0 == ch )
			{
				message( syntax::IMSG_WARNING_INVALID_TRIGRAPH_C, s[2] );
				continue;
			}
		}
		PARSE_DIGRAPH( '<', ':', '[' )
		PARSE_DIGRAPH( ':', '>', ']' )
		PARSE_DIGRAPH( '<', '%', '{' )
		PARSE_DIGRAPH( '%', '>', '}' )
		PARSE_DIGRAPH( '%', ':', '#' )
		// Proces UCNs
		else if( '\'' == s[0] && ('u' == s[1] || 'U' == s[1]) )
		{
			if( warn_ucn )
				message( syntax::IMSG_WARNING_UCN_FOUND ),
				warn_ucn = false;
			if( !(m_c_flags & c_flags_t::fl_process_ucn_chars) )
				goto ignore;

			const unsigned char_size = 'u' == s[1] ? 2 : 4;
			pp::variant res;
			if( !parse_hex_integer( res, char_size*2, s, end_s ) )
			{
				message( syntax::IMSG_WARNING_INVALID_TRIGRAPH_C, s[2] );
				continue;
			}
			ch = (uint32_t) res.as_int();
			if( !unicode::utf32::is_valid_char( ch ) )
			{
				message( syntax::IMSG_WARNING_INVALID_TRIGRAPH_C, s[2] );
				continue;
			}
			region_end = s;
			count = char_size;
		}
		else
		{
	ignore:
			++ s;
			continue;
		}

		__assert( 0 != ch );
		buf.append( region_start, region_end - region_start );
		buf.append( (const char *) &ch, count );
		region_start = s;
		ch = 0;
		count = 1;
	}
	buf.append( region_start, s - region_start );
	buf.append( &end_s[-1], 1 );	// append last character which was omitted in the above loop

	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Digraphs and trigraphs management

const status_t f1::c_pp::parser::append_digraph( char first_char, char second_char, char replacement_char )
{
	__debugbreak_if( m_digraph_map_second.find( replacement_char ) != core::string::npos )
		return STATUS_ERROR_ALREADY_EXISTS;

	m_digraph_map_first.push_back( first_char );
	m_digraph_map_first.push_back( second_char );
	m_digraph_map_second.push_back( replacement_char );
	m_c_flags &= c_flags_t::bits(~c_flags_t::fl_standard_digraphs);

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::append_digraphs( const char * digraphs, const char * replacement_chars, size_t count )
{
	if( size_t(-1) == count )
		count = __small_strlen( replacement_chars );
	for( size_t i = 0; i < count; ++ i )
		append_digraph( digraphs[i + 0], digraphs[i + 1], replacement_chars[i] );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::remove_digraph( char first_char, char second_char )
{
	const char * data = m_digraph_map_first.data();
	const char * data_end = data + m_digraph_map_first.size();
	const char * repl = m_digraph_map_second.data();

	core::string resulting_first;
	core::string resulting_second;
	for( ; data < data_end; data += 2, ++ repl )
	{
		if( data[0] != first_char || data[1] != second_char )
		{
			resulting_first.push_back( data[0] );
			resulting_first.push_back( data[1] );
			resulting_second.push_back( *repl );
		}
	}
	m_digraph_map_first.move( resulting_second );
	m_digraph_map_second.move( resulting_second );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::remove_digraphs( const char * digraphs, size_t count )
{
	if( size_t(-1) == count )
		count = __small_strlen( digraphs ) / 2;

	const char * data = m_digraph_map_first.data();
	const char * data_end = data + m_digraph_map_first.size();
	const char * repl = m_digraph_map_second.data();
	const char * digraphs_end = digraphs + count * 2;

	core::string resulting_first;
	core::string resulting_second;
	for( ; data < data_end; data += 2, ++ repl )
	{
		for( const char * dig = digraphs; dig < digraphs_end; dig += 2 )
		{
			if( * (uint16_t *) data != * (uint16_t *) dig )
			{
				resulting_first.push_back( data[0] );
				resulting_first.push_back( data[1] );
				resulting_second.push_back( *repl );
			}
		}
	}
	m_digraph_map_first.move( resulting_first );
	m_digraph_map_second.move( resulting_second );
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::append_trigraph( char third_char, char replacement_char )
{
	__debugbreak_if( m_trigraph_map_first.find( third_char ) != core::string::npos )
		return STATUS_ERROR_ALREADY_EXISTS;
	__debugbreak_if( m_trigraph_map_second.find( replacement_char ) != core::string::npos )
		return STATUS_ERROR_ALREADY_EXISTS;

	m_trigraph_map_first.push_back( third_char );
	m_trigraph_map_second.push_back( replacement_char );
	m_c_flags &= c_flags_t::bits(~c_flags_t::fl_standard_trigraphs);

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::append_trigraphs( const char * third_chars, const char * replacement_chars, size_t count )
{
	for( size_t i = 0; i < count; ++ i )
		append_trigraph( third_chars[i], replacement_chars[i] );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::remove_trigraph( char third_char )
{
	const char * data = m_trigraph_map_first.data();
	const char * data_end = data + m_trigraph_map_first.size();
	const char * repl = m_trigraph_map_second.data();

	core::string resulting_first;
	core::string resulting_second;
	for( ; data < data_end; ++ data, ++ repl )
	{
		if( *data != third_char )
		{
			resulting_first.push_back( *data );
			resulting_second.push_back( *repl );
		}
	}
	m_trigraph_map_first.move( resulting_second );
	m_trigraph_map_second.move( resulting_second );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::remove_trigraphs( const char * third_chars, size_t count )
{
	if( size_t(-1) == count )
		count = __small_strlen( third_chars );

	const char * data = m_trigraph_map_first.data();
	const char * data_end = data + m_trigraph_map_first.size();
	const char * repl = m_trigraph_map_second.data();
	const char * trigraphs_end = third_chars + count;

	core::string resulting_first;
	core::string resulting_second;
	for( ; data < data_end; ++ data, ++ repl )
	{
		for( const char * tri = third_chars; tri < trigraphs_end; ++ tri )
		{
			if( *data != *tri )
			{
				resulting_first.push_back( *data );
				resulting_second.push_back( *repl );
			}
		}
	}
	m_trigraph_map_first.move( resulting_first );
	m_trigraph_map_second.move( resulting_second );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::get_trigraph( char third_char, char & result )
{
	const char * data = m_trigraph_map_first.data();
	const char * data_end = data + m_trigraph_map_first.size();
	const char * repl = m_trigraph_map_second.data();

	for( ; data < data_end; ++ data, ++ repl )
	{
		if( *data == third_char )
		{
			result = *repl;
			return STATUS_SUCCESS;
		}
	}
	return STATUS_ERROR_NOT_FOUND;
}

/*END OF parser_multigraph.cxx*/
