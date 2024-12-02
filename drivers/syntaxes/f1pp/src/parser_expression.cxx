/*parser_expression.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::f1pp::parser::parse_value( pp::variant & result, const char * & s, const char * end_s, unsigned flags )
{
	status_t status;

	// Try to parse string
	char_encoding_t str_enc;
	int str_end_ch;
	string_parse_flags_t string_parse_flags = string_parse_flags_t::null;
	if( parse_string_header( s, end_s, str_enc, str_end_ch, string_parse_flags ) )
	{
		// Parse string
		if( m_c_flags & c_flags_t::fl_disable_string_escapes )
			string_parse_flags |= string_parse_flags_t::fl_parse_string_no_escapes;

		pp::string buf;
		skip_string( s, end_s, str_end_ch, &buf, string_parse_flags, str_enc );

		// Set the string as result
		if( '\'' == str_end_ch )
		{
			//_reverse_encoded_string( buf, str_enc );
			//const_cast<core::string &>(buf.get_string()).reverse();	// TODO: remove const_cast
			//convert_char_to_variant( result, buf, /*get_encoding*/( str_enc ) );
			convert_char_to_variant( result, buf, get_encoding( str_enc ) );
		}
		else
			result.set_string( buf.data(), buf.length() );

		// Exit
		return STATUS_SUCCESS;
	}

	// Try to parse container
	if( '{' == *s )
	{
		core::Tarray<pp::variant>	value_list;
		for( ; ; )
		{
			++ s;	// skip separator

			skip_spaces_and_comments( s, end_s );	// skip spaces, including new-lines
			if( s >= end_s )
			{
				message( c_pp::syntax::IMSG_ERROR_EXPECTED_VALUE );
				return STATUS_ERROR;
			}

			// Parse value
			pp::variant	* val = value_list.append_new();
			if( parse_value( *val, s, end_s, flags ).failed_status( status ) )
				return status;

			// Skip separator
			skip_spaces_and_comments( s, end_s );	// skip spaces, including new-lines
			if( '}' == *s )
			{
				++ s;
				break;
			}
			if( ',' != *s )
			{
				message( c_pp::syntax::IMSG_ERROR_EXPECTED_S_OR_S, "}", "," );
				return STATUS_ERROR;
			}
		}
		return result.set_list( value_list.data(), value_list.size() );
	}

	// Parse single operands
	if( super::parse_value( result, s, end_s, flags ).failed_status( status ) )
		return status;

	// Check whether it's a region
	skip_real_spaces_and_comments( s, end_s );
	if( s + 2 < end_s && '.' == s[0] && '.' == s[1] )
	{
		s += 2;

		// Parse second part of the range
		pp::variant value2;
		if( super::parse_value( value2, s, end_s, flags ).failed_status( status ) )
			return status;

		// And now parse the third part
		if( s + 2 < end_s && '.' == s[0] && '.' == s[1] )
		{
			s += 2;

			pp::variant value3;
			if( super::parse_value( value3, s, end_s, flags ).failed_status( status ) )
				return status;

			pp::variant value1;
			value1.move( result );
			result.set_range( value1, value3, value2 );
		}
		else
		{
			pp::variant value1;
			value1.move( result );
			result.set_range( value1, value2 );
		}
	}

	// Exit
	return status;
}

/*END OF parser_expression.cxx*/
