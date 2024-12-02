/*tryinclude_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::tryinclude_directive::tryinclude_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::tryinclude_directive::get_name() const
{
	return "tryinclude";
}

// {89EA6FD3-23EE-4960-B826-FD3DD4E9CE02}
const f1::guid_t f1::f1pp::tryinclude_directive::m_guid =
{ 0x89ea6fd3, 0x23ee, 0x4960, { 0xb8, 0x26, 0xfd, 0x3d, 0xd4, 0xe9, 0xce, 0x2 } };
const f1::guid_t & f1::f1pp::tryinclude_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::tryinclude_directive::get_caps( unsigned mask ) const __noexcept
{
	return super::get_caps( mask );
}

// Hooks
const status_t f1::f1pp::tryinclude_directive::on_include_not_found( f1::c_pp::parser & par, const pp::string & file_path_name, c_pp::parser::include_flags_t include_flags )
{
	__unreferenced_parameter( par );
	__unreferenced_parameter( file_path_name );
	__unreferenced_parameter( include_flags );
	return STATUS_SUCCESS;	// ignore includes that were not found
}

/*END OF tryinclude_directive.cxx*/
