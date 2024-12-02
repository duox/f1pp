/*enum_macro.cxx*/
#include "pch.h"
#include "main.h"

f1::f1pp::enum_macro::enum_macro( const core::Iidentifier * ident ):
	super( ident )
{
}

const status_t f1::f1pp::enum_macro::initialize( f1::f1pp::enumeration & en, const pp::string & name, const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	if( super::initialize( name, attrs, params, value, flags ).failed_status( status ) )
		return status;

	m_enumeration = &en;

	return status;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const f1::f1pp::parser * f1::f1pp::enum_macro::get_parser() const
{
	return m_enumeration->get_parser();
}
f1::f1pp::parser * f1::f1pp::enum_macro::get_parser()
{
	return m_enumeration->get_parser();
}

const f1::f1pp::enumeration * f1::f1pp::enum_macro::get_enumeration() const
{
	return m_enumeration;
}
f1::f1pp::enumeration *	f1::f1pp::enum_macro::get_enumeration()
{
	return m_enumeration;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

void f1::f1pp::enum_macro::on_all_names_removed()
{
	m_enumeration->remove_element( *this );
}

/*END OF enum_macro.cxx*/
