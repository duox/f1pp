/*standard.cxx*/
#include "pch.h"
#include "main.h"

f1::c_pp::cpp11::standard::standard( const core::Iidentifier * ident ):
	super( ident )
{
}

const status_t f1::c_pp::cpp11::standard::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Initialize predefined objects

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// {3A19D847-947C-4800-A7A5-5378CD75B1AD}
const f1::guid_t f1::c_pp::cpp11::standard::m_guid =
{ 0x3a19d847, 0x947c, 0x4800, { 0xa7, 0xa5, 0x53, 0x78, 0xcd, 0x75, 0xb1, 0xad } };
const f1::guid_t & f1::c_pp::cpp11::standard::get_guid() const
{
	return m_guid;
}

const char * f1::c_pp::cpp11::standard::get_ident_string() const
{
	return "cpp11";
}
const char * f1::c_pp::cpp11::standard::get_description_string() const
{
	return "C++11";
}

static const f1::guid_t base_guids[] =
{
	f1::c_pp::c99::standard::m_guid,
	f1::c_pp::cpp98::standard::m_guid,
};
const f1::guid_t * f1::c_pp::cpp11::standard::get_base_guid_list( __out size_t & count ) const
{
	count = _countof(base_guids);
	return base_guids;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::cpp11::standard::on_attach_to_parser( pp::Iparser & par )
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

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::cpp11::standard::on_detach_from_parser( pp::Iparser & par )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&par);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Remove all directives

	// Remove all macros

	// Exit
	return super::on_detach_from_parser( par );
}

/*END OF standard.cxx*/
