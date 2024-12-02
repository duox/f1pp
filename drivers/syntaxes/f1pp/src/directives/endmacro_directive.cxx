/*endmacro_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endmacro_directive::endmacro_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endmacro_directive::get_name() const
{
	return "endmacro";
}
// {82B621D4-4B4C-45BD-AE83-B13F4E12B681}
const f1::guid_t f1::f1pp::endmacro_directive::m_guid =
{ 0x82b621d4, 0x4b4c, 0x45bd, { 0xae, 0x83, 0xb1, 0x3f, 0x4e, 0x12, 0xb6, 0x81 } };
const f1::guid_t & f1::f1pp::endmacro_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::endmacro_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::endmacro_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	macro_directive::frame * frm = type_cast<macro_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endmacro" );
	if( frm->get_closing_directive_guid() != endmacro_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endmacro" );

	ns::context & nsc = parser.get_ns_context();

#if __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__
	c_pp_par->end_subsection();
#endif // __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__

	frm->check_different_sources();	// warn if sources differ

	// Create macro
	if( nullptr == frm->m_existing_macro )
	{
		// Remove leading and trailing spaces (but not new lines, otherwise last macro in the body will not be expanded)
		pp::string value( frm->m_storage.get_string() );
		c_pp_par->trim_spaces_and_comments( value, true );
		if( value.size() > 0 )
			value.add_part( "\x20" );

		// Get correct namespace
		core::Tref<ns::name_space> previous_ns = nsc.get_current_namespace();
		if( frm->m_def_flags & macro_directive::frame::fl_def_global )
			nsc.set_current_namespace( &nsc.get_root_namespace() );
		else if( frm->m_def_flags & macro_directive::frame::fl_def_public )
			nsc.move_to_parent_namespace();

		// Create macro object
		core::Tref<pp::Imacro> mac;
		status = c_pp_par->create_macro( mac,
			frm->m_ident, nullptr, frm->m_param_list, value,
			frm->m_macro_flags | pp::Imacro::macro_flags_t::fl_keep_new_lines | pp::Imacro::macro_flags_t::fl_self_parse_arglist );
		__assert( nullptr != mac );

		if( previous_ns != nsc.get_current_namespace() )
			nsc.set_current_namespace( previous_ns );

		mac->set_definition_location(
			*frm->get_initial_location().m_source,
			 frm->get_initial_location().m_line );

		// Set macro flags
		core::Tref<f1pp::macro> f1pp_mac = mac.dynamic_pointer_cast<f1pp::macro>();
		__debugbreak_if( nullptr == f1pp_mac )
			return STATUS_ERROR_INTERNAL;
		f1pp_mac->modify_ex_flags( macro::ex_flags_t::fl_parenthesis, frm->m_parenthesis );
		f1pp_mac->modify_ex_flags( macro::ex_flags_t::fl_multiline );

		if( frm->m_def_flags & macro_directive::frame::fl_def_global )
			f1pp_mac->modify_ex_flags( macro::ex_flags_t::fl_global );
		else if( frm->m_def_flags & macro_directive::frame::fl_def_public )
			f1pp_mac->modify_ex_flags( macro::ex_flags_t::fl_public );
	}

	// Close the directive
	c_pp_par->set_output( frm->m_old_output );
	c_pp_par->set_storage( frm->m_old_storage );
	c_pp_par->modify_c_flags( frm->m_old_c_flags, ~frm->m_old_c_flags );
	parser.leave_directive();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endmacro_directive.cxx*/
