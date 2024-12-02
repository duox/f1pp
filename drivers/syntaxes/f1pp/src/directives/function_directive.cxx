/*function_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::function_directive::function_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::function_directive::get_name() const
{
	return "function";
}

// {65E3F38E-347F-4ca6-81E0-615F8DC8F679}
const f1::guid_t f1::f1pp::function_directive::m_guid =
{ 0x65e3f38e, 0x347f, 0x4ca6, { 0x81, 0xe0, 0x61, 0x5f, 0x8d, 0xc8, 0xf6, 0x79 } };
const f1::guid_t & f1::f1pp::function_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::function_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

// Operations
const status_t f1::f1pp::function_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	f1pp_par->skip_real_spaces_and_comments();

	// Parse function name
	pp::string ident;
	if( f1pp_par->parse_macro_identifier( ident ).failed_status( status ) )
		return status;
	if( ident.empty() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		return STATUS_SUCCESS;
	}
	f1pp_par->skip_real_spaces_and_comments();

	// Parse parameter list
	core::Tconst_ref<pp::param_list> params = nullptr;
	if( !par->peek_char( '(' ) )
	{
		parser.message( syntax::IMSG_WARNING_EXPECTED_PARAMETER_LIST );

		core::Tref<pp::param_list> pars = nullptr;
		pars.createT<pp::param_list>();
		pars->initialize();
		params = pars;
	}
	else
	{
		if( f1pp_par->parse_param_list( params ).failed_status( status ) )
			return status;
	}

	// Create macro function
	core::Tconst_ref<pp::Imacro>	existing_func;
	parser.find_macro( existing_func, ident, pp::Iparser::find_macro_flags_t::fl_macro_search_current_namespace_only );
	if( nullptr != existing_func )
	{
		if( 0 == existing_func->get_flags( pp::Imacro::macro_flags_t::fl_weak ) )
		{
			status = parser.message( c_pp::syntax::IMSG_ERROR_MACRO_ALREADY_DEFINED_S, ident.c_str() );
			f1pp_par->display_macro_location( *existing_func, syntax::IMSG_INFO_DEFINED_HERE );
			return status;
		}

		core::Tref<f1::ns::name> nm;
		parser.get_ns_context().get_current_namespace()->find_subname( nm, ident.data(), ident.length() );
		if( nullptr != nm )
		{
			__assert( nullptr != existing_func );
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

	// Start nested operator
	frame * frm = new(std::nothrow) frame( *f1pp_par );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;
	frm->m_ident.move( ident );
	frm->m_param_list = params;
	frm->m_existing_func = existing_func;
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
f1::pp::Iframe * f1::f1pp::function_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return nullptr;
	return new(std::nothrow) frame( *f1pp_par );
}

f1::f1pp::function_directive::frame::frame( f1pp::parser & par ):
	super( par )
{
	m_output.initialize();
	m_storage.initialize( *par.get_context() );
}
const char * f1::f1pp::function_directive::frame::get_openning_directive_name() const
{
	return "function";
}
const char * f1::f1pp::function_directive::frame::get_closing_directive_name() const
{
	return "endfunction";
}
const f1::guid_t & f1::f1pp::function_directive::frame::get_openning_directive_guid() const
{
	return function_directive::m_guid;
}
const f1::guid_t & f1::f1pp::function_directive::frame::get_closing_directive_guid() const
{
	return endfunction_directive::m_guid;
}

/*END OF function_directive.cxx*/
