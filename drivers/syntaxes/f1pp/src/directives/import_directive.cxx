/*import_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::import_directive::import_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::import_directive::get_name() const
{
	return "import";
}

// {54029386-24B8-4901-B0B4-EB691B0A4A71}
const f1::guid_t f1::f1pp::import_directive::m_guid =
{ 0x54029386, 0x24b8, 0x4901, { 0xb0, 0xb4, 0xeb, 0x69, 0x1b, 0xa, 0x4a, 0x71 } };
const f1::guid_t & f1::f1pp::import_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::import_directive::get_caps( unsigned mask ) const __noexcept
{
	return super::get_caps( mask );
}

// Hooks
const status_t f1::f1pp::import_directive::include_file( f1::c_pp::parser & par, const pp::string & file_path_name, c_pp::parser::include_flags_t include_flags, c_pp::parser::include_desc ** inc_desc_ptr )
{
	f1::c_pp::parser::include_desc * desc = nullptr;
	status_t status = super::include_file( par, file_path_name, include_flags, &desc );
	if( status.failed() )
		return status;
	__debugbreak_if( nullptr == desc )
		return STATUS_ERROR_INTERNAL;

	desc->m_flags |= f1::c_pp::parser::include_flags_t::fl_once_included;

	if( nullptr != inc_desc_ptr )
		*inc_desc_ptr = desc;

	return status;
}

const status_t f1::f1pp::import_directive::on_include_not_found( f1::c_pp::parser & par, const pp::string & file_path_name, c_pp::parser::include_flags_t include_flags )
{
	__unreferenced_parameter( par );
	__unreferenced_parameter( file_path_name );
	__unreferenced_parameter( include_flags );
	return STATUS_SUCCESS;	// ignore includes that were not found
}

/*END OF import_directive.cxx*/
