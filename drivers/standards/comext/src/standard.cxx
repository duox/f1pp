/*standard.cxx*/
#include "pch.h"
#include "main.h"

f1::c_pp::comext::standard::standard( const core::Iidentifier * ident ):
	super( ident )
{
}

const status_t f1::c_pp::comext::standard::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Initialize predefined objects
	m_file_directive.initialize();
	m_ident_directive.initialize();
	m_include_next_directive.initialize();
	m_includepath_directive.initialize();
	m_sccs_directive.initialize();
	m_info_directive.initialize();
	m_warning_directive.initialize();

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

// {60980D1F-0EB0-46BB-ACF8-244965035352}
const f1::guid_t f1::c_pp::comext::standard::m_guid =
{ 0x60980d1f, 0x0eb0, 0x46bb, { 0xac, 0xf8, 0x24, 0x49, 0x65, 0x03, 0x53, 0x52 } };
const f1::guid_t & f1::c_pp::comext::standard::get_guid() const
{
	return m_guid;
}

const char * f1::c_pp::comext::standard::get_ident_string() const
{
	return "comext";
}
const char * f1::c_pp::comext::standard::get_description_string() const
{
	return "Common extensions";
}

static const f1::guid_t base_guids[] =
{
	f1::c_pp::c23::standard::m_guid,
	f1::c_pp::cpp23::standard::m_guid
};
const f1::guid_t * f1::c_pp::comext::standard::get_base_guid_list( __out size_t & count ) const
{
	count = _countof(base_guids);
	return base_guids;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::comext::standard::on_attach_to_parser( pp::Iparser & par )
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
	dlist->append_directive( m_file_directive );
	dlist->append_directive( m_ident_directive );
	dlist->append_directive( m_include_next_directive );
	dlist->append_directive( m_includepath_directive );
	dlist->append_directive( m_info_directive );
	dlist->append_directive( m_sccs_directive );
	dlist->append_directive( m_warning_directive );

	// Add all macros
	c_pp_par->append_macro( m_macro_COUNTER );
	c_pp_par->append_macro( m_macro_TIMESTAMP );

	// Append all pragmas
	c_pp_par->append_pragma( m_pragma_include_alias );
	c_pp_par->append_pragma( m_pragma_message );
	c_pp_par->append_pragma( m_pragma_once );
	c_pp_par->append_pragma( m_pragma_pop_macro );
	c_pp_par->append_pragma( m_pragma_push_macro );

	c_pp_par->append_known_pragma( "comment" );
	c_pp_par->append_known_pragma( "ident" );
	c_pp_par->append_known_pragma( "omp" );
	c_pp_par->append_known_pragma( "OPENCL" );
	c_pp_par->append_known_pragma( "pack" );
	c_pp_par->append_known_pragma( "weak" );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::comext::standard::on_detach_from_parser( pp::Iparser & par )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&par);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Remove all directives
	core::Tref<pp::directive_list> dlist = c_pp_par->get_directive_list();
	dlist->remove_directive( m_file_directive );
	dlist->remove_directive( m_ident_directive );
	dlist->remove_directive( m_include_next_directive );
	dlist->remove_directive( m_includepath_directive );
	dlist->remove_directive( m_info_directive );
	dlist->remove_directive( m_sccs_directive );
	dlist->remove_directive( m_warning_directive );

	// Remove all macros
	c_pp_par->remove_macro( m_macro_COUNTER );
	c_pp_par->remove_macro( m_macro_TIMESTAMP );

	// Append all pragmas
	c_pp_par->remove_pragma( m_pragma_include_alias );
	c_pp_par->remove_pragma( m_pragma_message );
	c_pp_par->remove_pragma( m_pragma_once );
	c_pp_par->remove_pragma( m_pragma_pop_macro );
	c_pp_par->remove_pragma( m_pragma_push_macro );

	c_pp_par->remove_known_pragma( "comment" );
	c_pp_par->remove_known_pragma( "ident" );
	c_pp_par->remove_known_pragma( "pack" );
	c_pp_par->remove_known_pragma( "weak" );

	// Exit
	return super::on_detach_from_parser( par );
}

bool f1::c_pp::comext::standard::is_item_supported( const f1::guid_t & item_class_guid, const pp::string & name ) const
{
	if( pp::Imacro::m_item_class_guid == item_class_guid )
	{
		if( name.equal( "__COUNTER__" ) )
			return true;
		if( name.equal( "__TIMESTAMP__" ) )
			return true;
	}
	else if( pp::Idirective::m_item_class_guid == item_class_guid )
	{
		if( name.equal( "include_next" ) )
			return true;
		if( name.equal( "includepath" ) )
			return true;
		if( name.equal( "info" ) )
			return true;
		if( name.equal( "warning" ) )
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
