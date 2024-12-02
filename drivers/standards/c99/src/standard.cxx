/*standard.cxx*/
#include "pch.h"
#include "main.h"

f1::c_pp::c99::standard::standard( const core::Iidentifier * ident ):
	super( ident )
{
}

const status_t f1::c_pp::c99::standard::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Initialize predefined objects
	m_define_directive.initialize();
	m_elif_directive.initialize();
	m_else_directive.initialize();
	m_endif_directive.initialize();
	m_error_directive.initialize();
	m_if_directive.initialize();
	m_ifdef_directive.initialize();
	m_ifndef_directive.initialize();
	m_include_directive.initialize();
	m_line_directive.initialize();
	m_pragma_directive.initialize();
	m_undef_directive.initialize();

	m_macro_LINE.initialize();
	m_macro_FILE.initialize();
	m_macro_Pragma.initialize();
	m_macro_DATE.initialize();
	m_macro_defined.initialize();
	m_macro_TIME.initialize();
	m_macro_VA_ARGS.initialize();

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// {995A0FD2-4A51-40B1-9D41-ABCC87564F53}
const f1::guid_t f1::c_pp::c99::standard::m_guid =
{ 0x995a0fd2, 0x4a51, 0x40b1, { 0x9d, 0x41, 0xab, 0xcc, 0x87, 0x56, 0x4f, 0x53 } };
const f1::guid_t & f1::c_pp::c99::standard::get_guid() const
{
	return m_guid;
}

const char * f1::c_pp::c99::standard::get_ident_string() const
{
	return "C99";
}
const char * f1::c_pp::c99::standard::get_description_string() const
{
	return "C99";
}

static const f1::guid_t base_guids[] =
{
	f1::c_pp::syntax::m_guid,
};
const f1::guid_t * f1::c_pp::c99::standard::get_base_guid_list( __out size_t & count ) const
{
	count = _countof(base_guids);
	return base_guids;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::c99::standard::on_attach_to_parser( pp::Iparser & par )
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
	dlist->append_directive( m_define_directive );
	dlist->append_directive( m_elif_directive );
	dlist->append_directive( m_else_directive );
	dlist->append_directive( m_endif_directive );
	dlist->append_directive( m_error_directive );
	dlist->append_directive( m_if_directive );
	dlist->append_directive( m_ifdef_directive );
	dlist->append_directive( m_ifndef_directive );
	dlist->append_directive( m_include_directive );
	dlist->append_directive( m_line_directive );
	dlist->append_directive( m_pragma_directive );
	dlist->append_directive( m_undef_directive );

	// Add all macros
	c_pp_par->append_macro( m_macro_LINE );
	c_pp_par->append_macro( m_macro_FILE );
	c_pp_par->append_macro( m_macro_Pragma );
	c_pp_par->append_macro( m_macro_DATE );
	c_pp_par->append_macro( m_macro_defined );
	c_pp_par->append_macro( m_macro_TIME );
	c_pp_par->append_macro( m_macro_VA_ARGS );

	// Append all pragmas
	c_pp_par->append_known_pragma( "STDC" );

	// Set parser parameters
	c_pp_par->set_limit_value_min( pp::limit_t::li_maximum_block_depth, 63 );
	c_pp_par->set_limit_value_min( pp::limit_t::li_maximum_identifier_length, 63 );
	c_pp_par->set_limit_value_min( pp::limit_t::li_maximum_macro_count, 4095 );
	c_pp_par->set_limit_value_min( pp::limit_t::li_maximum_macro_parameter_count, 127 );
	c_pp_par->set_limit_value_min( pp::limit_t::li_maximum_macro_argument_count, 127 );
	c_pp_par->set_limit_value_min( pp::limit_t::li_maximum_line_length, 4095 );
	c_pp_par->set_limit_value_min( pp::limit_t::li_maximum_include_depth, 15 );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c99::standard::on_detach_from_parser( pp::Iparser & par )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&par);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Remove all directives
	core::Tref<pp::directive_list> dlist = c_pp_par->get_directive_list();
	dlist->remove_directive( m_define_directive );
	dlist->remove_directive( m_elif_directive );
	dlist->remove_directive( m_else_directive );
	dlist->remove_directive( m_endif_directive );
	dlist->remove_directive( m_error_directive );
	dlist->remove_directive( m_if_directive );
	dlist->remove_directive( m_ifdef_directive );
	dlist->remove_directive( m_ifndef_directive );
	dlist->remove_directive( m_include_directive );
	dlist->remove_directive( m_line_directive );
	dlist->remove_directive( m_pragma_directive );
	dlist->remove_directive( m_undef_directive );

	// Remove all macros
	c_pp_par->remove_macro( m_macro_LINE );
	c_pp_par->remove_macro( m_macro_FILE );
	c_pp_par->remove_macro( m_macro_Pragma );
	c_pp_par->remove_macro( m_macro_DATE );
	c_pp_par->remove_macro( m_macro_defined );
	c_pp_par->remove_macro( m_macro_TIME );
	c_pp_par->remove_macro( m_macro_VA_ARGS );

	// Append all pragmas
	c_pp_par->remove_known_pragma( "STDC" );

	// Exit
	return super::on_detach_from_parser( par );
}

bool f1::c_pp::c99::standard::is_item_supported( const f1::guid_t & item_class_guid, const pp::string & name ) const
{
	if( pp::Imacro::m_item_class_guid == item_class_guid )
	{
		if( name.equal( "__LINE__" ) )
			return true;
		if( name.equal( "__FILE__" ) )
			return true;
		if( name.equal( "__DATE__" ) )
			return true;
		if( name.equal( "__TIME__" ) )
			return true;
		if( name.equal( "__VA_ARGS__" ) )
			return true;
		if( name.equal( "_Pragma" ) )
			return true;
		if( name.equal( "defined" ) )
			return true;
	}
	else if( pp::Idirective::m_item_class_guid == item_class_guid )
	{
		if( name.equal( "define" ) )
			return true;
		if( name.equal( "elif" ) )
			return true;
		if( name.equal( "else" ) )
			return true;
		if( name.equal( "endif" ) )
			return true;
		if( name.equal( "error" ) )
			return true;
		if( name.equal( "if" ) )
			return true;
		if( name.equal( "ifdef" ) )
			return true;
		if( name.equal( "ifndef" ) )
			return true;
		if( name.equal( "include" ) )
			return true;
		if( name.equal( "line" ) )
			return true;
		if( name.equal( "pragma" ) )
			return true;
		if( name.equal( "undef" ) )
			return true;
	}
	else if( c_pp::Ipragma::m_item_class_guid == item_class_guid )
	{
	}
	return super::is_item_supported( item_class_guid, name );
}

/*END OF standard.cxx*/
