/*switch_timings.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_timings::switch_timings( unsigned flags ) NOEXCEPT:
	super( flags ),
	m_value( false )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_timings::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_timings::get_long_name() const NOEXCEPT
{
	return "timings";
}
const char * f1::pp::frontend::f1pp::switch_timings::get_desc() const NOEXCEPT
{
	return "Calculate timings";
}
/*const char * f1::pp::frontend::f1pp::switch_timings::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_timings::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_timings::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	m_value = _T('-') != switchar;

	if( nullchar != *rest )
		application::g_app->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}

/*END OF switch_timings.cxx*/
