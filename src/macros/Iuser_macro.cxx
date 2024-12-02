/*Iuser_macro.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::Iuser_macro::Iuser_macro( const core::Iidentifier * ident ) NOEXCEPT:
	super( ident )
{
}

const status_t f1::pp::Iuser_macro::initialize( const pp::string & name, const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	if( super::initialize( attrs, params, value, flags ).failed_status( status ) )
		return status;

	m_name = name;

	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::pp::Iuser_macro::get_name() const
{
	return m_name.c_str();
}

/*END OF Iuser_macro.cxx*/
