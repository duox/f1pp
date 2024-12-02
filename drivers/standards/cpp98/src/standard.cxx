/*standard.cxx*/
#include "pch.h"
#include "main.h"

f1::c_pp::cpp98::standard::standard( const core::Iidentifier * ident ):
	super( ident )
{
}

const status_t f1::c_pp::cpp98::standard::initialize()
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

// {8903A182-E3C8-44FC-9ABB-64AA1CE0333B}
const f1::guid_t f1::c_pp::cpp98::standard::m_guid =
{ 0x8903a182, 0xe3c8, 0x44fc, { 0x9a, 0xbb, 0x64, 0xaa, 0x1c, 0xe0, 0x33, 0x3b } };
const f1::guid_t & f1::c_pp::cpp98::standard::get_guid() const
{
	return m_guid;
}

const char * f1::c_pp::cpp98::standard::get_ident_string() const
{
	return "cpp98";
}
const char * f1::c_pp::cpp98::standard::get_description_string() const
{
	return "C++98";
}

static const f1::guid_t base_guids[] =
{
	f1::c_pp::c99::standard::m_guid
};
const f1::guid_t * f1::c_pp::cpp98::standard::get_base_guid_list( __out size_t & count ) const
{
	count = _countof(base_guids);
	return base_guids;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::cpp98::standard::on_attach_to_parser( pp::Iparser & par )
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
const status_t f1::c_pp::cpp98::standard::on_detach_from_parser( pp::Iparser & par )
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
