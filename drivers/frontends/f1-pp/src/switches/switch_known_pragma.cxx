/*switch_known_pragma.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_known_pragma::switch_known_pragma( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_known_pragma::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_known_pragma::get_long_name() const NOEXCEPT
{
	return "known_pragma";
}
const char * f1::pp::frontend::f1pp::switch_known_pragma::get_desc() const NOEXCEPT
{
	return "Allow known_pragma";
}
/*const char * f1::pp::frontend::f1pp::switch_known_pragma::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_known_pragma::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_known_pragma::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	f1::core::const_string ident = rest;
	ident.dequotize();

	application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) c_pp::parser::append_known_pragma_command(
		ident, _T('-') != switchar ) );

	__unreferenced_parameter( name );
	return STATUS_SUCCESS;
}
/*const status_t f1::pp::frontend::f1pp::switch_known_pragma::apply( pp::context & ctx )
{
	if( m_value )
		ctx.append_parser_parameter( c_pp::parser::m_guid_set_c_flags, uintmax_t(c_pp::parser::c_flags_t::fl_process_known_pragma) );
	else
		ctx.append_parser_parameter( c_pp::parser::m_guid_clear_c_flags, uintmax_t(c_pp::parser::c_flags_t::fl_process_known_pragma) );
	return STATUS_SUCCESS;
}*/

/*END OF switch_known_pragma.cxx*/
