/*standard.cxx*/
#include "pch.h"
#include "main.h"

f1::c_pp::c23::standard::standard( const core::Iidentifier * ident ):
	super( ident )
{
}

const status_t f1::c_pp::c23::standard::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Initialize predefined objects
	m_embed_directive.initialize();
	m_warning_directive.initialize();
	m_elifdef_directive.initialize();
	m_elifndef_directive.initialize();

	m_macro_has_embed.initialize();
	m_macro_STDC_EMBED_EMPTY.initialize();
	m_macro_STDC_EMBED_FOUND.initialize();
	m_macro_STDC_EMBED_NOT_FOUND.initialize();

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// {DCDECD9E-4441-4B8C-B5E2-BB8335CF0AF1}
const f1::guid_t f1::c_pp::c23::standard::m_guid =
{ 0xdcdecd9e, 0x4441, 0x4b8c, { 0xb5, 0xe2, 0xbb, 0x83, 0x35, 0xcf, 0x0a, 0xf1 } };
const f1::guid_t & f1::c_pp::c23::standard::get_guid() const
{
	return m_guid;
}

const char * f1::c_pp::c23::standard::get_ident_string() const
{
	return "c23";
}
const char * f1::c_pp::c23::standard::get_description_string() const
{
	return "C23";
}

static const f1::guid_t base_guids[] =
{
	f1::c_pp::c11::standard::m_guid,
	f1::c_pp::cpp23::standard::m_guid,
};
const f1::guid_t * f1::c_pp::c23::standard::get_base_guid_list( __out size_t & count ) const
{
	count = _countof(base_guids);
	return base_guids;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::c23::standard::on_attach_to_parser( pp::Iparser & par )
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
	dlist->append_directive( m_embed_directive );

	// Add all macros
	c_pp_par->append_macro( m_macro_has_embed );
	c_pp_par->append_macro( m_macro_STDC_EMBED_EMPTY );
	c_pp_par->append_macro( m_macro_STDC_EMBED_FOUND );
	c_pp_par->append_macro( m_macro_STDC_EMBED_NOT_FOUND );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c23::standard::on_detach_from_parser( pp::Iparser & par )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&par);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Remove all directives
	core::Tref<pp::directive_list> dlist = c_pp_par->get_directive_list();
	dlist->remove_directive( m_embed_directive );

	// Remove all macros
	c_pp_par->remove_macro( m_macro_has_embed );
	c_pp_par->remove_macro( m_macro_STDC_EMBED_EMPTY );
	c_pp_par->remove_macro( m_macro_STDC_EMBED_FOUND );
	c_pp_par->remove_macro( m_macro_STDC_EMBED_NOT_FOUND );

	// Exit
	return super::on_detach_from_parser( par );
}

/*END OF standard.cxx*/
