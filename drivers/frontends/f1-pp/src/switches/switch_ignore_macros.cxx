/*switch_ignore_macros.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_ignore_macros::switch_ignore_macros( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_ignore_macros::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_ignore_macros::get_long_name() const NOEXCEPT
{
	return "ignore-macros";
}
const char * f1::pp::frontend::f1pp::switch_ignore_macros::get_desc() const NOEXCEPT
{
	return "Do not expand macros";
}
/*const char * f1::pp::frontend::f1pp::switch_ignore_macros::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_ignore_macros::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_ignore_macros::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( _T('-') != switchar )
		application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) c_pp::parser::modify_c_flags_parser_command(
			c_pp::parser::c_flags_t::fl_do_not_expand_macros ) );
	else
		application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) c_pp::parser::modify_c_flags_parser_command(
			c_pp::parser::c_flags_t::null, c_pp::parser::c_flags_t::fl_do_not_expand_macros ) );

	__unreferenced_parameter( name );
	__unreferenced_parameter( rest );
	return STATUS_SUCCESS;
}

/*END OF switch_ignore_macros.cxx*/
