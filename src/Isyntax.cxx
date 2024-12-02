/*Isyntax.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::Isyntax::Isyntax( const core::Iidentifier * ident ):
	super( ident )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*const f1::guid_t & f1::pp::Isyntax::get_guid() const
{
	DBG_FAIL_ON_PURE_CALL();
	return f1::guid_t::invalid;
}
const char * f1::pp::Isyntax::get_ident_string() const
{
	DBG_FAIL_ON_PURE_CALL();
	return nullptr;
}
const char * f1::pp::Isyntax::get_description_string() const
{
	DBG_FAIL_ON_PURE_CALL();
	return nullptr;
}
*/

const f1::guid_t * f1::pp::Isyntax::get_preferred_standard_list( __out size_t & count ) const
{
	count = 0;
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::Isyntax::new_parser( f1::core::Tref<Iparser> & res, const core::Iidentifier * ident  )
{
	res = nullptr;
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( ident );
	return STATUS_ERROR_PURE_CALL;
}

const status_t f1::pp::Isyntax::attach_to_context( pp::context & ctx )
{
	__unreferenced_parameter( ctx );
	return STATUS_SUCCESS;
}
const status_t f1::pp::Isyntax::detach_from_context( pp::context & ctx )
{
	__unreferenced_parameter( ctx );
	return STATUS_SUCCESS;
}

/*END OF Isyntax.cxx*/
