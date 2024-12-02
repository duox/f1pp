/*Iscoped_frame.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::Iscoped_frame::Iscoped_frame( f1pp::parser & par ):
	super( par )
{
	m_locals_ns.initialize();
	m_own_ns.initialize();
}

const status_t f1::f1pp::Iscoped_frame::enter()
{
	// Activate local scope
	m_parent_ns = m_parser->get_ns_context().get_current_namespace();
	m_own_ns.append_name( *m_parent_ns, "own" );
	m_locals_ns.append_name( m_own_ns, "locals" );

	m_parser->get_ns_context().set_current_namespace( &m_locals_ns );

//	OutputDebugString( "enter\n" );

	// Exit
	return super::enter();
}
const status_t f1::f1pp::Iscoped_frame::leave()
{
	// Deactivate local scope
	if( nullptr != m_parent_ns )
	{
		m_parser->get_ns_context().set_current_namespace( m_parent_ns );
		m_parent_ns->remove_subnamespace( m_own_ns );
		m_parent_ns = nullptr;
	}
//	OutputDebugString( "leave\n" );

	// Exit
	return super::leave();
}

const status_t f1::f1pp::Iscoped_frame::undefine_locals()
{
	m_locals_ns.remove_all_subnames();
//	array_foreach( f1::ns::name_list_t::const_iterator, it, m_locals_ns.get_subname_list() )
//	{
//	}
	return STATUS_SUCCESS;
}

/*END OF Iscoped_frame.cxx*/
