/*region_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::region_directive::region_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::region_directive::get_name() const
{
	return "region";
}

// {3375F9CA-E419-4942-B5AF-F0C67C82BE79}
const f1::guid_t f1::f1pp::region_directive::m_guid =
{ 0x3375f9ca, 0xe419, 0x4942, { 0xb5, 0xaf, 0xf0, 0xc6, 0x7c, 0x82, 0xbe, 0x79 } };
const f1::guid_t & f1::f1pp::region_directive::get_guid() const
{
	return m_guid;
}

/*END OF region_directive.cxx*/
