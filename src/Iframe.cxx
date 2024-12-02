/*Iframe.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::Iframe::Iframe( pp::Iparser & par ):
	m_parser( &par ), m_flags( 0 ), m_ref_count( 1 ), m_nesting_count( 0 ), m_warning_flags( 0 )
{
	par.get_parser()->get_location( m_location );
}
f1::pp::Iframe::~Iframe()
{
}

const status_t f1::pp::Iframe::modify_flags( unsigned flags_to_set, unsigned flags_to_clear, unsigned * flags_prev )
{
	m_flags.modify( flags_to_set, flags_to_clear, flags_prev );
	return STATUS_SUCCESS;
}
unsigned f1::pp::Iframe::get_flags( unsigned mask ) const
{
	return m_flags & mask;
}

void f1::pp::Iframe::add_ref()
{
	++ m_ref_count;
}
void f1::pp::Iframe::inc_nesting()
{
	++ m_nesting_count;
}
void f1::pp::Iframe::dec_nesting()
{
	__debugbreak_ifnot( 0 != m_nesting_count )
		-- m_nesting_count;
}
unsigned f1::pp::Iframe::get_nesting() const
{
	return m_nesting_count;
}


const status_t f1::pp::Iframe::enter()
{
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iframe::leave()
{
	release();
	return STATUS_SUCCESS;
}
void f1::pp::Iframe::release()
{
	__debugbreak_if( 0 == m_ref_count )
		return;
	-- m_ref_count;
	if( 0 == m_ref_count )
		delete_self();
}
void f1::pp::Iframe::delete_self()
{
	delete this;
}
const status_t f1::pp::Iframe::set_warning_flags( unsigned flags )
{
	__assert( 0 == (m_warning_flags & flags) );
	m_warning_flags |= flags;
	return STATUS_SUCCESS;
}
unsigned f1::pp::Iframe::get_warning_flags( unsigned mask ) const
{
	return m_warning_flags & mask;
}

/*END OF Iframe.cxx*/
