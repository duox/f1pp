/*ident_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::comext::ident_directive::ident_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::comext::ident_directive::get_name() const
{
	return "ident";
}
// {34FF0B75-BF22-470A-8E85-EB4E770E9E67}
const f1::guid_t f1::c_pp::comext::ident_directive::m_guid =
{ 0x34ff0b75, 0xbf22, 0x470a, { 0x8e, 0x85, 0xeb, 0x4e, 0x77, 0x0e, 0x9e, 0x67 } };
const f1::guid_t & f1::c_pp::comext::ident_directive::get_guid() const
{
	return m_guid;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

/*END OF ident_directive.cxx*/
