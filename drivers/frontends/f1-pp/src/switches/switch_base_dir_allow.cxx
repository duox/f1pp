/*switch_base_dir_allow.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_base_dir_allow::switch_base_dir_allow( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_base_dir_allow::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_base_dir_allow::get_long_name() const NOEXCEPT
{
	return "base-dir-allow";
}
const char * f1::pp::frontend::f1pp::switch_base_dir_allow::get_desc() const NOEXCEPT
{
	return "Append allowed base dir";
}
/*const char * f1::pp::frontend::f1pp::switch_base_dir_allow::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_base_dir_allow::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_base_dir_allow::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( nullchar == *rest )
		application::g_app->fatal_error( _T("%c%s: expected directory name"), switchar, core::string(name).c_str() );
	else
	{
		application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) c_pp::parser::append_allowed_base_path_command(
			rest, _T('-') != switchar ) );
	}
	return STATUS_SUCCESS;
}

/*END OF switch_base_dir_allow.cxx*/
