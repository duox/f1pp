/*message_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::message_directive::message_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::f1pp::message_directive::get_name() const
{
	return "message";
}
// {C433993B-48DE-4859-89A6-3DC2EF18B85B}
const f1::guid_t f1::f1pp::message_directive::m_guid =
{ 0xc433993b, 0x48de, 0x4859, { 0x89, 0xa6, 0x3d, 0xc2, 0xef, 0x18, 0xb8, 0x5b } };
const f1::guid_t & f1::f1pp::message_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::message_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::message_directive::parse( pp::Iparser & parser )
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
const status_t f1::f1pp::message_directive::execute( pp::Iparser & parser, const pp::string & buf )
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

	// Display message to stderr
	parser.message( syntax::IMSG_RAW_N, val.length(), val.data() );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::message_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::f1pp::message_directive> drc = type_cast<f1::f1pp::message_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, pp::string( buf ) );
#else
	return static_cast<f1::f1pp::message_directive *>( param )->execute( parser, pp::string( buf ) );
#endif
}

/*END OF message_directive.cxx*/
