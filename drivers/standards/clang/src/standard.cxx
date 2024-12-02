/*standard.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::c_pp::clang::standard::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Initialize predefined objects
	m_pragma_clang.initialize();

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// {78907E90-62E7-4F81-B17B-B5982778CF2A}
const f1::guid_t f1::c_pp::clang::standard::m_guid =
{ 0x78907e90, 0x62e7, 0x4f81, { 0xb1, 0x7b, 0xb5, 0x98, 0x27, 0x78, 0xcf, 0x2a } };
const f1::guid_t & f1::c_pp::clang::standard::get_guid() const
{
	return m_guid;
}

const char * f1::c_pp::clang::standard::get_ident_string() const
{
	return "clang";
}
const char * f1::c_pp::clang::standard::get_description_string() const
{
	return "clang preprocessor extensions";
}

static const f1::guid_t base_guids[] =
{
	f1::c_pp::c23::standard::m_guid,
	f1::c_pp::cpp23::standard::m_guid,
	f1::c_pp::comext::standard::m_guid,
};
const f1::guid_t * f1::c_pp::clang::standard::get_base_guid_list( __out size_t & count ) const
{
	count = _countof(base_guids);
	return base_guids;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::clang::standard::on_attach_to_parser( pp::Iparser & par )
{
	status_t status;

	// Call base class
	if( super::on_attach_to_parser( par ).failed_status( status ) )
		return status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&par);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Add all directives

	// Add all macros

	// Append all pragmas
	c_pp_par->append_pragma( m_pragma_clang );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::clang::standard::on_detach_from_parser( pp::Iparser & par )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&par);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Remove all directives

	// Remove all macros

	// Append all pragmas
	c_pp_par->remove_pragma( m_pragma_clang );

	// Exit
	return super::on_detach_from_parser( par );
}

/*END OF standard.cxx*/
