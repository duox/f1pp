/*fatalerror_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::fatalerror_directive::fatalerror_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::f1pp::fatalerror_directive::get_name() const
{
	return "fatalerror";
}
// {C6EAB7F5-47F7-45ab-B432-6127CDDB3889}
const f1::guid_t f1::f1pp::fatalerror_directive::m_guid =
{ 0xc6eab7f5, 0x47f7, 0x45ab, { 0xb4, 0x32, 0x61, 0x27, 0xcd, 0xdb, 0x38, 0x89 } };
const f1::guid_t & f1::f1pp::fatalerror_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::fatalerror_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::fatalerror_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	if( !c_pp_par->is_directive_available( *this ) )
		c_pp_par->message( syntax::IMSG_WARNING_DIRECTIVE_NOT_AVAILABLE, get_name() );

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch string
	if( isspace( par->peek_char() ) )
		par->skip_char();

	// Expand macros in the string
	if( parser.get_context()->get_flags( pp::context::fl_message_macro_expansion ) )
		c_pp_par->capture_line( line_capture_callback, this );
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
const status_t f1::f1pp::fatalerror_directive::execute( pp::Iparser & parser, const pp::string & buf )
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

	// Display error and set fatal (user) error flag
	parser.message( syntax::IMSG_FATAL_ERROR_USER_N, val.length(), val.data() );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::fatalerror_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::f1pp::fatalerror_directive> drc = type_cast<f1::f1pp::fatalerror_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, pp::string( buf ) );
#else
	return static_cast<f1::f1pp::fatalerror_directive *>( param )->execute( parser, pp::string( buf ) );
#endif
}

/*END OF fatalerror_directive.cxx*/
