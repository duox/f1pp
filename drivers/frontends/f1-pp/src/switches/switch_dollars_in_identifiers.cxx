/*switch_dollars_in_identifiers.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_dollars_in_identifiers::switch_dollars_in_identifiers( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_dollars_in_identifiers::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_dollars_in_identifiers::get_long_name() const NOEXCEPT
{
	return "dollars-in-identifiers";
}
const char * f1::pp::frontend::f1pp::switch_dollars_in_identifiers::get_desc() const NOEXCEPT
{
	return "Allow dollar signs '$' in identifiers";
}
/*const char * f1::pp::frontend::f1pp::switch_dollars_in_identifiers::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_dollars_in_identifiers::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_dollars_in_identifiers::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( _T('-') != switchar )
		application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) c_pp::parser::modify_c_flags_parser_command(
			c_pp::parser::c_flags_t::fl_dollar_sign_in_idents ) );
	else
		application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) c_pp::parser::modify_c_flags_parser_command(
			c_pp::parser::c_flags_t::null, c_pp::parser::c_flags_t::fl_dollar_sign_in_idents ) );

	/*if( _T('-') == switchar )
		application::get_app()->get_context()->append_parser_parameter( c_pp::parser::m_guid_clear_c_flags, uintmax_t(c_pp::parser::c_flags_t::fl_dollar_sign_in_idents) );
	else
		application::get_app()->get_context()->append_parser_parameter( c_pp::parser::m_guid_set_c_flags, uintmax_t(c_pp::parser::c_flags_t::fl_dollar_sign_in_idents) );*/

	if( nullchar != *rest )
		application::g_app->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}

/*END OF switch_dollars_in_identifiers.cxx*/
