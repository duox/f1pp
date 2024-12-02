/*sccs_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::comext::sccs_directive::sccs_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::comext::sccs_directive::get_name() const
{
	return "sccs";
}
// {0FF78F36-53E8-4981-8216-12C50A4C1C0D}
const f1::guid_t f1::c_pp::comext::sccs_directive::m_guid =
{ 0x0ff78f36, 0x53e8, 0x4981, { 0x82, 0x16, 0x12, 0xc5, 0x0a, 0x4c, 0x1c, 0x0d } };
const f1::guid_t & f1::c_pp::comext::sccs_directive::get_guid() const
{
	return m_guid;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

/*END OF sccs_directive.cxx*/
