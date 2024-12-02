/*output_class.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::output::gnucpp::output_class::output_class( const core::Iidentifier * ident ) noexcept:
	super( ident )
{
}

// {46E81D83-B36E-41F7-A354-B24AE305F3A5}
const f1::guid_t f1::pp::output::gnucpp::output_class::m_guid =
{ 0x46e81d83, 0xb36e, 0x41f7, { 0xa3, 0x54, 0xb2, 0x4a, 0xe3, 0x05, 0xf3, 0xa5 } };
const f1::guid_t & f1::pp::output::gnucpp::output_class::get_guid() const
{
	return m_guid;
}
const char * f1::pp::output::gnucpp::output_class::get_name() const
{
	return "f1pp.gnucpp";
}

const status_t f1::pp::output::gnucpp::output_class::new_output( core::Tref<pp::Ioutput> & res, const core::Iidentifier * ident )
{
	return res.createT<pp::output::gnucpp::output>( ident );
}

/*END OF output_class.cxx*/
