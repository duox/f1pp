/*standard.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::c_pp::gcc::standard::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Initialize predefined objects
	m_macro_BASE_FILE.initialize();
	m_macro_FILE_NAME.initialize();
	m_macro_INCLUDE_LEVEL.initialize();

	m_pragma_GCC.initialize();

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// {F6CE61A2-727F-4740-AD48-8E0FFC4B2D2C}
const f1::guid_t f1::c_pp::gcc::standard::m_guid =
{ 0xf6ce61a2, 0x727f, 0x4740, { 0xad, 0x48, 0x8e, 0x0f, 0xfc, 0x4b, 0x2d, 0x2c } };
const f1::guid_t & f1::c_pp::gcc::standard::get_guid() const
{
	return m_guid;
}

const char * f1::c_pp::gcc::standard::get_ident_string() const
{
	return "gcc";
}
const char * f1::c_pp::gcc::standard::get_description_string() const
{
	return "GCC preprocessor extensions";
}

static const f1::guid_t base_guids[] =
{
	f1::c_pp::c23::standard::m_guid,
	f1::c_pp::cpp23::standard::m_guid,
	f1::c_pp::comext::standard::m_guid,
};
const f1::guid_t * f1::c_pp::gcc::standard::get_base_guid_list( __out size_t & count ) const
{
	count = _countof(base_guids);
	return base_guids;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::gcc::standard::on_attach_to_parser( pp::Iparser & par )
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
	c_pp_par->append_macro( m_macro_BASE_FILE );
	c_pp_par->append_macro( m_macro_FILE_NAME );
	c_pp_par->append_macro( m_macro_INCLUDE_LEVEL );

	// Append all pragmas
	c_pp_par->append_pragma( m_pragma_GCC );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::gcc::standard::on_detach_from_parser( pp::Iparser & par )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&par);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Remove all directives

	// Remove all macros
	c_pp_par->remove_macro( m_macro_BASE_FILE );
	c_pp_par->remove_macro( m_macro_FILE_NAME );
	c_pp_par->remove_macro( m_macro_INCLUDE_LEVEL );

	// Remove all pragmas
	c_pp_par->remove_pragma( m_pragma_GCC );

	// Exit
	return super::on_detach_from_parser( par );
}

/*END OF standard.cxx*/
