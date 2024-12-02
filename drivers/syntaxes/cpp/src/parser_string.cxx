/*parser_string.cxx*/
#include "pch.h"
#include "main.h"

static struct {
	const char * prefix;
	size_t		len;
} prefixes[] = {
	{ "",	0 },	// ce_none
	{ "L",	1 },	// ce_widechar
	{ "u8",	2 },	// ce_utf8
	{ "u",	1 },	// ce_utf16
	{ "U",	1 },	// ce_utf32
};

/*const status_t f1::c_pp::parser::format_string( pp::string & buf, const char *& s, const char * end_s, string_parse_flags_t flags, char_encoding_t encoding )
{
	if( ce_none != encoding )
	{
		__debugbreak_if( unsigned(encoding) >= _countof(prefixes) )
			return STATUS_ERROR_INVALID_PARAMETER;
		buf.add_part( prefixes[encoding] );
	}

	if( flags & fl_parse_string_raw	)
		buf.add_part( "R" );

	return STATUS_SUCCESS;
}*/

const status_t f1::c_pp::parser::skip_string( const char *& s, const char * end_s, int end_ch, pp::string * buf,
	string_parse_flags_t flags, char_encoding_t encoding )
{
	// Check current state
	if( 0 == end_ch )
		end_ch = *s++;

	if( end_s - s <= 0 )
	{
		if( !(flags & string_parse_flags_t::fl_parse_string_ignore_unterm) )	// check for single '"' in the end of file
		{
			m_seq_start = nullptr;
			return message( syntax::IMSG_ERROR_UNTERMINATED_STRING );
		}
		return STATUS_SUCCESS;
	}

	// Parse raw string if required
	if( flags & string_parse_flags_t::fl_parse_string_raw )
	{
		// Parse string prefix
		const char * const prefix_start = s;
		while( s < end_s && '(' != *s )
		{
			if( '\n' == *s || '\r' == *s )
				skip_newline( s, end_s, m_parser );
			else if( '\"' == *s )
			{
				message( syntax::IMSG_WARNING_INVALID_RAW_STRING );
				s = prefix_start;
				goto normal_string;	// prefix was not found; fall back to the normal string parsing
			}
			else
				++ s;
		}
		if( s >= end_s )
		{
			m_seq_start = nullptr;
			//if( nullptr != buf )
			//	buf->clear();
			return message( syntax::IMSG_ERROR_UNTERMINATED_STRING );
		}
		const size_t prefix_length = s - prefix_start;
		++ s;	// skip opening parenthesis

		// Parse string 
		bool string_ok = false;
		const char * start = s;
		for( size_t rest_length = end_s - s - 1; s < end_s; ++ s, -- rest_length )
		{
			if( rest_length < prefix_length )
				break;	// early exit if there's no chance to find prefix
			if( ')' == *s && (0 == prefix_length || !__small_memcmp( s + 1, prefix_start, prefix_length )) )
			{
				if( s[prefix_length + 1] == end_ch )
				{
					string_ok = true;
					break;
				}
				else
					message( syntax::IMSG_WARNING_RAW_STRING_TERM_IN_MIDDLE );
			}
			if( '\n' == *s || '\r' == *s )
			{
				skip_newline( s, end_s, m_parser );
				-- s;
				rest_length = end_s - s - 1;
			}
			if( end_ch == *s && nullptr != buf )
			{
				buf->add_part( start, s - start );
				buf->add_part( "\\", 1 );
				start = s;
			}
		}
		if( !string_ok )
		{
			m_seq_start = nullptr;
			//if( nullptr != buf )
			//	buf->clear();
			message( syntax::IMSG_ERROR_UNTERMINATED_STRING );
		}

		// Store string to the target buffer
		if( nullptr != buf )
			buf->add_part( start, s - start );
		s += prefix_length + 1;	// skip closing parenthesis and prefix

		// Skip closing quote
		if( end_ch != *s )
		{
			const char c[4] = { '<', char(end_ch), '>', '\0' };
			return message( syntax::IMSG_ERROR_EXPECTED_S, c );
		}
		++ s;

		// Done, exit
		return STATUS_SUCCESS;
	}
normal_string:

	// Parse string contents
	const char * start = s;
	bool string_ok = 0 == end_ch;
	for( ; s < end_s; ++ s )
	{
		if( end_ch == *s )
		{
			string_ok = true;
			break;
		}
		else if( '\\' == *s )
		{
			if( nullptr != buf )
				buf->add_part( start, s - start );
			++ s;

			const bool old_disable_line_update = m_disable_line_update;
			m_disable_line_update = true;

			if( !skip_newline( s, end_s, m_parser ) )
			{
				if( flags & string_parse_flags_t::fl_parse_string_no_escapes )
				{
					-- s;
					start = s;
					continue;
				}

				pp::string tmpbuf;
				if( parse_esc_char( tmpbuf, s, end_s, encoding ).failed() )
					message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
				else
				{
					if( nullptr != buf )
						buf->add_part( tmpbuf );
				}
			}
			else
				skip_real_spaces_and_comments( s, end_s );	// skip whitespace in the start of the next line
			m_disable_line_update = old_disable_line_update;
			start = s;
			-- s;
		}
		else if( '\n' == *s || '\r' == *s )
		{
			if( nullptr != buf )
				buf->add_part( start, s - start );
			//message( syntax::IMSG_ERROR_NEWLINE_IN_STRING );
			//skip_newline( s, end_s );
			//-- s;
			if( !(flags & string_parse_flags_t::fl_parse_string_allow_nl) )
				break;
			start = s;
		}
	}
	if( nullptr != buf )
		buf->add_part( start, s - start );
	if( end_ch == *s )
		++ s;

	// Done
	if( !string_ok && !(flags & string_parse_flags_t::fl_parse_string_ignore_unterm) )
	{
		m_seq_start = nullptr;
		//if( nullptr != buf && 0 != end_ch )
		//	buf->add_part( (const char *) &end_ch, 1 );
		//	buf->clear();
		return message( syntax::IMSG_ERROR_UNTERMINATED_STRING );
	}

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::skip_string( const char *& s, const char * end_s, pp::string * buf,
	string_parse_flags_t flags, char_encoding_t encoding )
{
	if( s >= end_s )
		return STATUS_SUCCESS;
	const int end_ch = get_end_quote( *s );
	if( 0 == end_ch )
		return STATUS_SUCCESS;
	++ s;
	return skip_string( s, end_s, end_ch, buf, flags, encoding );
}
const status_t f1::c_pp::parser::skip_string( string_parse_flags_t flags, char_encoding_t encoding )
{
	if( m_parser->is_eof() )
		return STATUS_SUCCESS;
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	const int end_ch = get_end_quote( *s );
	if( 0 == end_ch )
		return STATUS_SUCCESS;
	++ s;
	status_t status = skip_string( s, end_s, end_ch, nullptr, flags, encoding );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}
const status_t f1::c_pp::parser::skip_string( int end_ch, string_parse_flags_t flags, char_encoding_t encoding )
{
//	if( m_parser->is_eof() )
//		return STATUS_SUCCESS;
	const char * start = m_parser->get_ptr();
	const char * s = start;
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	if( 0 == end_ch )
	{
		end_ch = get_end_quote( *s );
		if( 0 == end_ch )
			return STATUS_SUCCESS;
		++ s;
	}
	status_t status = skip_string( s, end_s, end_ch, nullptr, flags, encoding );
	if( s != start && s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}

int f1::c_pp::parser::get_end_quote( int ch ) const
{
	if( '\'' == ch || '\"' == ch )
		return ch;
	return 0;
}

const f1::nls::Iencoding * f1::c_pp::parser::get_encoding( char_encoding_t encoding ) const
{
	return const_cast<const f1::nls::Iencoding *>( const_cast<self *>(this)->get_encoding( encoding ) );
}
f1::nls::Iencoding * f1::c_pp::parser::get_encoding( char_encoding_t encoding )
{
	if( ce_widechar == encoding )
		encoding = get_c_flags( c_flags_t::fl_long_wchar_t ) ? ce_utf32 : ce_utf16;

	switch( encoding )
	{
	case ce_none:
	case ce_utf8:
		return m_nls_utf8_encoding;
	case ce_utf16:
		return m_nls_utf16_encoding;
	case ce_utf32:
		return m_nls_utf32_encoding;
	default:
		return nullptr;
	}
}

const status_t f1::c_pp::parser::parse_string( pp::string & buf, const char *& s, const char * end_s,
	string_parse_flags_t flags, char_encoding_t encoding )
{
	int end_ch = get_end_quote( *s );
	if( 0 == end_ch )
		return STATUS_SUCCESS;
	++ s;
	return skip_string( s, end_s, end_ch, &buf, flags, encoding );
}
const status_t f1::c_pp::parser::parse_string( pp::string & buf, int end_ch, string_parse_flags_t flags, char_encoding_t encoding )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source

	if( 0 == end_ch )
	{
		end_ch = get_end_quote( *s );
		if( 0 == end_ch )
			return STATUS_SUCCESS;
		s ++;
	}

	status_t status = skip_string( s, end_s, end_ch, &buf, flags, encoding );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}
const status_t f1::c_pp::parser::parse_string( int end_ch, string_parse_flags_t flags, char_encoding_t encoding )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source

	// Check current state
	if( s >= end_s )
		return STATUS_SUCCESS;

	if( 0 == end_ch )
	{
		end_ch = get_end_quote( *s );
		if( 0 == end_ch )
			return STATUS_SUCCESS;
		s ++;
	}

	// Parse string
	pp::string buf;
	status_t status = skip_string( s, end_s, end_ch, &buf, flags, encoding );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );

	// Process results
	output( (const char *) &end_ch, 1 );	// TODO: write header
	output( buf );
	output( (const char *) &end_ch, 1 );

	// Exit
	return status;
}

