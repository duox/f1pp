/*endregion_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endregion_directive::endregion_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endregion_directive::get_name() const
{
	return "endregion";
}

// {E6B7CB60-987B-4c47-88F1-3A9F58973DEA}
const f1::guid_t f1::f1pp::endregion_directive::m_guid =
{ 0xe6b7cb60, 0x987b, 0x4c47, { 0x88, 0xf1, 0x3a, 0x9f, 0x58, 0x97, 0x3d, 0xea } };
const f1::guid_t & f1::f1pp::endregion_directive::get_guid() const
{
	return m_guid;
}

/*END OF endregion_directive.cxx*/
