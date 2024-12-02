/*switch_long_wide_char.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_long_wide_char::switch_long_wide_char( unsigned flags ) NOEXCEPT:
	super( flags ),
	m_value( false )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_long_wide_char::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_long_wide_char::get_long_name() const NOEXCEPT
{
	return "long-wide-char";
}
const char * f1::pp::frontend::f1pp::switch_long_wide_char::get_desc() const NOEXCEPT
{
	return "wchar_t size is long (32-bits, UTF-32)";
}
/*const char * f1::pp::frontend::f1pp::switch_long_wide_char::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_long_wide_char::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_long_wide_char::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
//	m_value = _T('-') != switchar;

	if( _T('-') != switchar )
		application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) c_pp::parser::modify_c_flags_parser_command(
			c_pp::parser::c_flags_t::fl_long_wchar_t ) );
	else
		application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) c_pp::parser::modify_c_flags_parser_command(
			c_pp::parser::c_flags_t::null, c_pp::parser::c_flags_t::fl_long_wchar_t ) );

	if( nullchar != *rest )
		application::g_app->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}
/*const status_t f1::pp::frontend::f1pp::switch_long_wide_char::apply( pp::context & ctx )
{
	if( m_value )
		ctx.append_parser_parameter( c_pp::parser::m_guid_set_c_flags, uintmax_t(c_pp::parser::c_flags_t::fl_long_wide_char) );
	else
		ctx.append_parser_parameter( c_pp::parser::m_guid_clear_c_flags, uintmax_t(c_pp::parser::c_flags_t::fl_long_wide_char) );
	return STATUS_SUCCESS;
}*/

/*END OF switch_long_wide_char.cxx*/
