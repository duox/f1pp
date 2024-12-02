/*parser_integer.cxx*/
#include "pch.h"
#include "main.h"

#define CPP_SUFFIX_LONG			0x0001
#define CPP_SUFFIX_LONGLONG		(0x0002 | CPP_SUFFIX_LONG)
#define CPP_SUFFIX_UNSIGNED		0x0004
static const struct {
	unsigned	m_mask;
	const char *m_name;
	size_t		m_length;
} suffix_desc[] =
{
	{ CPP_SUFFIX_LONGLONG,	STR_LEN_PAIR( "I64" ) },{ CPP_SUFFIX_LONGLONG,	STR_LEN_PAIR( "i64" ) },
	{ CPP_SUFFIX_LONGLONG,	STR_LEN_PAIR( "LL" ) },	{ CPP_SUFFIX_LONGLONG,	STR_LEN_PAIR( "ll" ) },
	{ CPP_SUFFIX_LONG,		STR_LEN_PAIR( "L" ) },	{ CPP_SUFFIX_LONG,		STR_LEN_PAIR( "l" ) },
	{ CPP_SUFFIX_UNSIGNED,	STR_LEN_PAIR( "U" ) },	{ CPP_SUFFIX_UNSIGNED,	STR_LEN_PAIR( "u" ) },
	{ CPP_SUFFIX_LONGLONG,	STR_LEN_PAIR( "Z" ) },	{ CPP_SUFFIX_LONGLONG,	STR_LEN_PAIR( "z" ) },
};

