/*standard.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::c_pp::msvc::standard::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Initialize predefined objects
	m_macro_pragma.initialize();
	m_macro_COUNTER.initialize();
	m_macro_TIMESTAMP.initialize();

	m_pragma_include_alias.initialize();
	m_pragma_message.initialize();
	m_pragma_once.initialize();
	m_pragma_pop_macro.initialize();
	m_pragma_push_macro.initialize();

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// {7A0DFE9A-0388-4989-B2CF-57FEE145F981}
const f1::guid_t f1::c_pp::msvc::standard::m_guid =
{ 0x7a0dfe9a, 0x0388, 0x4989, { 0xb2, 0xcf, 0x57, 0xfe, 0xe1, 0x45, 0xf9, 0x81 } };
const f1::guid_t & f1::c_pp::msvc::standard::get_guid() const
{
	return m_guid;
}

const char * f1::c_pp::msvc::standard::get_ident_string() const
{
	return "msvc";
}
const char * f1::c_pp::msvc::standard::get_description_string() const
{
	return "MSVC";
}

static const f1::guid_t base_guids[] =
{
	f1::c_pp::c23::standard::m_guid,
	f1::c_pp::cpp23::standard::m_guid,
	f1::c_pp::comext::standard::m_guid,
};
const f1::guid_t * f1::c_pp::msvc::standard::get_base_guid_list( __out size_t & count ) const
{
	count = _countof(base_guids);
	return base_guids;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::msvc::standard::on_attach_to_parser( pp::Iparser & par )
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
	c_pp_par->append_macro( m_macro_pragma );
	c_pp_par->append_macro( m_macro_COUNTER );
	c_pp_par->append_macro( m_macro_TIMESTAMP );

	// Append all pragmas
	c_pp_par->append_pragma( m_pragma_include_alias );
	c_pp_par->append_pragma( m_pragma_message );
	c_pp_par->append_pragma( m_pragma_once );
	c_pp_par->append_pragma( m_pragma_pop_macro );
	c_pp_par->append_pragma( m_pragma_push_macro );

	c_pp_par->append_known_pragma( "endregion" );
	c_pp_par->append_known_pragma( "region" );
	c_pp_par->append_known_pragma( "stdc" );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::msvc::standard::on_detach_from_parser( pp::Iparser & par )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&par);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Remove all directives

	// Remove all macros
	c_pp_par->remove_macro( m_macro_pragma );
	c_pp_par->remove_macro( m_macro_COUNTER );
	c_pp_par->remove_macro( m_macro_TIMESTAMP );

	// Remove all pragmas
	c_pp_par->remove_pragma( m_pragma_include_alias );
	c_pp_par->remove_pragma( m_pragma_message );
	c_pp_par->remove_pragma( m_pragma_once );
	c_pp_par->remove_pragma( m_pragma_pop_macro );
	c_pp_par->remove_pragma( m_pragma_push_macro );

	c_pp_par->append_known_pragma( "stdc" );

	// Exit
	return super::on_detach_from_parser( par );
}

bool f1::c_pp::msvc::standard::is_item_supported( const f1::guid_t & item_class_guid, const pp::string & name ) const
{
	if( pp::Imacro::m_item_class_guid == item_class_guid )
	{
		if( name.equal( "__pragma" ) )
			return true;
		if( name.equal( "__COUNTER__" ) )
			return true;
		if( name.equal( "__TIMESTAMP__" ) )
			return true;
	}
	else if( c_pp::Ipragma::m_item_class_guid == item_class_guid )
	{
		if( name.equal( "include_alias" ) )
			return true;
		if( name.equal( "message" ) )
			return true;
		if( name.equal( "once" ) )
			return true;
		if( name.equal( "pop_macro" ) )
			return true;
		if( name.equal( "push_macro" ) )
			return true;
	}
	return super::is_item_supported( item_class_guid, name );
}

/*END OF standard.cxx*/
