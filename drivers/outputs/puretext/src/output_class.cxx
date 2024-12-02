/*output_class.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::output::puretext::output_class::output_class( const core::Iidentifier * ident ) noexcept:
	super( ident )
{
}

// {14479968-D67D-49CE-B631-ABDA752DC9F1}
const f1::guid_t f1::pp::output::puretext::output_class::m_guid =
{ 0x14479968, 0xd67d, 0x49ce, { 0xb6, 0x31, 0xab, 0xda, 0x75, 0x2d, 0xc9, 0xf1 } };
const f1::guid_t & f1::pp::output::puretext::output_class::get_guid() const
{
	return m_guid;
}
const char * f1::pp::output::puretext::output_class::get_name() const
{
	return "f1pp.puretext";
}

const status_t f1::pp::output::puretext::output_class::new_output( core::Tref<pp::Ioutput> & res, const core::Iidentifier * ident )
{
	return res.createT<pp::output::puretext::output>( ident );
}

/*END OF output_class.cxx*/
