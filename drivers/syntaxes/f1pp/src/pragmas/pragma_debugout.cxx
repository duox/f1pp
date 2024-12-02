/*pragma_debugout.cxx*/
/** @file
 *
 * @brief Implementation of `#pragma debugout("text")'.
 *
 */
#include "../pch.h"
#include "../main.h"

f1::f1pp::pragma_debugout::pragma_debugout( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::f1pp::pragma_debugout::get_name() const
{
	return "debugout";
}
// {C53EA798-280D-42ef-89FA-5790E2B166E6}
const f1::guid_t f1::f1pp::pragma_debugout::m_guid =
{ 0xc53ea798, 0x280d, 0x42ef, { 0x89, 0xfa, 0x57, 0x90, 0xe2, 0xb1, 0x66, 0xe6 } };
const f1::guid_t & f1::f1pp::pragma_debugout::get_guid() const
{
	return m_guid;
}

// Operations
const status_t f1::f1pp::pragma_debugout::parse( pp::Iparser & parser, const pp::string & param_str ) const
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
	buffer.push_back( '\n' );

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
	core::debug::print( buffer.c_str() );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_debugout.cxx*/
