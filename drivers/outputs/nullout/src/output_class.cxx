/*output_class.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::output::nullout::output_class::output_class( const core::Iidentifier * ident ) noexcept:
	super( ident )
{
}

// {32A8E4D2-9032-4349-AAF1-E2900DABEEDA}
const f1::guid_t f1::pp::output::nullout::output_class::m_guid =
{ 0x32a8e4d2, 0x9032, 0x4349, { 0xaa, 0xf1, 0xe2, 0x90, 0x0d, 0xab, 0xee, 0xda } };
const f1::guid_t & f1::pp::output::nullout::output_class::get_guid() const
{
	return m_guid;
}
const char * f1::pp::output::nullout::output_class::get_name() const
{
	return "f1pp.nullout";
}

const status_t f1::pp::output::nullout::output_class::new_output( core::Tref<pp::Ioutput> & res, const core::Iidentifier * ident )
{
	return res.createT<pp::output::nullout::output>( ident );
}

/*END OF output_class.cxx*/
