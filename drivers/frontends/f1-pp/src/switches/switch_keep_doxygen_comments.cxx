/*switch_keep_doxygen_comments.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_keep_doxygen_comments::switch_keep_doxygen_comments( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_keep_doxygen_comments::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_keep_doxygen_comments::get_long_name() const NOEXCEPT
{
	return "keep-doxygen-comments";
}
const char * f1::pp::frontend::f1pp::switch_keep_doxygen_comments::get_desc() const NOEXCEPT
{
	return "Keep Doxygen comments in output";
}
/*const char * f1::pp::frontend::f1pp::switch_keep_doxygen_comments::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_keep_doxygen_comments::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_keep_doxygen_comments::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( _T('-') != switchar )
		application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) c_pp::parser::modify_c_flags_parser_command(
			c_pp::parser::c_flags_t::fl_keep_doxygen_comments ) );
	else
		application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) c_pp::parser::modify_c_flags_parser_command(
			c_pp::parser::c_flags_t::null, c_pp::parser::c_flags_t::fl_keep_doxygen_comments ) );

	if( nullchar != *rest )
		application::get_app()->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}

/*END OF switch_keep_doxygen_comments.cxx*/
