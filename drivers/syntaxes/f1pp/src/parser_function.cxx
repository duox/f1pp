/*parser_function.cxx*/
#include "pch.h"
#include "main.h"

f1::f1pp::parser::multiline_patch_callback::multiline_patch_callback( f1pp::parser * parser ):
	m_parser( parser )
{
	m_locals_ns_name.initialize();
	m_locals_ns.initialize();
	m_locals_ns.append_name( m_locals_ns_name );
}
const status_t f1::f1pp::parser::multiline_patch_callback::initialize( f1pp::macro & mac )
{
	m_macro = &mac;
	return STATUS_SUCCESS;
}
void f1::f1pp::parser::multiline_patch_callback::on_patch_initiated( f1::parser::Iparser & parser, void * )
{
	__debugbreak_if( nullptr == m_macro )
		return;

	// Enter special processing mode
	m_parser->m_in_function = !!m_macro->get_ex_flags( macro::ex_flags_t::fl_function );
	if( m_parser->m_in_function )
	{
		m_parser->m_capture_save_stack.push_back( m_parser->m_capturing_line );
		m_parser->m_capturing_line = false;
		//m_parser->disable_capture();
	}

	// Activate function scope
	m_parent_ns = m_parser->get_ns_context().get_current_namespace();
	m_parent_ns->append_subname( m_locals_ns_name );
	m_parent_ns->append_subnamespace( m_locals_ns );
	m_parser->get_ns_context().set_current_namespace( &m_locals_ns );

	// Enter compound directive (function instantiation)
	m_parser->enter_directive( *( m_parser->m_in_function ?
		(pp::Iframe *) new(std::nothrow) function_frame( *m_parser ) :
		(pp::Iframe *) new(std::nothrow) macro_frame( *m_parser )
		) );

	__unreferenced_parameter( parser );
}
void f1::f1pp::parser::multiline_patch_callback::on_patch_completed( f1::parser::Iparser & parser, void * )
{
	// Leave compound directive (function instantiation)
	m_parser->leave_directive();

	// Deactivate function scope
	m_parser->get_ns_context().set_current_namespace( m_parent_ns );
	m_locals_ns.remove_all_subnames();
	m_parent_ns->remove_subnamespace( m_locals_ns );
	m_parent_ns->remove_subname( m_locals_ns_name );
	m_parent_ns = nullptr;

	// Leave special processing mode
	if( m_parser->m_in_function )
	{
		//m_parser->enable_capture();
		m_parser->m_capturing_line = m_parser->m_capture_save_stack.back();
		m_parser->m_capture_save_stack.pop_back();

		m_parser->m_in_function = false;
	}

	__unreferenced_parameter( parser );
}

/*END OF parser_function.cxx*/
