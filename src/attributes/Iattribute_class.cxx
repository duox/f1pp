/*Iattribute_class.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::Iattribute_class::Iattribute_class( const core::Iidentifier * ident ):
	super( ident )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const f1::guid_t & __nodiscard f1::pp::Iattribute_class::get_guid() const
{
	DBG_FAIL_ON_PURE_CALL();
	return f1::guid_t::invalid;
}
const f1::pp::string & __nodiscard f1::pp::Iattribute_class::get_name() const
{
	DBG_FAIL_ON_PURE_CALL();
	return f1::pp::string::null;
}
unsigned __nodiscard f1::pp::Iattribute_class::get_caps( unsigned mask ) const noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}
const f1::pp::param_list * __nodiscard f1::pp::Iattribute_class::get_param_list() const noexcept
{
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::Iattribute_class::new_attribute( core::Tref<pp::Iattribute> & res, const core::Iidentifier * ident )
{
	res = nullptr;
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( ident );
	return STATUS_ERROR_PURE_CALL;
}

/*END OF Iattribute_class.cxx*/
