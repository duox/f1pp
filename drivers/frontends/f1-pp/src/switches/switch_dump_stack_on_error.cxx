/*switch_dump_stack_on_error.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_dump_stack_on_error::switch_dump_stack_on_error( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_dump_stack_on_error::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_dump_stack_on_error::get_long_name() const NOEXCEPT
{
	return "dump-stack-on-error";
}
const char * f1::pp::frontend::f1pp::switch_dump_stack_on_error::get_desc() const NOEXCEPT
{
	return "Dump macro expansion stack on error";
}
/*const char * f1::pp::frontend::f1pp::switch_dump_stack_on_error::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_dump_stack_on_error::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_dump_stack_on_error::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	pp::Iparser::parser_flags_t flags_to_set = null;
	pp::Iparser::parser_flags_t flags_to_clear = null;

	if( _T('-') == switchar )
		flags_to_clear = pp::Iparser::parser_flags_t::fl_dump_macro_stack_on_error;
	else
		flags_to_set = pp::Iparser::parser_flags_t::fl_dump_macro_stack_on_error;

	application::get_app()->get_context()->add_preinit_parser_command(
		*new(std::nothrow) pp::Iparser::cmd_modify_flags(
			flags_to_set, flags_to_clear
			)
		);

	if( nullchar != *rest )
		application::g_app->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}
/*const status_t f1::pp::frontend::f1pp::switch_dump_stack_on_error::apply( pp::context & ctx )
{
	if( m_value )
		ctx.modify_flags( pp::context::fl_msg_full_path );
	else
		ctx.modify_flags( 0, pp::context::fl_msg_full_path );
	return STATUS_SUCCESS;
}*/

/*END OF switch_dump_stack_on_error.cxx*/
