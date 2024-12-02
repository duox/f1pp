/*switch_system_include_base.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_system_include_base::switch_system_include_base( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_system_include_base::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_system_include_base::get_long_name() const NOEXCEPT
{
	return "system-include-base";
}
const char * f1::pp::frontend::f1pp::switch_system_include_base::get_desc() const NOEXCEPT
{
	return "Specify system include base";
}
/*const char * f1::pp::frontend::f1pp::switch_system_include_base::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_system_include_base::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_system_include_base::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( nullchar == *rest )
		application::g_app->fatal_error( _T("%c%s: expected directory name"), switchar, core::string(name).c_str() );
	else
	{
		application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) pp::Iparser::cmd_system_include_base(
			rest ) );
	}
	return STATUS_SUCCESS;
}

/*END OF switch_system_include_base.cxx*/
