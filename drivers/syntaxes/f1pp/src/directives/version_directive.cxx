/*version_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::version_directive::version_directive( unsigned flags ):
	super( flags ),
	m_fail_if_not_matched( false )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::f1pp::version_directive::get_name() const
{
	return "version";
}
// {5F461FB0-8099-4782-A039-416FDFAF067A}
const f1::guid_t f1::f1pp::version_directive::m_guid =
{ 0x5f461fb0, 0x8099, 0x4782, { 0xa0, 0x39, 0x41, 0x6f, 0xdf, 0xaf, 0x06, 0x7a } };
const f1::guid_t & f1::f1pp::version_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::version_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return (cap_keep_text_eol) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::version_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Skip separating space character
	const int ch = par->peek_char();
	if( isspace( ch ) && '\n' != ch && '\r' != ch )
		par->skip_char();

	// Expand macros in the string
	f1par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::version_directive::execute( pp::Iparser & parser, const pp::string & buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> f1par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Dequotize string if required (this involves exact un-stringification)
	// TODO
//	pp::string val;
//	if( f1par->unstringify_chars( val, buf ).failed_status( status ) )
//		return status;

	// Output data
//	parser.output( val );
//	if( !m_nonl )
//		parser.get_parser()->on_nl();

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::version_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::f1pp::version_directive> drc = type_cast<f1::f1pp::version_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, pp::string( buf ) );
#else
	return static_cast<f1::f1pp::version_directive *>( param )->execute( parser, pp::string( buf ) );
#endif
}

/*END OF version_directive.cxx*/
