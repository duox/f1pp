/*output_class.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::output::ppline::output_class::output_class( const core::Iidentifier * ident ) noexcept:
	super( ident )
{
}

// {9D5858A2-9BF3-43CA-9217-95AF63FF4B13}
const f1::guid_t f1::pp::output::ppline::output_class::m_guid =
{ 0x9d5858a2, 0x9bf3, 0x43ca, { 0x92, 0x17, 0x95, 0xaf, 0x63, 0xff, 0x4b, 0x13 } };
const f1::guid_t & f1::pp::output::ppline::output_class::get_guid() const
{
	return m_guid;
}
const char * f1::pp::output::ppline::output_class::get_name() const
{
	return "f1pp.ppline";
}

const status_t f1::pp::output::ppline::output_class::new_output( core::Tref<pp::Ioutput> & res, const core::Iidentifier * ident )
{
	return res.createT<pp::output::ppline::output>( ident );
}

/*END OF output_class.cxx*/
