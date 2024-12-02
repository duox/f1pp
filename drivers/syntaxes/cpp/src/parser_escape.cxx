/*parser_escape.cxx*/
/** @file
 *
 * @brief Escape sequences
 */
#include "pch.h"
#include "main.h"

const status_t f1::c_pp::parser::_esc_cr( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding )
{
	++ s;		// skip '\r'
	if( s < end_s && '\n' == *s )
		++ s;	// skip '\n'

	__unreferenced_parameter( par );
	__unreferenced_parameter( buf );
	__unreferenced_parameter( encoding );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::_esc_nl( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding )
{
	++ s;		// skip '\n'
	if( s < end_s && '\r' == *s )
		++ s;	// skip '\r'

	__unreferenced_parameter( par );
	__unreferenced_parameter( buf );
	__unreferenced_parameter( encoding );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::_write_numeric_with_encoding( pp::string & buf, char_encoding_t encoding, uint32_t uch )
{
	if( ce_widechar == encoding )
		encoding = get_c_flags( c_flags_t::fl_long_wchar_t ) ? ce_utf32 : ce_utf16;

	size_t size;
	switch( encoding )
	{
	case ce_none:
	case ce_utf8:	size = 1;	break;
	case ce_utf16:	size = 2;	break;
	case ce_utf32:	size = 4;	break;
	default:
		DBG_FAIL();
		return STATUS_ERROR_INVALID_PARAMETER;
	}

	if( uch > (uint32_t(-1) >> (32 - size*8)) )
		message( syntax::IMSG_WARNING_CHARACTER_CONSTANT_OVERFLOW );
	buf.add_part( (const char *) &uch, size );

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::_esc_ctrl( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding )
{
	uint32_t uch = *s ++;
	if( 'A' <= uch && uch <= 'Z' )
		uch = uch - 'A' + 1;
	else if( 'a' <= uch && uch <= 'z' )
		uch = uch - 'a' + 1;
	else
		return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );

	buf.add_part( (const char *) &uch, 1 );
	//par._write_numeric_with_encoding( buf, encoding, uch );

	__unreferenced_parameter( end_s );
	__unreferenced_parameter( encoding );
	return STATUS_SUCCESS;
}
// https://html.spec.whatwg.org/multipage/syntax.html#character-references
// https://html.spec.whatwg.org/multipage/named-characters.html#named-character-references
const status_t f1::c_pp::parser::_esc_html( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding )
{
	uint32_t uch = 0;
	bool valid_char = false;

	++ s;	// skip '&'

	// Check for HTML decimal or hexadecimal numeric character
	if( '#' == *s )
	{
		int radix;

		// Parse numeric prefix and determine 
		++ s;
		if( 'x' == *s || 'X' == *s )
			++ s,
			radix = 16;
		else
			radix = 10;

		// Fetch character code
		const char * ep = s;
		uch = _strntoul( s, end_s - s, &ep, radix );
		if( s == end_s )
			return par.message( syntax::IMSG_ERROR_EXPECTED_INTEGER );

		// Check code validity
		// TODO: https://infra.spec.whatwg.org/#noncharacter
		if( iscntrl( uch ) && (0x0D == uch || !isspace( uch )) )
			par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
		else
			valid_char = true;
	}
	// Parse named character reference
	else
	{
		// Parse string
		const char * const name_start = s;
		for( ; s < end_s; ++ s )
		{
			if( !isalpha(*s) )	// since the named character list is static and no charater names contain characters other than letter
				break;
		}

		if( name_start == s )
			par.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		else
		{
			// TODO: import pp\drivers\syntaxes\c-pp\archives\html-named-characters.json
			//status_t status = find_named_character( uch, name_start, s - name_start );
			//if( status.failed() )
				par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
			//else
			//	valid_char = true;
		}
	}

	// Check ending
	if( s + 1 < end_s )
	{
		if( ';' != *s )
			par.message( syntax::IMSG_ERROR_EXPECTED_S, "';'" );
		else
			++ s;
	}

	// Store value
	if( valid_char )
		buf.add_part( (const char *) &uch, 1 );
		//par._write_numeric_with_encoding( buf, encoding, uch );

	// Exit
	__unreferenced_parameter( encoding );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::_esc_ucn( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding )
{
	// Check current state
	if( end_s - s < 3 )
		return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );

	// Parse opening brace
	if( '{' != *s )
		return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
	++ s;

	// Parse character name
	const char * const start = s;
	while( s < end_s && (isalnum( *s ) && nullptr != strchr("\x20-_", *s )) )
		++ s;
	const char * const end = s;

	// Parse closing brace
	if( s >= end_s || '}' != *s )
		return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
	++ s;

	// Find the character by name
	if( par.find_ucn( buf, start, end - start ).failed() )
		return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );

	// Exit
	__unreferenced_parameter( encoding );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::_esc_u16( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding )
{
	if( ce_none != encoding && ce_widechar != encoding && ce_utf16 != encoding )
		return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );

	const char * ep = nullptr;
	unsigned long uch;
	if( '{' == *s )	// \u{n...n}
	{
		uch = _strntoul( s, end_s - s, &ep, 16 );
		if( nullptr == ep || ep - s < 4 || '}' != *ep )	// at least three characters should be present: '{n}'
			return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
		++ ep;	// skip '}'
	}
	else					// \unnnn
	{
		uch = _strntoul( s, 4, &ep, 16 );
		if( nullptr == ep || ep - s < 4 )	// at least four characters should be present
			return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
	}

	// Check result
	unicode::uchar16le_t uch16 = unicode::uchar16le_t(uch);
	if( uch16 != uch )
		par.message( syntax::IMSG_WARNING_CHARACTER_CONSTANT_OVERFLOW );

	// Convert to UTF-8
	status_t status;
	unicode::uchar8_t mb_buf[8];
	size_t bytes_stored = 0, bytes_passed = 0;
	if( unicode::utf8::convert( mb_buf, sizeof(mb_buf), &uch16, 2, &bytes_stored, &bytes_passed ).failed_status( status ) )
		return status;

	// Add to buffer
	buf.add_part( mb_buf, bytes_stored );
	s = ep;

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::_esc_u32( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding )
{
	if( ce_none != encoding && ce_utf32 != encoding )
		return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );

	if( end_s - s < 8 )
		return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );

	const char * ep = nullptr;
	unicode::uchar32le_t uch = _strntoul( s, 8, &ep, 16 );
	if( nullptr == ep || ep - s != 8 )	// at least eight characters should be present
		return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );

	// Convert to UTF-8
	status_t status;
	unicode::uchar8_t mb_buf[8];
	size_t bytes_stored = 0, bytes_passed = 0;
	unicode::utf8::convert( mb_buf, sizeof(mb_buf), &uch, 4, &bytes_stored, &bytes_passed );

	// Add to buffer
	buf.add_part( mb_buf, bytes_stored );
	s = ep;

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::_esc_num( c_pp::parser & par,
	pp::string & buf, const char *& s, const char * end_s,
	int radix, size_t nchars, char_encoding_t encoding )
{
	status_t status;
	bool overflow = false;
	uint32_t uch;

	if( s >= end_s )
		return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
	if( ce_widechar == encoding )
		encoding = par.get_c_flags( c_flags_t::fl_long_wchar_t ) ? ce_utf32 : ce_utf16;

	// C++23 delimited escape sequence?
	if( '{' == *s )
	{
		const char * start = ++ s;

		// Parse integer
		uch = _strntoul( s, nchars, &s, radix );
		if( nullptr == s || s - start < 1 )	// at least one character should be present
			return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
		if( s >= end_s || '}' != *s )
			par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
		else
			++ s;	// skip '}'
	}
	// It's a regular escape sequence
	else
	{
		const char * start = s;

		// Parse integer
		uch = _strntoul( s, nchars, &s, radix );
		if( nullptr == s || s - start < 1 )	// at least one character should be present
			return par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
		if( s < end_s && isdigit( *s ) )
			par.message( syntax::IMSG_WARNING_DIGIT_NOT_PART_OF_NUMERIC_C, *s );
	}

	// Convert to UTF-8
	unicode::uchar8_t mb_buf[8];
	size_t bytes_stored = 0, bytes_passed = 0;
	switch( encoding )
	{
	case ce_utf8:
	default:
		mb_buf[0] = unicode::uchar8_t(uch & 0xFF);
		overflow = 0 != (uch & ~0xFFUL);
		bytes_stored = 1;
		break;
	case ce_utf16:
		overflow = 0 != (uch & ~0xFFFFUL);
		unicode::utf8::convert( mb_buf, sizeof(mb_buf), (const unicode::uchar16le_t *) &uch, 2, &bytes_stored, &bytes_passed );
		break;
	case ce_utf32:
		overflow = 0 != (uch & ~0xFFFFFFFFUL);
		unicode::utf8::convert( mb_buf, sizeof(mb_buf), (const unicode::uchar32le_t *) &uch, 4, &bytes_stored, &bytes_passed );
		break;
	}
	buf.add_part( (const char *) mb_buf, bytes_stored );
	//par._write_numeric_with_encoding( buf, encoding, uch );

	// Check for overflow
	if( overflow )
		par.message( syntax::IMSG_WARNING_CHARACTER_CONSTANT_OVERFLOW );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::_esc_dec( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding )
{
	return _esc_num( par, buf, s, end_s, 10, __min( 3, end_s - s ), encoding );
}
const status_t f1::c_pp::parser::_esc_hex( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding )
{
	++ s;	// skip 'x' or 'X' prefix

	// According to the C standard, length of the hexadecimal character escape is unlimited, but 
	// "shall be in the range of representable values for the type unsigned char for an integer character constant, or
	// the unsigned type corresponding to wchar_t for a wide character constant."
	// Without leading '0' characters, such constants can take 2 characters maximum for unsigned char constant and
	// 4 characters maximum for unsigned wide char constants. Additionally, we support UTF-32 encoding with
	// 8 characters maximum.
	const char * start = s;
	while( s < end_s && '0' == *s )	// skip all leading '0'
		++ s;
	if( s > start && (s >= end_s || !isxdigit(*s)) )
		-- s;

	ptrdiff_t max_len;
	switch( encoding ) {			// calculate length of the representation
		case ce_widechar:
		case ce_utf16:
			max_len = 4;
			break;
		case ce_utf32:
			max_len = 8;
			break;
		default:
			max_len = 2;
			break;
	}

	// Call common parser
	return _esc_num( par, buf, s, end_s, 16, __min( max_len, end_s - s ), encoding );
}
const status_t f1::c_pp::parser::_esc_oct( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding )
{
	return _esc_num( par, buf, s, end_s, 8, __min( 3, end_s - s ), encoding );
}
const status_t f1::c_pp::parser::_esc_oct_new( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding )
{
	++s;	// skip 'o' or 'O' prefix
	return _esc_num( par, buf, s, end_s, 8, __min( 3, end_s - s ), encoding );
}

const status_t f1::c_pp::parser::load_default_escapes()
{
	m_esc_chars['\n'].m_callback = _esc_nl;
	m_esc_chars['\r'].m_callback = _esc_cr;
	m_esc_chars['&' ].m_callback = _esc_html;
	m_esc_chars['c' ].m_callback = _esc_ctrl;
	m_esc_chars['N' ].m_callback = _esc_ucn;
	m_esc_chars['o' ].m_callback = _esc_oct_new;
	m_esc_chars['O' ].m_callback = _esc_oct_new;
	m_esc_chars['u' ].m_callback = _esc_u16;
	m_esc_chars['U' ].m_callback = _esc_u32;
	m_esc_chars['x' ].m_callback = _esc_hex;

	m_esc_chars['0'].m_callback =
	m_esc_chars['1'].m_callback =
	m_esc_chars['2'].m_callback =
	m_esc_chars['3'].m_callback =
	m_esc_chars['4'].m_callback =
	m_esc_chars['5'].m_callback =
	m_esc_chars['6'].m_callback =
	m_esc_chars['7'].m_callback = _esc_oct;	// note that '\8' and '\9' are unknown escapes

	m_esc_chars['\''].m_value = '\'';
	m_esc_chars['\"'].m_value = '\"';
	m_esc_chars['?' ].m_value = '?';
	m_esc_chars['\\'].m_value = '\\';
	m_esc_chars['a' ].m_value = '\a';
	m_esc_chars['b' ].m_value = '\b';
	m_esc_chars['e' ].m_value = 0x1B;
	m_esc_chars['f' ].m_value = '\f';
	m_esc_chars['n' ].m_value = '\n';
	m_esc_chars['r' ].m_value = '\r';
	m_esc_chars['t' ].m_value = '\t';
	m_esc_chars['v' ].m_value = '\v';

	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::set_esc_char_value( int ch, int val )
{
	__debugbreak_if( 0x7f <= ch )
		return STATUS_ERROR_INVALID_PARAMETER;

	m_esc_chars[ch].m_value = val;
	m_esc_chars[ch].m_callback = nullptr;

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::set_esc_char_callback( int ch, esc_char_callback fn )
{
	__debugbreak_if( 0x7f <= ch )
		return STATUS_ERROR_INVALID_PARAMETER;

	m_esc_chars[ch].m_callback = fn;

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::parse_esc_char( pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding ) const
{
	if( s >= end_s )
		return STATUS_SUCCESS;
	if( 0x7f <= *s )
		return STATUS_ERROR_INVALID_PARAMETER;

	const esc_desc_t & desc = m_esc_chars[*s];
	if( nullptr != desc.m_callback )
		return (*desc.m_callback)( *const_cast<self *>(this), buf, s, end_s, encoding );
	if( 0 == desc.m_value )
		return STATUS_SUCCESS;	// not found, ignore

	++ s;
	buf.add_part( (const char *) &desc.m_value, 1 );

	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::find_ucn( pp::string & buf, const char * s, size_t len ) const
{
	__unreferenced_parameter( buf );
	__unreferenced_parameter( s );
	__unreferenced_parameter( len );
	return STATUS_ERROR_TODO;	// TODO: use ICU for querying character names
}

#if 0
// use parse_esc_char instead
const status_t f1::c_pp::parser::parse_escape( max_int_t & ch, size_t & nbytes )
{
	unsigned uch;
	const char * s;
	const char * ep = nullptr;

	nbytes = 1;

//	if( data_left() < 1 )
//		return SUCCESS;
	uch = m_parser->fetch_char();
	s = m_parser->get_ptr();

	switch( uch )
	{
	case '\'':	ch = '\'';	break;
	case '\"':	ch = '\"';	break;
	case '?':	ch = '?';	break;
	case '\\':	ch = '\\';	break;
	case 'a':	ch = '\a';	break;
	case 'b':	ch = '\b';	break;
	case 'e':	ch = 0x1B;	break;
	case 'f':	ch = '\f';	break;
	case 'n':	ch = '\n';	break;
	case 'r':	ch = '\r';	break;
	case 't':	ch = '\t';	break;
	case 'v':	ch = '\v';	break;
	case '\n':	// line continuation
		m_parser->skip_char( '\r' );
		nbytes = 0;
		break;
	case '\r':	// line continuation
		m_parser->skip_char( '\n' );
		nbytes = 0;
		break;
	case 'c':	// \cC
		uch = m_parser->fetch_char();
		if( 'A' <= uch && uch <= 'Z' )
			ch = uch - 'A' + 1;
		else if( 'a' <= uch && uch <= 'z' )
			ch = uch - 'a' + 1;
		else
			return message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
		break;
	case 'u':
		if( m_parser->skip_char( '{' ) )	// \u{n...n}
		{
			s = m_parser->data();
			uch = _strntoul( s, m_parser->data_left(), &ep, 16 );
			if( nullptr == ep || ep - s < 4 || '}' != *ep )	// at least three characters should be present: '{n}'
				return message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
			++ ep;	// skip '}'
		}
		else					// \unnnn
		{
			uch = _strntoul( s, 4, &ep, 16 );
			if( nullptr == ep || ep - s < 4 )	// at least four characters should be present
				return message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
		}
		m_parser->set_ptr( ep );

		ch = uch;
		nbytes = 2;

		break;
	case 'U':	// \Unnnnnnnn
		uch = _strntoul( s, 8, &ep, 16 );
		if( nullptr == ep || ep - s != 8 )	// at least eight characters should be present
			return message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
		m_parser->set_ptr( ep );

		ch = uch;
		nbytes = 4;

		break;
	case 'x':	// \xnn
		uch = _strntoul( s, 2, &ep, 16 );
		if( nullptr == ep || ep - s < 1 )	// at least one or two characters should be present
			return message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );

		ch = (char) uch;
		break;
	default:
		if( isdigit( uch ) )
		{
			// Parse single '\0' escape
//			if( '0' == ch && !isdigit( m_parser->peek_char_at( 1 ) ) )
//			{
//				ch = '\0';
//				break;
//			}

//			if( m_parser->data_left() >= 3 )	// '\nnn': at least three characters should be present
			{
				const char * s = m_parser->get_ptr();
				uch = _strntoul( s, 3, &ep, 8 );
				if( nullptr == ep || ep - s < 1 )	// at least one character should be present
					return message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
				m_parser->set_ptr( ep );

				ch = (char) uch;
				nbytes = ep - s;
			}
//			else
//				return message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
		}
		break;
	}
	return STATUS_SUCCESS;
}
#endif

/*END OF parser_escape.cxx*/
