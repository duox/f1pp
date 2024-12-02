/*pragma_GCC.cxx*/
/** @file
 *
 * @brief Pragma gcc implementation.
 *
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::gcc::pragma_GCC::pragma_GCC( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::c_pp::gcc::pragma_GCC::get_name() const
{
	return "GCC";
}
// {4B0747B7-F8D1-48F6-AFD4-EF8CAF57B0E5}
const f1::guid_t f1::c_pp::gcc::pragma_GCC::m_guid =
{ 0x4b0747b7, 0xf8d1, 0x48f6, { 0xaf, 0xd4, 0xef, 0x8c, 0xaf, 0x57, 0xb0, 0xe5 } };
const f1::guid_t & f1::c_pp::gcc::pragma_GCC::get_guid() const
{
	return m_guid;
}

const status_t f1::c_pp::gcc::pragma_GCC::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Add all pragmas
	append_subpragma( *new(std::nothrow) pragma_GCC_error );
	append_subpragma( *new(std::nothrow) pragma_GCC_warning );

	append_known_subpragma( "dependency" );
	append_known_subpragma( "diagnostic" );
	append_known_subpragma( "poison" );
	append_known_subpragma( "system_header" );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_GCC.cxx*/
