/*standard.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::c_pp::cpp23::standard::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Initialize predefined objects
	m_elifdef_directive.initialize();
	m_elifndef_directive.initialize();

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// {066F9772-482D-4B89-9018-565F56D7D3F6}
const f1::guid_t f1::c_pp::cpp23::standard::m_guid =
{ 0x066f9772, 0x482d, 0x4b89, { 0x90, 0x18, 0x56, 0x5f, 0x56, 0xd7, 0xd3, 0xf6 } };
const f1::guid_t & f1::c_pp::cpp23::standard::get_guid() const
{
	return m_guid;
}

const char * f1::c_pp::cpp23::standard::get_ident_string() const
{
	return "cpp23";
}
const char * f1::c_pp::cpp23::standard::get_description_string() const
{
	return "C++23";
}

static const f1::guid_t base_guids[] =
{
	f1::c_pp::c11::standard::m_guid,
	f1::c_pp::cpp20::standard::m_guid,
};
const f1::guid_t * f1::c_pp::cpp23::standard::get_base_guid_list( __out size_t & count ) const
{
	count = _countof(base_guids);
	return base_guids;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::cpp23::standard::on_attach_to_parser( pp::Iparser & par )
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
	core::Tref<pp::directive_list> dlist = c_pp_par->get_directive_list();
	dlist->append_directive( m_elifdef_directive );
	dlist->append_directive( m_elifndef_directive );

	// Add all macros

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::cpp23::standard::on_detach_from_parser( pp::Iparser & par )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&par);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Remove all directives
	core::Tref<pp::directive_list> dlist = c_pp_par->get_directive_list();
	dlist->remove_directive( m_elifdef_directive );
	dlist->remove_directive( m_elifndef_directive );

	// Remove all macros

	// Exit
	return super::on_detach_from_parser( par );
}

bool f1::c_pp::cpp23::standard::is_item_supported( const f1::guid_t & item_class_guid, const pp::string & name ) const
{
	if( pp::Idirective::m_item_class_guid == item_class_guid )
	{
		if( name.equal( "elifdef" ) )
			return true;
		if( name.equal( "elifndef" ) )
			return true;
	}
	return super::is_item_supported( item_class_guid, name );
}

/*END OF standard.cxx*/
