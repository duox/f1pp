/*parser_comment.cxx*/
#include "pch.h"
#include "main.h"

bool f1::c_pp::parser::skip_spaces_and_comments( const char * & start, const char * end, f1::parser::Iparser * parser )
{
	const char * s = start;
	const char * const end_s = end;
	bool res = false;
	bool was_space = false;

	while( s < end_s )
	{
		if( '\n' == *s || '\r' == *s )
		{
			skip_newline( s, end_s, parser );
			m_new_line = true;
			res = true;
			continue;
		}
		if( isspace( *s ) )
		{
			++ s;
			if( !was_space )
			{
				//output( "\x20", 1 );
				was_space = true;
			}
			res = true;
			continue;
		}
		if( '/' == *s )
		{
			if( !parse_comment( s, end, parser ) )
				break;
			was_space = false;
			res = true;
			continue;
		}
		if( '\\' == *s )
		{
			was_space = false;
			++ s;
			res |= skip_newline( s, end_s, parser );
			continue;
		}
		break;
	}

	start = s;
	return res;
}
bool f1::c_pp::parser::skip_real_spaces_and_comments( const char * & start, const char * end, f1::parser::Iparser * parser )
{
	const char * s = start;
	const char * const end_s = end;
	bool res = false;
	bool was_space = false;

	while( s < end_s )
	{
		if( isspace( *s ) && '\n' != *s && '\r' != *s )
		{
			++ s;
			if( !was_space )
			{
				//output( "\x20", 1 );
				was_space = true;
			}
			res = true;
			continue;
		}
		if( '/' == *s )
		{
			if( !parse_comment( s, end, parser ) )
				break;
			was_space = false;
			res = true;
			continue;
		}
		if( '\\' == *s )
		{
			was_space = false;
			++ s;
			res |= skip_newline( s, end_s, parser );
			continue;
		}
		break;
	}

	start = s;
	return res;
}
bool f1::c_pp::parser::parse_comment( const char *& s, const char * end_s, f1::parser::Iparser * parser )
{
	bool comment_ok = false;
	const char * start = s + 1;

	// Check for single '/' in the end of file
	if( end_s - s <= 1 )
		return false;

	// Check for backslash and newline.
	// According to ISO/IEC 9899:2023 6.4.9.3 Example, the '//' and '/*' comment start sequences
	// can be split with \ + newline sequence(s)
	while( start < end_s && '\\' == *start )
	{
		++ start;
		skip_newline( start, end_s, parser );
	}
	if( start >= end_s )
		return false;		// single '/' followed by the seqence of \ + nl till the end of file

	// Parse comments
	if( '*' == *start )		// multiline comment?
	{
		// Check for doxygen comments
		if( ('*' == start[1] && '/' != start[2]) || '!' == start[1] )
		{
			if( m_c_flags & c_flags_t::fl_keep_doxygen_comments )
				start_char_seq();
		}
		s = start + 1;		// skip '/*'

		// Parse the rest of the comment
		if( get_c_flags( c_flags_t::fl_nested_comments ) )
		{
			unsigned nesting_count = 0;
			while( end_s - s >= 2 )
			{
				if( '/' == s[0] && '*' == s[1] )
				{
					message( syntax::IMSG_WARNING_NESTED_COMMENT );
					s += 2;
					++ nesting_count;
					continue;
				}
				if( '*' == s[0] && '/' == s[1] )
				{
					s += 2;
					if( 0 == nesting_count )
					{
						comment_ok = true;
						break;
					}
					-- nesting_count;
					continue;
				}
				if( '\n' == *s || '\r' == *s )
				{
					skip_newline( s, end_s, parser );
					continue;
				}
				++ s;
			}
		}
		else
		{
			while( end_s - s >= 2 )
			{
				if( '/' == s[0] && '*' == s[1] )
				{
					message( syntax::IMSG_WARNING_NESTED_COMMENT );
					s+= 2;
					continue;
				}
				if( '*' == s[0] && '/' == s[1] )
				{
					comment_ok = true;
					s += 2;
					break;
				}
				if( '\n' == *s || '\r' == *s )
				{
					skip_newline( s, end_s, parser );
					continue;
				}
				++ s;
			}
		}
		if( !comment_ok )
			message( syntax::IMSG_ERROR_UNTERMINATED_COMMENT );
	}
	else if( '/' == *start )	// single line comment?
	{
		// Check for doxygen comments
		if( '/' == start[1] || '!' == start[1] )
		{
			if( m_c_flags & c_flags_t::fl_keep_doxygen_comments )
				start_char_seq();
		}

		s = start + 1;		// skip '//'

		// Parse the rest of the comment
		for( ; s < end_s; ++ s )
		{
			if( '\\' == *s )
			{
				++ s;
				skip_newline( s, end_s, parser );
			}
			if( '\n' == *s || '\r' == *s )
				break;
		}
		comment_ok = true;
	}
	else
		return false;

	// Output single space if comment parsed successfully
//	if( comment_ok )
//		output( "\x20", 1 );

	// Exit
	return comment_ok;
}

