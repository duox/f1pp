/*switch_full_paths.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_full_paths::switch_full_paths( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_full_paths::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_full_paths::get_long_name() const NOEXCEPT
{
	return "full-path";
}
const char * f1::pp::frontend::f1pp::switch_full_paths::get_desc() const NOEXCEPT
{
	return "Display full path in message headers";
}
/*const char * f1::pp::frontend::f1pp::switch_full_paths::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_full_paths::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_full_paths::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( _T('-') == switchar )
		application::get_app()->get_context()->modify_flags( 0, pp::context::fl_msg_full_path );
	else
		application::get_app()->get_context()->modify_flags( pp::context::fl_msg_full_path );

	if( nullchar != *rest )
		application::g_app->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}
/*const status_t f1::pp::frontend::f1pp::switch_full_paths::apply( pp::context & ctx )
{
	if( m_value )
		ctx.modify_flags( pp::context::fl_msg_full_path );
	else
		ctx.modify_flags( 0, pp::context::fl_msg_full_path );
	return STATUS_SUCCESS;
}*/

/*END OF switch_full_paths.cxx*/
