/*macro_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::macro_directive::macro_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::macro_directive::get_name() const
{
	return "macro";
}

// {344C93E5-3390-4A20-BA9C-C1AEB3C49832}
const f1::guid_t f1::f1pp::macro_directive::m_guid =
{ 0x344c93e5, 0x3390, 0x4a20, { 0xba, 0x9c, 0xc1, 0xae, 0xb3, 0xc4, 0x98, 0x32 } };
const f1::guid_t & f1::f1pp::macro_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::macro_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_structured) & mask;
}

// Operations
const status_t f1::f1pp::macro_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Parse macro name
	f1pp_par->skip_real_spaces_and_comments();

	pp::string ident;
	f1pp_par->parse_macro_identifier( ident );
	if( ident.is_empty() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		return STATUS_SUCCESS;
	}

	// Parse macro parameters
	f1pp_par->skip_real_spaces_and_comments();

	const bool parenthesis = par->skip_char( '(' );
	core::Tref<pp::param_list> params = nullptr;
	if( !par->is_nl() )
		f1pp_par->parse_param_list( params, parenthesis ? ')' : '\n' );

	// Check whether a macro with such signature already exists
	core::Tref<pp::Imacro> existing_macro;
	nullptr == params ?
		f1pp_par->find_macro( existing_macro, ident,			pp::Iparser::find_macro_flags_t::fl_macro_search_current_namespace_only ):
		f1pp_par->find_macro( existing_macro, ident, *params,	pp::Iparser::find_macro_flags_t::fl_macro_search_current_namespace_only );
	if( nullptr != existing_macro )
	{
		if( 0 == existing_macro->get_flags( pp::Imacro::macro_flags_t::fl_weak ) )
		{
			status = parser.message( c_pp::syntax::IMSG_ERROR_MACRO_ALREADY_DEFINED_S, ident.c_str() );
			f1pp_par->display_macro_location( *existing_macro, syntax::IMSG_INFO_DEFINED_HERE );
			return status;
		}

		core::Tref<f1::ns::name> nm;
		parser.get_ns_context().get_current_namespace()->find_subname( nm, ident.data(), ident.length() );
		if( nullptr != nm )
		{
			__assert( nullptr != existing_macro );
			parser.get_ns_context().get_current_namespace()->remove_subname( *nm );
		}
	}

	// Gather macro attributes
	pp::Imacro::macro_flags_t macro_flags = null;
	if( f1pp_par->exists_directive_attribute( "deprecated" ) )
		macro_flags |= pp::Imacro::macro_flags_t::fl_deprecated;
	if( f1pp_par->exists_directive_attribute( "disabled" ) )
		macro_flags |= pp::Imacro::macro_flags_t::fl_disabled;
	if( f1pp_par->exists_directive_attribute( "protected" ) )
		macro_flags |= pp::Imacro::macro_flags_t::fl_immutable;
	if( f1pp_par->exists_directive_attribute( "weak" ) )
		macro_flags |= pp::Imacro::macro_flags_t::fl_weak;

	unsigned def_flags = 0;
	if( f1pp_par->exists_directive_attribute( "global" ) )
		def_flags |= frame::fl_def_global;
	if( f1pp_par->exists_directive_attribute( "local" ) )
		def_flags |= frame::fl_def_local;
	if( f1pp_par->exists_directive_attribute( "public" ) )
		def_flags |= frame::fl_def_public;

	// Fetch expression string and expand macros
	frame * frm = new(std::nothrow) frame( *f1pp_par );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;
	frm->m_ident.move( ident );
	frm->m_param_list = params;
	frm->m_parenthesis = parenthesis;
	frm->m_existing_macro = existing_macro;
	frm->m_macro_flags = macro_flags;
	frm->m_def_flags = def_flags;

#if __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__
	f1pp_par->begin_subsection( *frm, frm->m_ident );
#endif // __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__

	f1pp_par->modify_c_flags(
		f1::c_pp::parser::c_flags_t::fl_keep_comments |
		f1::c_pp::parser::c_flags_t::fl_execute_exec_top_only |
		f1::c_pp::parser::c_flags_t::fl_do_not_expand_macros,
		null,
		&frm->m_old_c_flags
		);
	parser.set_output( &frm->m_output, &frm->m_old_output );
	parser.set_storage( &frm->m_storage, &frm->m_old_storage );
	parser.enter_directive( *frm );

	// Exit
	return STATUS_SUCCESS;
}
f1::pp::Iframe * f1::f1pp::macro_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return nullptr;
	return new(std::nothrow) frame( *f1pp_par );
}

f1::f1pp::macro_directive::frame::frame( f1pp::parser & par ):
	super( par ),
	m_parenthesis( false )
{
	m_output.initialize();
	m_storage.initialize( *par.get_context() );
}
const char * f1::f1pp::macro_directive::frame::get_openning_directive_name() const
{
	return "macro";
}
const char * f1::f1pp::macro_directive::frame::get_closing_directive_name() const
{
	return "endmacro";
}
const f1::guid_t & f1::f1pp::macro_directive::frame::get_openning_directive_guid() const
{
	return macro_directive::m_guid;
}
const f1::guid_t & f1::f1pp::macro_directive::frame::get_closing_directive_guid() const
{
	return endmacro_directive::m_guid;
}

/*END OF macro_directive.cxx*/
