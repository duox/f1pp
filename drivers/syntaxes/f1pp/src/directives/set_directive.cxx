/*set_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::set_directive::set_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::set_directive::get_name() const
{
	return "set";
}
// {78AAB378-7576-4e8b-B7EE-BF82F075A2AA}
const f1::guid_t f1::f1pp::set_directive::m_guid =
{ 0x78aab378, 0x7576, 0x4e8b, { 0xb7, 0xee, 0xbf, 0x82, 0xf0, 0x75, 0xa2, 0xaa } };
const f1::guid_t & f1::f1pp::set_directive::get_guid() const
{
	return m_guid;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::f1pp::set_directive::operator_complete( f1pp::parser & parser )
{
	if( parser.get_parser()->skip_char( ';' ) )
		return parse( parser );
	return super::operator_complete( parser );
}

/*END OF set_directive.cxx*/
