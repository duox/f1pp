/*Ioption.cxx*/
#include "pch.h"
#include "main.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const f1::core::stringslice & f1::pp::Ioption::get_name() const
{
	DBG_FAIL_ON_PURE_CALL();
	return core::stringslice::null;
}
const f1::guid_t & f1::pp::Ioption::get_guid() const
{
	DBG_FAIL_ON_PURE_CALL();
	return f1::guid_t::invalid;
}

const f1::pp::variant::type_t f1::pp::Ioption::get_expected_type() const
{
	return f1::pp::variant::ty_none;
}

const f1::pp::variant & f1::pp::Ioption::get_default_value() const
{
	return pp::variant::null;
}

const f1::pp::variant & f1::pp::Ioption::get_value() const
{
	return pp::variant::invalid;
}
const status_t f1::pp::Ioption::set_value( const pp::variant & val )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( val );
	return STATUS_ERROR_PURE_CALL;
}

/*END OF Ioption.cxx*/
