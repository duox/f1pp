/*switch_endianness.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_endianness::switch_endianness( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_endianness::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_endianness::get_long_name() const NOEXCEPT
{
	return "endianness";
}
const char * f1::pp::frontend::f1pp::switch_endianness::get_desc() const NOEXCEPT
{
	return "Specify input characters endianness";
}
/*const char * f1::pp::frontend::f1pp::switch_endianness::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_endianness::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_endianness::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	core::string value( rest );
	if( value.equal_nocase( "le" ) || value.equal_nocase( "little" ) )
	{
		application::get_app()->get_context()->modify_flags( pp::context::fl_little_endian, pp::context::fl_big_endian );
	}
	else if( value.equal_nocase( "be" ) || value.equal_nocase( "big" ) )
	{
		application::get_app()->get_context()->modify_flags( pp::context::fl_big_endian, pp::context::fl_little_endian );
	}
	else
		application::get_app()->fatal_error( _T("%c%s: expected LE, little or BE, big"), switchar, core::string(name).c_str() );

	return STATUS_SUCCESS;
}

/*END OF switch_endianness.cxx*/
