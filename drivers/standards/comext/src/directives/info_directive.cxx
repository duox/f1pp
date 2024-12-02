/*info_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::comext::info_directive::info_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::comext::info_directive::get_name() const
{
	return "info";
}
// {625B3E4E-5D95-4c6d-8DA9-3A0DCE563AD6}
const f1::guid_t f1::c_pp::comext::info_directive::m_guid =
{ 0x625b3e4e, 0x5d95, 0x4c6d, { 0x8d, 0xa9, 0x3a, 0xd, 0xce, 0x56, 0x3a, 0xd6 } };
const f1::guid_t & f1::c_pp::comext::info_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::comext::info_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::comext::info_directive::parse( pp::Iparser & parser )
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
const status_t f1::c_pp::comext::info_directive::execute( pp::Iparser & parser, const pp::string & buf )
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

	// Display info and set fatal (user) info flag
	parser.message( syntax::IMSG_INFO_USER_N, val.length(), val.data() );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::comext::info_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::c_pp::comext::info_directive> drc = type_cast<f1::c_pp::comext::info_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, pp::string( buf ) );
#else
	return static_cast<f1::c_pp::comext::info_directive *>( param )->execute( parser, pp::string( buf ) );
#endif
}

/*END OF info_directive.cxx*/
