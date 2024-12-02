/*abort_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::abort_directive::abort_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::abort_directive::get_name() const
{
	return "abort";
}

// {CF7A9F7F-26CB-4ABA-B395-8D757192E97C}
const f1::guid_t f1::f1pp::abort_directive::m_guid =
{ 0xcf7a9f7f, 0x26cb, 0x4aba, { 0xb3, 0x95, 0x8d, 0x75, 0x71, 0x92, 0xe9, 0x7c } };
const f1::guid_t & f1::f1pp::abort_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::abort_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

// Operations
const status_t f1::f1pp::abort_directive::parse( pp::Iparser & parser )
{
	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Skip separating space character
	const int ch = par->peek_char();
	if( isspace( ch ) && '\n' != ch && '\r' != ch )
		par->skip_char();

	// Expand macros in the string
	f1pp_par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::abort_directive::execute( pp::Iparser & parser, const pp::string & buf )
{
	status_t status;

	// Display message if it's specified
	if( !buf.empty() )
	{
		// Check current state
		core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
		__debugbreak_if( nullptr == c_pp_par )
			return STATUS_ERROR_NOT_INITIALIZED;

		// Dequotize string if required (this involves exact un-stringification)
		pp::string val;
		if( c_pp_par->unstringify_chars( val, buf ).failed_status( status ) )
			return status;

		// Display message
		if( !val.empty() )
		{
			//parser.message( syntax::IMSG_DEBUG_USER_N, val.length(), val.data() );
			parser.get_msgout_context().print( "%s", core::string( val.data(), val.length() ).c_str() );
			parser.get_msgout_context().print( "\n" );
		}
	}

	// Abort execution
	parser.abort_parser();

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::abort_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::f1pp::abort_directive> drc = type_cast<f1::f1pp::abort_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, pp::string( buf ) );
#else
	return static_cast<f1::f1pp::abort_directive *>( param )->execute( parser, pp::string( buf ) );
#endif
}

/*END OF abort_directive.cxx*/
