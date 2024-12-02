/*output_class.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::output::linenum::output_class::output_class( const core::Iidentifier * ident ) noexcept:
	super( ident )
{
}

// {2B0EAF97-3540-4B52-97B9-71D4EC11541C}
const f1::guid_t f1::pp::output::linenum::output_class::m_guid =
{ 0x2b0eaf97, 0x3540, 0x4b52, { 0x97, 0xb9, 0x71, 0xd4, 0xec, 0x11, 0x54, 0x1c } };
const f1::guid_t & f1::pp::output::linenum::output_class::get_guid() const
{
	return m_guid;
}
const char * f1::pp::output::linenum::output_class::get_name() const
{
	return "f1pp.linenum";
}

const status_t f1::pp::output::linenum::output_class::new_output( core::Tref<pp::Ioutput> & res, const core::Iidentifier * ident )
{
	return res.createT<pp::output::linenum::output>( ident );
}

/*END OF output_class.cxx*/
