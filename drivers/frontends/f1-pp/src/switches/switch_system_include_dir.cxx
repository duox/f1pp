/*switch_system_include_dir.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_system_include_dir::switch_system_include_dir( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_system_include_dir::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_system_include_dir::get_long_name() const NOEXCEPT
{
	return "system-include-dir";
}
const char * f1::pp::frontend::f1pp::switch_system_include_dir::get_desc() const NOEXCEPT
{
	return "Specify system include directories";
}
/*const char * f1::pp::frontend::f1pp::switch_system_include_dir::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_system_include_dir::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_system_include_dir::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( nullchar == *rest )
		application::g_app->fatal_error( _T("%c%s: expected directory name(s)"), switchar, core::string(name).c_str() );
	else
	{
		application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) c_pp::parser::cmd_include_path(
			c_pp::parser::include_path_flags_t::fl_system, rest, _T('-') == switchar ) );
	}
	return STATUS_SUCCESS;
}
/*const status_t f1::pp::frontend::f1pp::switch_system_include_dir::apply( pp::context & ctx )
{
	if( !m_dirs_to_include.empty() )
		ctx.get_include_dirs().append_search_paths( m_dirs_to_include );
	if( !m_dirs_to_exclude.empty() )
		ctx.get_include_dirs().remove_search_paths( m_dirs_to_exclude );

	return STATUS_SUCCESS;
}*/

/*END OF switch_system_include_dir.cxx*/
