/*pragma_info.cxx*/
/** @file
 *
 * @brief Implementation of `#pragma info("text")'.
 *
 */
#include "../pch.h"
#include "../main.h"

f1::f1pp::pragma_info::pragma_info( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::f1pp::pragma_info::get_name() const
{
	return "info";
}
// {95F786F2-B2B4-4480-B628-4D800253DC08}
const f1::guid_t f1::f1pp::pragma_info::m_guid =
{ 0x95f786f2, 0xb2b4, 0x4480, { 0xb6, 0x28, 0x4d, 0x80, 0x2, 0x53, 0xdc, 0x8 } };
const f1::guid_t & f1::f1pp::pragma_info::get_guid() const
{
	return m_guid;
}

// Operations
const status_t f1::f1pp::pragma_info::parse( pp::Iparser & parser, const pp::string & param_str ) const
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	const char * s = param_str.data();
	const char * end_s = param_str.data_end();
	if( nullptr == s )
		return parser.message( syntax::IMSG_WARNING_EXPECTED_S, "'('" );

	// Parse opening parenthesis
	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	bool parenthesis = false;
	if( '(' == *s )
	{
		parenthesis = true;
		++ s;
	}

	// Concatenate all strings inside the ( and )
	bool display_eol_error = true;
	core::string buffer;
	buffer.reserve( end_s - s );
	while( s < end_s )
	{
		// Skip spaces and check for closing parentheses
		c_pp_par->skip_real_spaces_and_comments( s, end_s );
		if( ')' == *s )
		{
			if( !parenthesis )
			{
				c_pp_par->message( syntax::IMSG_WARNING_UNEXPECTED_S, "')'" );
				parenthesis = true;	// pretend that there was an openning parenthesis
			}
			break;
		}

		// Fetch string
		pp::string buf;
		c_pp_par->skip_string( s, end_s, &buf );
		if( 0 == buf.length() )
		{
			c_pp_par->message( syntax::IMSG_ERROR_EXPECTED_STRING );
			parenthesis = false;		// do not check for closing parenthesis
			display_eol_error = false;	// do not check for extra characters after the directive
			break;
		}
		buffer.append( buf.data(), buf.length() );
	}

	// Parse closing parenthesis
	if( parenthesis )
	{
		if( ')' != *s )
			return parser.message( syntax::IMSG_WARNING_EXPECTED_S, "')'" );
		++ s;
	}

	// Check end of statement
	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	if( s < end_s && display_eol_error )
		return parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );

	// Display the string
	parser.message( syntax::IMSG_INFO_USER_N, buffer.length(), buffer.data() );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_info.cxx*/
