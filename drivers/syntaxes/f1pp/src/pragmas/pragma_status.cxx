/*pragma_status.cxx*/
/** @file
 *
 * @brief Implementation of `#pragma status("text")'.
 *
 */
#include "../pch.h"
#include "../main.h"

f1::f1pp::pragma_status::pragma_status( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::f1pp::pragma_status::get_name() const
{
	return "status";
}
// {0E63612F-0099-4BA7-B763-884E04833106}
const f1::guid_t f1::f1pp::pragma_status::m_guid =
{ 0x0e63612f, 0x0099, 0x4ba7, { 0xb7, 0x63, 0x88, 0x4e, 0x04, 0x83, 0x31, 0x06 } };
const f1::guid_t & f1::f1pp::pragma_status::get_guid() const
{
	return m_guid;
}

// Operations
const status_t f1::f1pp::pragma_status::parse( pp::Iparser & parser, const pp::string & param_str ) const
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

	// Set status
	if( parser.get_context()->is_event_enabled( pp::context::event_id_t::ev_status_text_changed ) )
	{
		pp::string val( buffer );
		parser.get_context()->invoke_event( pp::context::event_id_t::ev_status_text_changed, &val );
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_status.cxx*/
