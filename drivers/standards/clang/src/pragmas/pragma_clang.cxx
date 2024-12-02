/*pragma_clang.cxx*/
/** @file
 *
 * @brief Pragma clang implementation.
 *
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::clang::pragma_clang::pragma_clang( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::c_pp::clang::pragma_clang::get_name() const
{
	return "clang";
}
// {A42E4F6F-B3C0-458D-97A4-604007F83379}
const f1::guid_t f1::c_pp::clang::pragma_clang::m_guid =
{ 0xa42e4f6f, 0xb3c0, 0x458d, { 0x97, 0xa4, 0x60, 0x40, 0x07, 0xf8, 0x33, 0x79 } };
const f1::guid_t & f1::c_pp::clang::pragma_clang::get_guid() const
{
	return m_guid;
}

const status_t f1::c_pp::clang::pragma_clang::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Add all pragmas
	append_subpragma( *new(std::nothrow) pragma_clang_deprecated );
	append_subpragma( *new(std::nothrow) pragma_clang_final );

	append_known_subpragma( "attribute" );
	append_known_subpragma( "__debug" );
	append_known_subpragma( "diagnostic" );
	append_known_subpragma( "fp" );
	append_known_subpragma( "loop" );
	append_known_subpragma( "optimize" );
	append_known_subpragma( "restrict_expansion" );
	append_known_subpragma( "section " );
	append_known_subpragma( "system_header" );

	// Exit
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations
/*
const status_t f1::c_pp::clang::pragma_clang::parse( pp::Iparser & parser, const pp::string & param_str ) const
{
	core::Tref<pp::Idirective> drct;
	if( parser.find_directive( drct, f1::c_pp::c99::pragma_directive::m_guid ).failed() )
		return STATUS_ERROR_NOT_FOUND;

	core::Tref<f1::c_pp::c99::pragma_directive> prgm = drct.dynamic_pointer_cast<f1::c_pp::c99::pragma_directive>();
	__debugbreak_if( nullptr == prgm )
		return STATUS_ERROR_INVALID_TYPE;

	const char * s = param_str.data();
	const char * end_s = param_str.data_endw();

	// Fetch pragma name
	pp::string ident;
	c_par->skip_real_spaces_and_comments( s, end_s );
	if( c_par->parse_macro_identifier( ident, s, end_s ).failed() )
	{
		par->skip_to_nl();
		parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		return STATUS_SUCCESS;
	}

	// Execute the sub-pragma
	return prgm->execute( parser, ident, pp::string( s, end_s - s ), this );
}
*/
/*END OF pragma_clang.cxx*/