bool f1::c_pp::parser::parse_integer( pp::variant & res )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	bool parsed = parse_integer( res, s, end_s );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return parsed;
}
const status_t f1::c_pp::parser::parse_integer( pp::variant & res, bool & parsed )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	parsed = parse_integer( res, s, end_s );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return STATUS_SUCCESS;
}
bool f1::c_pp::parser::parse_integer( pp::variant & res, const pp::string & buf )
{
	const char * s = buf.data();
	bool result = parse_integer( res, s, buf.data_end() );
	if( !result )
		return false;
	if( s != buf.data_end() )
		return false;
	return true;
}
bool f1::c_pp::parser::parse_integer( pp::variant & res, const char *& s, const char * end_s )
{
	int radix = 10;
	int sign = 0;
	unsigned length = 0;
	bool was_overflow_reported = false;

	if( nullptr == s )
		return false;

	// Parse sign
	if( '-' == *s || '+' == *s )
	{
		sign = *s ++;
		if( '\\' == *s )
			++ s,
			skip_newline( s, end_s );
	}

	// Parse radix specifier
	if( '0' == *s )
	{
		++ s;
		if( '\\' == *s )
			++ s,
			skip_newline( s, end_s );

		if( 'x' == *s || 'X' == *s )
		{
			radix = 16;
			++ s;
		}
		else if( 'b' == *s || 'B' == *s )	// TODO: C++14
		{
			radix = 2;
			++ s;
		}
		else if( 'o' == *s || 'O' == *s )	// TODO: a proposal
		{
			radix = 8;
			++ s;
		}
		else
		{
			radix = 8;
			length = 1;
			if( s < end_s && isodigit( *s ) )
				message( syntax::IMSG_WARNING_OCTAL_CONSTANT );
		}
	}

	// Parse numeric
	int64_t value = 0;
	while( s < end_s )
	{
		// Skip new line break
		if( '\\' == *s )
			++ s,
			skip_newline( s, end_s );

		// Skip digit separator
		if( '\'' == *s )
		{
			++ s;
			continue;	// TODO: since C++14
		}
		if( '_' == *s )
		{
			++ s;
			continue;	// TODO: extension
		}

		// Fetch digit
		int val;
		if( IN_RANGE( *s, '0', '9' ) )
			val = *s - '0';
		else if( IN_RANGE( *s, 'A', 'Z' ) )
			val = *s - 'A' + 10;
		else if( IN_RANGE( *s, 'a', 'z' ) )
			val = *s - 'a' + 10;
		else
			break;
		if( val >= radix )
			break;
		++ s;
		++ length;

		// Apply digit's value
		const int64_t new_value = value*radix + val;
		if( new_value < value && was_overflow_reported )
		{
			was_overflow_reported = true;
			message( syntax::IMSG_WARNING_CONSTANT_OVERFLOW );
		}
		value = new_value;
	}
	if( 0 == length )
		return false;

	// Parse suffixes
	unsigned suffix_mask = 0;
	size_t i;
	while( s < end_s )
	{
		for( i = 0; i < countof(suffix_desc); ++ i )
		{
			if( size_t(end_s - s) < suffix_desc[i].m_length )
				continue;
			switch( suffix_desc[i].m_length )
			{
			default:
				if( !memcmp( s, suffix_desc[i].m_name, suffix_desc[i].m_length ) )
					break;
				continue;
			case 4:	if( suffix_desc[i].m_name[3] != s[3] )	continue;
			case 3:	if( suffix_desc[i].m_name[2] != s[2] )	continue;
			case 2:	if( suffix_desc[i].m_name[1] != s[1] )	continue;
			case 1:	if( suffix_desc[i].m_name[0] != s[0] )	continue;
			}
			break;
		}
		if( i >= countof(suffix_desc) )	// no suffix found - break
			break;

		// Update suffix mask
		if( suffix_mask & suffix_desc[i].m_mask )
			message( syntax::IMSG_WARNING_EXCESSIVE_INTEGER_SUFFIX );
		suffix_mask |= suffix_desc[i].m_mask;
		s += suffix_desc[i].m_length;
	}

	// Check promotion rules
	if( suffix_mask & CPP_SUFFIX_UNSIGNED )
	{
		if( '-' == sign )
			message( syntax::IMSG_WARNING_UNARY_MINUS_APPLIED_TO_UINT );
	}
	else
	{
		if( value > INT_MAX )
			message( syntax::IMSG_WARNING_CONSTANT_OVERFLOW );
	}

	// Store parsed data
	const int64_t final_value =  '-' == sign ? (uint64_t) - (int64_t) value : value;
	res = final_value;
	if( suffix_mask & CPP_SUFFIX_UNSIGNED ||	// ignore all other suffixes
		value > INTMAX_MAX )					// integer literal cannot be represented as intmax_t
		res.set_type( pp::variant::ty_uint );
	else
		res.set_type( pp::variant::ty_int );

	// Exit
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::parser::parse_hex_integer( pp::variant & res, unsigned num_digits, bool & parsed )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	parsed = parse_hex_integer( res, num_digits, s, end_s );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return STATUS_SUCCESS;
}
bool f1::c_pp::parser::parse_hex_integer( pp::variant & res, unsigned num_digits, const pp::string & buf )
{
	const char * s = buf.data();
	bool result = parse_hex_integer( res, num_digits, s, buf.data_end() );
	if( !result )
		return false;
	if( s != buf.data_end() )
		return false;
	return true;
}
bool f1::c_pp::parser::parse_hex_integer( pp::variant & res, unsigned num_digits, const char *& s, const char * end_s )
{
	__debugbreak_if( num_digits > sizeof(res.as_int())*4 )
		return false;
	__debugbreak_if( num_digits > size_t(end_s - s) )
		return false;

	bool warn_overflow = true;
	unsigned i;
	uintmax_t val = 0;
	for( i = 0; i < num_digits; ++ i, ++ s )
	{
		const uintmax_t new_val = val * 16 + toxdigit( *s );
		if( new_val > val )
		{
			if( warn_overflow )
			{
				message( syntax::IMSG_WARNING_CONSTANT_OVERFLOW );
				warn_overflow = false;
			}
		}
		val = new_val;
	}

	return true;
}

/*END OF parser_integer.cxx*/
