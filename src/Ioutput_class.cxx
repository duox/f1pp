/*Ioutput_class_class.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::Ioutput_class::Ioutput_class( const core::Iidentifier * ident ) __noexcept:
	super( ident )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const f1::guid_t & f1::pp::Ioutput_class::get_guid() const
{
	DBG_FAIL_ON_PURE_CALL();
	return f1::guid_t::invalid;
}
const char * f1::pp::Ioutput_class::get_name() const
{
	DBG_FAIL_ON_PURE_CALL();
	return nullcstr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::Ioutput_class::new_output( core::Tref<pp::Ioutput> & res, const core::Iidentifier * ident )
{
	DBG_FAIL_ON_PURE_CALL();
	res = nullptr;
	__unreferenced_parameter( ident );
	return STATUS_ERROR_PURE_CALL;
}

/*END OF Ioutput_class.cxx*/
