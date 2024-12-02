/*switch_disable_output.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_disable_output::switch_disable_output( unsigned flags ) NOEXCEPT:
	super( flags )/*,
	m_value( false )*/
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_disable_output::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_disable_output::get_long_name() const NOEXCEPT
{
	return "disable-output";
}
const char * f1::pp::frontend::f1pp::switch_disable_output::get_desc() const NOEXCEPT
{
	return "Process only directives, don't do any output";
}
/*const char * f1::pp::frontend::f1pp::switch_disable_output::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_disable_output::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_disable_output::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( _T('-') == switchar )
	{
		application::get_app()->set_output_class_guid( application::get_app()->get_default_output_guid() );
		//m_value = false;
	}
	else
	{
		application::get_app()->set_output_class_guid( f1::pp::output::nullout::output_class::m_guid );
		//m_value = true;
	}

	if( nullchar != *rest )
		application::g_app->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}

/*END OF switch_disable_output.cxx*/
