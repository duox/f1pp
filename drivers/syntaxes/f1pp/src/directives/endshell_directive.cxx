/*endshell_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endshell_directive::endshell_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endshell_directive::get_name() const
{
	return "endshell";
}
// {1A1D59F6-C6C6-4D49-B017-9542183A7AF6}
const f1::guid_t f1::f1pp::endshell_directive::m_guid =
{ 0x1a1d59f6, 0xc6c6, 0x4d49, { 0xb0, 0x17, 0x95, 0x42, 0x18, 0x3a, 0x7a, 0xf6 } };
const f1::guid_t & f1::f1pp::endshell_directive::get_guid() const
{
	return m_guid;
}

// Operations

/*END OF endshell_directive.cxx*/
