/*pragma_message.cxx*/
/** @file
 *
 * @brief Implementation of `#pragma message("text")'.
 *
 *	Note that DigitalMars compiler uses
 * #pragma message "text"
 *	form, without parenthesis.
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::comext::pragma_message::pragma_message( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::c_pp::comext::pragma_message::get_name() const
{
	return "message";
}
// {521C7E81-C50C-4412-AFED-C2C186890A8B}
const f1::guid_t f1::c_pp::comext::pragma_message::m_guid =
{ 0x521c7e81, 0xc50c, 0x4412, { 0xaf, 0xed, 0xc2, 0xc1, 0x86, 0x89, 0xa, 0x8b } };
const f1::guid_t & f1::c_pp::comext::pragma_message::get_guid() const
{
	return m_guid;
}

// Operations
const status_t f1::c_pp::comext::pragma_message::parse( pp::Iparser & parser, const pp::string & param_str ) const
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
	bool do_not_print = false;
	bool display_eol_error = true;
	bool expected_string_error = true;
	core::string buffer;
	buffer.reserve( end_s - s );
	while( s < end_s )
	{
		// Skip spaces and check for closing parentheses
		c_pp_par->skip_real_spaces_and_comments( s, end_s );
		if( s >= end_s || ')' == *s )
		{
			if( !parenthesis && ')' == *s )
			{
				c_pp_par->message( syntax::IMSG_WARNING_UNEXPECTED_S, "')'" );
				parenthesis = true;		// pretend that there was an openning parenthesis
				do_not_print = true;	// do not print string if it's empty
				expected_string_error = false;
			}
			break;
		}

		// Fetch string
		if( '\"' != *s )
		{
			expected_string_error = true;
			parenthesis = false;		// do not check for closing parenthesis
			display_eol_error = false;	// do not check for extra characters after the directive
			break;
		}
		expected_string_error = false;
		pp::string buf;
		c_pp_par->skip_string( s, end_s, &buf );
		if( 0 == buf.length() )
			break;
		buffer.append( buf.data(), buf.length() );
	}
	if( expected_string_error )
	{
		c_pp_par->message( syntax::IMSG_ERROR_EXPECTED_STRING );
		do_not_print = true;		// do not print string if it's empty
		buffer.clear();
	}

	// Parse closing parenthesis
	if( parenthesis )
	{
		if( s >= end_s || ')' != *s )
			return parser.message( syntax::IMSG_WARNING_EXPECTED_S, "')'" );
		++ s;
	}

	// Check end of statement
	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	if( s < end_s && display_eol_error )
		return parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );

	// Display the string
	if( !do_not_print || !buffer.empty() )
	{
		buffer.push_back( '\n' );
		parser.get_msgout_context().print( buffer.c_str() );
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_message.cxx*/
