/*endswitchndef_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endswitchndef_directive::endswitchndef_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endswitchndef_directive::get_name() const
{
	return "endswitchndef";
}
// {E5614800-A153-42DA-ABC4-9BD52C270A1C}
const f1::guid_t f1::f1pp::endswitchndef_directive::m_guid =
{ 0xe5614800, 0xa153, 0x42da, { 0xab, 0xc4, 0x9b, 0xd5, 0x2c, 0x27, 0x0a, 0x1c } };
const f1::guid_t & f1::f1pp::endswitchndef_directive::get_guid() const
{
	return m_guid;
}

/*END OF endswitchndef_directive.cxx*/
