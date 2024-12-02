/*status_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::status_directive::status_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::f1pp::status_directive::get_name() const
{
	return "status";
}
// {89F7C92A-4FE4-4D6C-B283-65E80F1382B4}
const f1::guid_t f1::f1pp::status_directive::m_guid =
{ 0x89f7c92a, 0x4fe4, 0x4d6c, { 0xb2, 0x83, 0x65, 0xe8, 0x0f, 0x13, 0x82, 0xb4 } };
const f1::guid_t & f1::f1pp::status_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::status_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::status_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch string
	if( isspace( par->peek_char() ) )
		par->skip_char();

	// Expand macros in the string
	if( parser.get_context()->get_flags( pp::context::fl_message_macro_expansion ) )
	{
		c_pp_par->capture_line( line_capture_callback, this );
	}
	else
	{
		pp::string	buf;
		if( c_pp_par->parse_to_newline( buf ).failed_status( status ) )
			return status;
		execute( parser, buf );
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::status_directive::execute( pp::Iparser & parser, const pp::string & buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Dequotize string if required (this involves exact un-stringification)
	pp::string val;
	if( c_pp_par->get_c_flags( parser::c_flags_t::fl_dequotize_messages ) )
	{
		if( c_pp_par->unstringify_chars( val, buf ).failed_status( status ) )
			return status;
	}
	else
		val = buf;

	// Set status
	parser.get_context()->invoke_event( pp::context::event_id_t::ev_status_text_changed, &val );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::status_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::f1pp::status_directive> drc = type_cast<f1::f1pp::status_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, pp::string( buf ) );
#else
	return static_cast<f1::f1pp::status_directive *>( param )->execute( parser, pp::string( buf ) );
#endif
}

/*END OF status_directive.cxx*/
