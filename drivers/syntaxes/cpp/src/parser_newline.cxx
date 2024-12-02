/*parser_newline.cxx*/
#include "pch.h"
#include "main.h"

bool f1::c_pp::parser::set_newline( bool newline )
{
	m_was_new_line = m_new_line;
	m_new_line = newline;
	return m_was_new_line;
}

const status_t f1::c_pp::parser::fetch_newline()
{
	flush_chars();			// flush sequence
	if( m_capturing_line )
		finish_capture_line( m_parser->get_line() );
	status_t status = parse_newline();
	if( !m_new_line )//&& !m_disable_next_line_update )
	{
//if( 0 == m_args_parse_mode )//&& !m_capturing_line )
//				if( !m_capturing_line )
//					output( "\r\n", 2 );
	//			output( "\x20", 1 );
		set_newline( true );
	}
	m_seq_start = m_parser->get_ptr();

	return status;
}

const status_t f1::c_pp::parser::skip_to_newline()
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	status_t res = skip_to_newline( s, end_s );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return res;
}
const status_t f1::c_pp::parser::skip_to_newline( const char *& s, const char * end_s )
{
	// Skip as much new lines as possible
	while( s < end_s )
	{
		if( '\\' == *s )
		{
			++ s;
			skip_real_spaces_and_comments( s, end_s );
		}
		if( '\n' == *s || '\r' == *s )
		{
			break;
		}
		++ s;
	}

	// Exit
	return STATUS_SUCCESS;
}

/*const status_t f1::c_pp::parser::skip_newline( const char * s, const char * end_s )
{
	return STATUS_SUCCESS;
}*/

const status_t f1::c_pp::parser::parse_newline()
{
	//unsigned new_line_count = 0;

	// Skip as much new lines as possible
	while( !is_eof() )
	{
		const char * s = m_parser->get_ptr();
		if( '\n' != *s && '\r' != *s )
			break;
		m_parser->skip_nl( *s );
		//++ new_line_count;
	}

//	if( '(' != m_args_parse_mode )
//		on_line_changed( m_parser->get_line() );

	// Exit
	return STATUS_SUCCESS;
}

bool f1::c_pp::parser::skip_newline()
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	bool res = skip_newline( s, end_s, m_parser );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return res;
}
bool f1::c_pp::parser::skip_newline( const char *& s, const char * end_s, f1::parser::Iparser * parser )
{
	if( s >= end_s )
		return false;

	if( '\n' == *s )
	{
		++ s;
		if( s < end_s && '\r' == *s )
			++ s;
	}
	else if( '\r' == *s )
	{
		++ s;
		if( s < end_s && '\n' == *s )
			++ s;
	}
	else
		return false;

	if( nullptr != parser )
	{
		parser->on_nl();
		//on_line_changed( parser->get_line() );
	}

	return true;
}

const status_t f1::c_pp::parser::parse_to_newline( pp::string & buf, bool count_parenthesis )
{
#if 1
	const char * s = m_parser->get_ptr();
	const char * end_s = m_parser->get_end_ptr();
	const char * start = s;
	core::Tarray<int>	quote_stack;
	//char_encoding_t enc;
	//int end_ch;
	//string_parse_flags_t parse_flags;

	while( s < end_s )
	{
		if( '\\' == *s && ( '\n' == s[1] || '\r' == s[1] ) )
		{
			buf.add_part( start, s - start );
			++ s;

			__assert( false == m_disable_line_update );
			m_disable_line_update = true;
			skip_newline( s, end_s, m_parser );
			m_disable_line_update = false;

			start = s;
		}
		else if( isspace( *s ) )
		{
			const int ch = *s;

//			buf.add_part( start, s - start );

			if( '\n' == ch || '\r' == ch )
			{
				if( quote_stack.empty() )
					break;
			}
			++ s;

//			start = s;
		}
		else if( count_parenthesis && '(' == *s )
		{
			quote_stack.push_back( ')' );
			++ s;
		}
		else if( count_parenthesis && ')' == *s && !quote_stack.empty() && *s == *quote_stack.rbegin() )
		{
			quote_stack.pop_back();
			++ s;
		}
		else if( '/' == *s )
		{
			const char * end = s;
			if( quote_stack.empty() && parse_comment( s, end_s, m_parser ) )
			{
				buf.add_part( start, end - start );
				start = s;
			}
			else
				++ s;
		}
//		else if( parse_string_header( s, end_s, enc, end_ch, parse_flags ) )	- no need since new-lines are handled in the same way
//			skip_string( s, end_s, end_ch, nullptr, parse_flags, enc );
		else
			++ s;
	}
	buf.add_part( start, s - start );
	m_parser->set_ptr( s );
#elif 1
	while( !is_eof() )
	{
		if( m_parser->is_nl() )
			break;
		int ch = m_parser->fetch_char();
		if( '\\' == ch )
			m_parser->skip_nl();
	}
#else
	const char * start = m_parser->get_ptr();
	m_parser->skip_to_nl();
	buf.add_part( start, m_parser->get_ptr() - start );
#endif

	return STATUS_SUCCESS;
}

/*END OF parser_newline.cxx*/
