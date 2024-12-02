/*switch_debug.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_debug::switch_debug( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_debug::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_debug::get_long_name() const NOEXCEPT
{
	return "debug";
}
const char * f1::pp::frontend::f1pp::switch_debug::get_desc() const NOEXCEPT
{
	return "Output advanced debugging information";
}
/*const char * f1::pp::frontend::f1pp::switch_debug::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_debug::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_debug::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( _T('-') == switchar )
	{
		f1::pp::service::get().modify_flags( 0, f1::pp::service::fl_debug_output );
		f1::util::application::g_app->modify_flags( 0, util::application::fl_dump_leaks_on_exit );
	}
	else
	{
		f1::pp::service::get().modify_flags( f1::pp::service::fl_debug_output, 0 );
		f1::util::application::g_app->modify_flags( util::application::fl_dump_leaks_on_exit, 0 );
	}

	if( nullchar != *rest )
		application::g_app->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}

/*END OF switch_debug.cxx*/