const status_t f1::c_pp::parser::skip_spaces_and_comments( bool * skipped_something )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	bool skipped = skip_spaces_and_comments( s, end_s, m_parser );
	if( skipped && s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	if( nullptr != skipped_something )
		*skipped_something = skipped;
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::skip_real_spaces_and_comments( bool * skipped_something )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	bool skipped = skip_real_spaces_and_comments( s, end_s, m_parser );
	if( skipped && s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	if( nullptr != skipped_something )
		*skipped_something = skipped;
	return STATUS_SUCCESS;
}
bool f1::c_pp::parser::parse_comment()
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	bool skipped = parse_comment( s, end_s, m_parser );
	if( skipped && s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return skipped;
}
/*
const status_t f1::c_pp::parser::parse_comment()
{
	const char * s = m_parser->get_ptr();
	const char * const end = m_parser->get_end_ptr();

	if( L'/' == *s )
	{
		for( ; s < end; ++ s )
		{
			if( L'\n' == *s || L'\r' == *s )
			{
				parse_newline();
				break;
			}
		}
	}
	else if( L'*' == *s )
	{
		for( ; s < end - 1; ++ s )
		{
			if( '*' == s[0] && '/' == s[1] )
				break;
			if( '\n' == *s || '\r' == *s )
				parse_newline();
		}
	}

	return STATUS_SUCCESS;
}
*/
const status_t f1::c_pp::parser::trim_spaces_and_comments( pp::string & buf, bool trim_newlines )
{
	const char * s = buf.data();
	const char * end_s = buf.data_end();

	// Skip initial spaces and comments
	trim_newlines ?
		skip_spaces_and_comments( s, end_s  ):
		skip_real_spaces_and_comments( s, end_s  );
	if( s == end_s )
	{
		buf.clear();
		return STATUS_SUCCESS;
	}
	const char * first_non_space = s;

	// Skip to end of line, remembering when last non-space and non-comment character was met
	// TODO: wouldn't it be faster to start scanning from the end?
	const char * last_non_space = s;
	for( ; s < end_s; ++ s )
	{
		if( '/' == *s )
		{
			if( !parse_comment( s, end_s ) )
				last_non_space = s;
			else
				-- s;
		}
		else if( isspace( *s ) )
		{
			if( trim_newlines || ( '\n' != *s && '\r' != *s ) )
				continue;
			last_non_space = s;
		}
		else
			last_non_space = s;
	}

	// Process results
	if( first_non_space != buf.data() || last_non_space + 1 != buf.data_end() )
	{
		pp::string res;
		// TODO
#if 0
		// Try to fetch data w/o allocation
		res.assign( buf, first_non_space - buf.data(), last_non_space - first_non_space );
#else
		res.add_part_alloc( first_non_space, last_non_space - first_non_space + 1 );
#endif
		buf.move( res );
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF parser_comment.cxx*/
