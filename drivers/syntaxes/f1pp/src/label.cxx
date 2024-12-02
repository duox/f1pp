/*label.cxx*/
#include "pch.h"
#include "main.h"

f1::f1pp::label::label( const ns::identifier * ident ):
	super( ident )
{
}

const status_t f1::f1pp::label::initialize( pp::Iparser & par, const pp::string	& name )
{
	status_t status;

	// Call base class
	if( super::initialize( *par.get_ns_context().get_current_namespace(), name.data(), name.length() ).failed_status( status ) )
		return status;

	// Store parameters
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&par);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	par.get_parser()->get_pos( m_position );
	m_name = name;

	core::Tref<goto_directive> drct;
	f1pp_par->find_directive( drct, goto_directive::m_guid );
	m_goto_directive = drct;

	// Capture current execution state
	m_frame_stack = par.get_frame_stack();
	array_foreach( core::Tarray<pp::Iframe *>::iterator, it, m_frame_stack )
		(*it)->add_ref();

	// Check for delayed forward jump
	if( !f1pp_par->get_delayed_label_name().empty() )
	{
		if( f1pp_par->get_delayed_label_name().equal( name ) )
		{
			f1pp_par->arrived_to_delayed_label( *this );
		}
	}

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const f1::pp::string & f1::f1pp::label::get_name() const
{
	return m_name;
}
const f1::parser::Iparser::position & f1::f1pp::label::get_position() const
{
	return m_position;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::label::jump( pp::Iparser & par ) const
{
	// Set parser pointer
	par.get_parser()->set_pos( m_position );

	// Restore execution stack
	par.set_frame_stack( m_frame_stack );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF label.cxx*/
