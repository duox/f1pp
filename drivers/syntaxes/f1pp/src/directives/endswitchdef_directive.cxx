/*endswitchdef_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endswitchdef_directive::endswitchdef_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endswitchdef_directive::get_name() const
{
	return "endswitchdef";
}
// {B37C33D4-762F-42EF-A13F-C250F16D4220}
const f1::guid_t f1::f1pp::endswitchdef_directive::m_guid =
{ 0xb37c33d4, 0x762f, 0x42ef, { 0xa1, 0x3f, 0xc2, 0x50, 0xf1, 0x6d, 0x42, 0x20 } };
const f1::guid_t & f1::f1pp::endswitchdef_directive::get_guid() const
{
	return m_guid;
}

/*END OF endswitchdef_directive.cxx*/
