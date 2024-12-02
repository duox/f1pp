/*standard.cxx*/
#include "pch.h"
#include "main.h"

f1::c_pp::cpp17::standard::standard( const core::Iidentifier * ident ):
	super( ident )
{
}

const status_t f1::c_pp::cpp17::standard::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Initialize predefined objects
	m_macro_has_include.initialize();

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// {D367D4CA-0920-46C0-B228-7C2C5198BA08}
const f1::guid_t f1::c_pp::cpp17::standard::m_guid =
{ 0xd367d4ca, 0x0920, 0x46c0, { 0xb2, 0x28, 0x7c, 0x2c, 0x51, 0x98, 0xba, 0x08 } };
const f1::guid_t & f1::c_pp::cpp17::standard::get_guid() const
{
	return m_guid;
}

const char * f1::c_pp::cpp17::standard::get_ident_string() const
{
	return "cpp17";
}
const char * f1::c_pp::cpp17::standard::get_description_string() const
{
	return "C++17";
}

static const f1::guid_t base_guids[] =
{
	f1::c_pp::c11::standard::m_guid,
	f1::c_pp::cpp14::standard::m_guid,
};
const f1::guid_t * f1::c_pp::cpp17::standard::get_base_guid_list( __out size_t & count ) const
{
	count = _countof(base_guids);
	return base_guids;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::cpp17::standard::on_attach_to_parser( pp::Iparser & par )
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
	c_pp_par->append_macro( m_macro_has_include );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::cpp17::standard::on_detach_from_parser( pp::Iparser & par )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&par);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Remove all directives

	// Remove all macros
	c_pp_par->remove_macro( m_macro_has_include );

	// Exit
	return super::on_detach_from_parser( par );
}

bool f1::c_pp::cpp17::standard::is_item_supported( const f1::guid_t & item_class_guid, const pp::string & name ) const
{
	if( pp::Imacro::m_item_class_guid == item_class_guid && name.equal( "__has_include" ) )
		return true;
	return super::is_item_supported( item_class_guid, name );
}

/*END OF standard.cxx*/