/**
 * @brief Parse several consecutive strings merging them into one string.
 *
 * @param[in] flags (string_parse_flags_t) string parse flags applied to subsequent calls to skip_string.
 * @return (status_t) operation status.
 *
 *	This function may sometimes fail since construct "a" L"b" must be stored as L"ab". Since this function output
 * parsed strings immediately, prefix discrepancy in the following strings will lead to breaking the loop.
 *
 * @TODO: sometimes duplicates strings (see stringize.c with --merge-string enabled)
 */
const status_t f1::c_pp::parser::merge_strings( string_parse_flags_t flags )
{
	int end_ch = 0;
	char_encoding_t enc;
	if( !parse_string_header( enc, end_ch, flags ) )
		return STATUS_SUCCESS;
	return merge_strings( enc, end_ch, flags );
}
const status_t f1::c_pp::parser::merge_strings( char_encoding_t initial_encoding, int initial_end_ch, string_parse_flags_t flags )
{
	status_t status;
	int end_ch = initial_end_ch;
	pp::string common_string;
	char_encoding_t common_encoding = initial_encoding;
	bool mixed_encoding_reported = false;
	string_parse_flags_t parse_flags = flags;
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();
	bool raw_string = false;	// TODO: use it

	while( s < end_s )
	{
		// Parse string and store it to the buffer
		pp::string buf;
		if( skip_string( s, end_s, end_ch, &buf, parse_flags ).failed_status( status ) )
			return status;
		parse_flags = flags;
		common_string.add_part( buf );

		// Try to parse next string header
		const char * old_s = s;
		enable_output( false );
		skip_spaces_and_comments( s, end_s );
		enable_output( true );

		if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
			m_parser->set_ptr( s );

		char_encoding_t enc;
		parse_flags = flags;
		if( !parse_string_header( enc, end_ch, parse_flags ) )	// no next string
		{
			s = old_s;
			break;
		}
		s = m_parser->get_ptr();

		if( parse_flags & string_parse_flags_t::fl_parse_string_raw )
			raw_string = true;

		// Check for mixed string encodings
		if( enc != common_encoding )
		{
			// Report mixed encodings
			if( !mixed_encoding_reported )
			{
				message( syntax::IMSG_WARNING_MIXED_ENCODING );
				mixed_encoding_reported = true;
			}

			// Calculate common grounds
			if( ce_none == enc || ce_none == common_encoding )	// string without prefix; use parsed prefix
				common_encoding = enc;
			else	// two different prefixes; use the lergest one (TODO: may be just bail out on error?)
				common_encoding = core::Tmax( common_encoding, enc );
		}
	}

	if( !common_string.empty() )
	{
		//CT_ASSERT( ce_count == _countof(prefixes) );//, "discrepancy between prefix array and char_encoding_t" );
		pp::string out_string;

//		if( raw_string )
//			output( "R", 1 );
		output( prefixes[common_encoding].prefix, prefixes[common_encoding].len );
		stringify_chars( out_string, common_string, end_ch ); output( out_string );
	}

	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parse string literal prefix.
 *
 * @param[out] enc (char_encoding_t &) string encoding.
 * @param[out] end_ch (int &) string ending character (type of quote).
 * @param[out] parse_flags (string_parse_flags_t &) additional flags related to string contents parsing.
 *
 * @return (bool) true if string literal prefix (including double quote) is parsed successfully.
 *
 *	'R' raw string: introduced in C++11; __cpp_raw_strings macro must be defined
 *	'u'/'U' strings: introduced in C++11: __cpp_unicode_literals and __cpp_unicode_characters must be defined
 *	'u8' strings: introduced in C++20
 */
bool f1::c_pp::parser::parse_string_header( const char * & s, const char * end_s,
	char_encoding_t & enc, int & end_ch, string_parse_flags_t & parse_flags )
{
	int ch;
	char_encoding_t presumed_encoding = ce_none;
	size_t offset = 0;
	bool raw_literal = false;
	bool excessive_raw_literal = false;

	ch = *s;

	// Parse raw string prefix
	// Currently disabled as e.g. Ru8 (raw UTF-8) string literal prefixes are not supported
	/*if( 'R' == ch )
	{
		raw_literal = true;
		++ offset;
		ch = m_parser->peek_char_at( offset );
	}*/

	// Parse prefix
	if( 'L' == ch )
		presumed_encoding = ce_widechar,
		offset = 1;
	else if( 'U' == ch )
		presumed_encoding = ce_utf32,
		offset = 1;
	else if( 'u' == ch )
	{
		if( '8' == m_parser->peek_char_at( 1 ) )
			presumed_encoding = ce_utf8,
			offset = 2;
		else
			presumed_encoding = ce_utf16,
			offset = 1;
	}

	// Parse raw string prefix
	ch = s[offset];
	if( 'R' == ch )
	{
		if( raw_literal )
			excessive_raw_literal = true;
		raw_literal = true;
		++ offset;
	}

	// Parse quote
	ch = s[offset];
	if( '\"' != ch && '\'' != ch )
		return false;

    end_ch = ch;
	enc = presumed_encoding;
//	flush_chars();
	s += offset + 1;
//	m_seq_start = m_parser->data() - 1;

	if( raw_literal )
		parse_flags |= string_parse_flags_t::fl_parse_string_raw;
	else
		parse_flags &= ~string_parse_flags_t::fl_parse_string_raw;
	if( excessive_raw_literal )	// TODO: remove
	{
		message( '\'' == end_ch ?
			syntax::IMSG_WARNING_EXCESSIVE_CHARACTER_PREFIX :
			syntax::IMSG_WARNING_EXCESSIVE_STRING_PREFIX
			);
	}

	// Exit
	return true;
}
bool f1::c_pp::parser::parse_string_header( char_encoding_t & enc, int & end_ch, string_parse_flags_t & parse_flags )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	bool status = parse_string_header( s, end_s, enc, end_ch, parse_flags );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Convert raw string of chars to C-correct representation.
 */
const status_t f1::c_pp::parser::stringify_chars( pp::string & buf,
	const pp::string & src, int end_ch, unsigned flags ) const
{
	const char * s = src.data();
	const char * end_s = s + src.size();
	char sbuf[4];
	const char * ptr;
	size_t len;
	core::Tarray<int>	quote_stack;

	if( 0 != end_ch )
		buf.add_part( (const char *) &end_ch, 1 );

	const char * start = s;
	for( ; s < end_s; ++ s )
	{
		switch( *s )
		{
		case '\a':	ptr = "\\a", len = 2;	break;
		case '\b':	ptr = "\\b", len = 2;	break;
		case '\f':	ptr = "\\f", len = 2;	break;
		case '\n':	ptr = "\\n", len = 2;	break;
		case '\r':	ptr = "\\r", len = 2;	break;
		case '\t':	ptr = "\\t", len = 2;	break;
		case '\v':	ptr = "\\v", len = 2;	break;
		case '?':	ptr = "\\\?", len = 2;	break;
		case '\\':
			if( quote_stack.empty() && 0 == (flags & fl_stringify_slashes) )
				ptr = "\\",
				len = 1;
			else
				ptr = "\\\\",
				len = 2;
			break;
		case '\'':
			if( '\'' == end_ch )
				ptr = "\\\'",
				len = 2;
			else
				ptr = "\'",
				len = 1;

			if( quote_stack.empty() )
				quote_stack.push_back( *s );
			else if( *s == *quote_stack.rbegin() )
				quote_stack.pop_back();

			break;
		case '\"':
			if( '\"' == end_ch )
				ptr = "\\\"",
				len = 2;
			else
				ptr = "\"",
				len = 1;

			if( quote_stack.empty() )
				quote_stack.push_back( *s );
			else if( *s == *quote_stack.rbegin() )
				quote_stack.pop_back();

			break;
		default:
			if( 0x1fu >= unsigned(*s) || 0x7f == *s )
			{
				sbuf[0] = '\\';
				sbuf[1] = 'x';
				sbuf[2] = toxdigit( *s >> 4 );
				sbuf[3] = toxdigit( *s & 0xf );
				ptr = sbuf;
				len = 4;
				break;
			}
			continue;	// continue gathering bulk of chars
		}

		buf.add_part( start, s - start );	// store gathered bulk of chars
		start = s + 1;						// move to the next character
		buf.add_part( ptr, len );			// add escape sequence
	}
	buf.add_part( start, s - start );	// store gathered bulk of chars

	if( 0 != end_ch )
		buf.add_part( (const char *) &end_ch, 1 );

	return STATUS_SUCCESS;
}
/**
 * @brief Convert C-correct representation of string of chars to raw string.
 */
const status_t f1::c_pp::parser::unstringify_chars( pp::string & buf, const pp::string & src, bool allow_non_strings ) const
{
	return unstringify_chars( buf, src.data(), src.length(), allow_non_strings );
}
const status_t f1::c_pp::parser::unstringify_chars( pp::string & buf, const char * s, size_t length, bool allow_non_strings ) const
{
	status_t status;
	const char * end_s = s + length;
	core::Tarray<int>	quote_stack;

	const char * start = s;
	while( s < end_s )
	{
		if( '\"' == *s || '\'' == *s )
		{
			if( quote_stack.empty() )
			{
				buf.add_part( start, s - start );	// store gathered bulk of chars
				quote_stack.push_back( *s );
				start = s + 1;
			}
			else if( *s == *quote_stack.rbegin() )
			{
				buf.add_part( start, s - start );	// store gathered bulk of chars
				quote_stack.pop_back();
				start = s + 1;
			}
		}
		else if( '\\' == *s )
		{
			if( !quote_stack.empty() )
			{
				buf.add_part( start, s - start );		// store gathered bulk of chars
				++ s;
				if( parse_esc_char( buf, s, end_s ).failed_status( status ) )
					return status;
				start = s;
			}
			else
				++ s;
			continue;
		}
		/*else if( ( '\x20' == *s || '\t' == *s ) && quote_stack.empty() )	// skip spaces that out of strings
		{
			buf.add_part( start, s - start );	// store gathered bulk of chars
			for( ; s < end_s && ('\x20' == *s || '\t' == *s); ++ s )
				;
			start = s;
			continue;
		}*/
		else if( quote_stack.empty() )
		{
			if( isspace( *s ) && allow_non_strings )
			{
				for( ; s < end_s && isspace( *s ); ++ s )
					;
				continue;
			}

			const char * pos = s;
			const_cast<self *>(this)->skip_spaces_and_comments( pos, end_s );	// skip spaces that out of strings
			if( pos != s || allow_non_strings )
			{
				buf.add_part( start, s - start );	// store gathered bulk of chars
				start = s = pos;
				if( !allow_non_strings )
					continue;
			}
			else	// some non-space and non-comment characters
			{
				return STATUS_ERROR_INVALID_FORMAT;
			}
		}
		++ s;
	}
	buf.add_part( start, s - start );			// store gathered bulk of chars

	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::_reverse_encoded_string( pp::string & str, char_encoding_t encoding )
{
	char * data = const_cast<char *>(str.get_stringslice().data());
	size_t size = str.get_stringslice().length();

	if( ce_widechar == encoding )
		encoding = get_c_flags( c_flags_t::fl_long_wchar_t ) ? ce_utf32 : ce_utf16;

	switch( encoding )
	{
	case ce_none:
	case ce_utf8:
		// do nothing
		break;
	case ce_utf16:
		_membswapw( data, data, size / 2 );
		break;
	case ce_utf32:
		_membswapl( data, data, size / 4 );
		break;
	default:
		DBG_FAIL();
		return STATUS_ERROR_INVALID_PARAMETER;
	}

	return STATUS_SUCCESS;
}

/*END OF parser_string.cxx*/
