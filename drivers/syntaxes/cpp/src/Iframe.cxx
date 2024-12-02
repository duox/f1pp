/*Iframe.cxx*/
#include "pch.h"
#include "main.h"

f1::c_pp::Iframe::Iframe( c_pp::parser & par ):
	super( par )//, m_was_allowed( true )
{
//	if( !par.is_exec_allowed() )
//		m_flags |= fl_initially_disabled;
}
void f1::c_pp::Iframe::check_different_sources()
{
	// Warn if sources differ
	if( 0 == (m_warning_flags & fl_warned_different_sources) )
	{
		if( m_parser->get_parser()->get_current_source() != get_initial_location().m_source )
		{
			m_parser->message( syntax::IMSG_WARNING_DIFFERENT_SOURCES );
			m_parser->message( get_initial_location(), syntax::IMSG_INFO_STARTED_HERE );
			m_warning_flags |= fl_warned_different_sources;
		}
	}
}

const status_t f1::c_pp::Iframe::enter()
{
	core::Tref<f1::c_pp::parser> c_pp_par = m_parser.dynamic_pointer_cast<c_pp::parser>();
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	//m_was_allowed = c_pp_par->is_exec_allowed();
	if( !c_pp_par->was_exec_allowed() )
		modify_flags( fl_initially_disabled );

	return super::enter();
}
const status_t f1::c_pp::Iframe::leave()
{
	core::Tref<f1::c_pp::parser> c_pp_par = m_parser.dynamic_pointer_cast<c_pp::parser>();
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	//c_pp_par->allow_exec( m_was_allowed );
	c_pp_par->set_allow_exec( !get_flags(fl_initially_disabled) );

	return super::leave();
}

const status_t f1::c_pp::Iframe::allow_exec( bool allow, bool force )
{
	if( 0 != get_flags( pp::Iframe::fl_initially_disabled ) )
		return STATUS_SUCCESS;
	return m_parser.dynamic_pointer_cast<c_pp::parser>()->set_allow_exec( allow, force );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
f1::c_pp::idle_frame::idle_frame( Idirective & drc, c_pp::parser & par ):
	m_directive( &drc )
{
}
const char * f1::c_pp::idle_frame::get_openning_directive_name() const
{
	return m_directive->get_name();
}
const char * f1::c_pp::idle_frame::get_closing_directive_name() const
{
	return m_directive->get_guid();
}
const f1::guid_t & f1::c_pp::idle_frame::get_openning_directive_guid() const
{
}
const f1::guid_t & f1::c_pp::idle_frame::get_closing_directive_guid() const
{
}
*/
/*END OF Iframe.cxx*/
