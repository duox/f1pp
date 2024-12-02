/*Istorage_class_class.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::Istorage_class::Istorage_class( const core::Iidentifier * ident ) __noexcept:
	super( ident )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const f1::guid_t & f1::pp::Istorage_class::get_guid() const
{
	DBG_FAIL_ON_PURE_CALL();
	return f1::guid_t::invalid;
}
const char * f1::pp::Istorage_class::get_name() const
{
	DBG_FAIL_ON_PURE_CALL();
	return nullcstr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::Istorage_class::new_storage( core::Tref<pp::Istorage> & res, const core::Iidentifier * ident )
{
	DBG_FAIL_ON_PURE_CALL();
	res = nullptr;
	__unreferenced_parameter( ident );
	return STATUS_ERROR_PURE_CALL;
}

/*END OF Istorage_class.cxx*/
