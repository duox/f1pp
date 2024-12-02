/*echo_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::echo_directive::echo_directive( unsigned flags ) __noexcept:
	super( flags ),
	m_nonl( false )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::f1pp::echo_directive::get_name() const
{
	return "echo";
}
// {36F5D4B5-1A9A-442E-BCB3-37D5E61FC961}
const f1::guid_t f1::f1pp::echo_directive::m_guid =
{ 0x36f5d4b5, 0x1a9a, 0x442e, { 0xbc, 0xb3, 0x37, 0xd5, 0xe6, 0x1f, 0xc9, 0x61 } };
const f1::guid_t & f1::f1pp::echo_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::echo_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return (cap_keep_text_eol) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::echo_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	m_nonl = f1par->exists_directive_attribute( "nonl" );

	// Skip separating space character
	const int ch = par->peek_char();
	if( isspace( ch ) && '\n' != ch && '\r' != ch )
		par->skip_char();

	// Expand macros in the string
	f1par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::echo_directive::execute( pp::Iparser & parser, const pp::string & buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> f1par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Dequotize string if required (this involves exact un-stringification)
	pp::string val;
	if( f1par->unstringify_chars( val, buf ).failed_status( status ) )
		return status;

	// Output data
	parser.output( val );
	if( m_nonl )
		f1par->set_ignore_next_newline();
	/*if( !m_nonl )
	{
		parser.output( "\n" );
		parser.get_parser()->on_nl();
	}*/

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::echo_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::f1pp::echo_directive> drc = type_cast<f1::f1pp::echo_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, pp::string( buf ) );
#else
	return static_cast<f1::f1pp::echo_directive *>( param )->execute( parser, pp::string( buf ) );
#endif
}

/*END OF echo_directive.cxx*/
