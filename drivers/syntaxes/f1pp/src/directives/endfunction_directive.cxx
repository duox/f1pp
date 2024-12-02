/*endfunction_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endfunction_directive::endfunction_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endfunction_directive::get_name() const
{
	return "endfunction";
}
// {F9EC4CC2-BF4D-4ABA-A28F-0302097E02D8}
const f1::guid_t f1::f1pp::endfunction_directive::m_guid =
{ 0xf9ec4cc2, 0xbf4d, 0x4aba, { 0xa2, 0x8f, 0x03, 0x02, 0x09, 0x7e, 0x02, 0xd8 } };
const f1::guid_t & f1::f1pp::endfunction_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::endfunction_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::endfunction_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> cpp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == cpp_par )
		return STATUS_ERROR_INVALID_TYPE;

	function_directive::frame * frm = type_cast<function_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endfunction" );
	if( frm->get_closing_directive_guid() != endfunction_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endfunction" );

#if __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__
	cpp_par->end_subsection();
#endif // __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__

	frm->check_different_sources();	// warn if sources differ

	// Create macro
	if( nullptr == frm->m_existing_func )
	{
		// Get correct namespace
		core::Tref<ns::name_space> previous_ns = parser.get_ns_context().get_current_namespace();
		if( frm->m_def_flags & macro_directive::frame::fl_def_global )
			parser.get_ns_context().set_current_namespace( &parser.get_ns_context().get_root_namespace() );
		else if( frm->m_def_flags & macro_directive::frame::fl_def_public )
			parser.get_ns_context().move_to_parent_namespace();
#if 1
		// Create F1PP macro
		core::Tref<f1pp::macro> f1pp_func;
		status = cpp_par->create_user_macro<f1pp::macro>( f1pp_func,
			frm->m_ident, nullptr, frm->m_param_list, &frm->m_storage.get_string(),
			frm->m_macro_flags | pp::Imacro::macro_flags_t::fl_keep_new_lines | pp::Imacro::macro_flags_t::fl_self_parse_arglist );
#else
		core::Tref<pp::Imacro> func;
		status = cpp_par->create_macro( func,
			frm->m_ident, nullptr, frm->m_param_list, frm->m_storage.get_string(),
			frm->m_macro_flags | pp::Imacro::macro_flags_t::fl_keep_new_lines );
		__assert( nullptr != func );

		core::Tref<f1pp::macro> f1pp_func = func.dynamic_pointer_cast<f1pp::macro>();
		__debugbreak_if( nullptr == f1pp_func )
			return STATUS_ERROR_INTERNAL;
#endif
		if( previous_ns != parser.get_ns_context().get_current_namespace() )
			parser.get_ns_context().set_current_namespace( previous_ns );

		// Set macro flags
		f1pp_func->modify_ex_flags( macro::ex_flags_t::fl_function | macro::ex_flags_t::fl_multiline | macro::ex_flags_t::fl_parenthesis );
	}

	// Close the directive
	parser.set_output( frm->m_old_output );
	parser.set_storage( frm->m_old_storage );
	cpp_par->modify_c_flags( frm->m_old_c_flags, ~frm->m_old_c_flags );
	parser.leave_directive();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endfunction_directive.cxx*/
