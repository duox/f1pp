/*print_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::print_directive::print_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::f1pp::print_directive::get_name() const
{
	return "print";
}
// {15359DF2-EDEC-4DAC-9283-3329074630A4}
const f1::guid_t f1::f1pp::print_directive::m_guid =
{ 0x15359df2, 0xedec, 0x4dac, { 0x92, 0x83, 0x33, 0x29, 0x07, 0x46, 0x30, 0xa4 } };
const f1::guid_t & f1::f1pp::print_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::print_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return (cap_keep_text_eol) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::print_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Skip separating space character
	const int ch = par->peek_char();
	if( isspace( ch ) && '\n' != ch && '\r' != ch )
		par->skip_char();

	// Expand macros in the string
	c_pp_par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::print_directive::execute( pp::Iparser & parser, const pp::string & buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Dequotize string if required (this involves exact un-stringification)
	pp::string val;
	if( c_pp_par->unstringify_chars( val, buf ).failed_status( status ) )
		return status;

	// Display message
	const core::string & str = val.get_string();
	parser.get_msgout_context().print( "%s", str.c_str() );
	if( '\n' != *str.rbegin() && '\r' != *str.rbegin() )
		parser.get_msgout_context().print( "\n" );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::print_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::f1pp::print_directive> drc = type_cast<f1::f1pp::print_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, pp::string( buf ) );
#else
	return static_cast<f1::f1pp::print_directive *>( param )->execute( parser, pp::string( buf ) );
#endif
}

/*END OF print_directive.cxx*/
