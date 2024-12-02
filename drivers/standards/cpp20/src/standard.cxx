/*standard.cxx*/
#include "pch.h"
#include "main.h"

f1::c_pp::cpp20::standard::standard( const core::Iidentifier * ident ):
	super( ident )
{
}

const status_t f1::c_pp::cpp20::standard::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Initialize predefined objects
	m_macro_VA_OPT.initialize();

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// {71DC065E-30FB-4EB3-9FE6-9BF9A61DF126}
const f1::guid_t f1::c_pp::cpp20::standard::m_guid =
{ 0x71dc065e, 0x30fb, 0x4eb3, { 0x9f, 0xe6, 0x9b, 0xf9, 0xa6, 0x1d, 0xf1, 0x26 } };
const f1::guid_t & f1::c_pp::cpp20::standard::get_guid() const
{
	return m_guid;
}

const char * f1::c_pp::cpp20::standard::get_ident_string() const
{
	return "cpp20";
}
const char * f1::c_pp::cpp20::standard::get_description_string() const
{
	return "C++20";
}

static const f1::guid_t base_guids[] =
{
	f1::c_pp::c11::standard::m_guid,
	f1::c_pp::cpp17::standard::m_guid,
};
const f1::guid_t * f1::c_pp::cpp20::standard::get_base_guid_list( __out size_t & count ) const
{
	count = _countof(base_guids);
	return base_guids;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::cpp20::standard::on_attach_to_parser( pp::Iparser & par )
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
	c_pp_par->append_macro( m_macro_VA_OPT );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::cpp20::standard::on_detach_from_parser( pp::Iparser & par )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&par);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Remove all directives

	// Remove all macros
	c_pp_par->remove_macro( m_macro_VA_OPT );

	// Exit
	return super::on_detach_from_parser( par );
}

bool f1::c_pp::cpp20::standard::is_item_supported( const f1::guid_t & item_class_guid, const pp::string & name ) const
{
	if( pp::Imacro::m_item_class_guid == item_class_guid && name.equal( "__VA_OPT__" ) )
		return true;
	return super::is_item_supported( item_class_guid, name );
}

/*END OF standard.cxx*/
