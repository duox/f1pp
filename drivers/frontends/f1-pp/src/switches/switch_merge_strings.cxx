/*switch_merge_strings.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_merge_strings::switch_merge_strings( unsigned flags ) NOEXCEPT:
	super( flags ),
	m_value( false )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_merge_strings::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_merge_strings::get_long_name() const NOEXCEPT
{
	return "merge-strings";
}
const char * f1::pp::frontend::f1pp::switch_merge_strings::get_desc() const NOEXCEPT
{
	return "Merge consecutive strings";
}
/*const char * f1::pp::frontend::f1pp::switch_merge_strings::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_merge_strings::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_merge_strings::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	m_value = _T('-') != switchar;

	if( nullchar != *rest )
		application::g_app->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}
const status_t f1::pp::frontend::f1pp::switch_merge_strings::apply( pp::context & ctx )
{
	if( m_value )
		ctx.modify_flags( pp::context::fl_merge_strings );
	else
		ctx.modify_flags( 0, pp::context::fl_merge_strings );
	return STATUS_SUCCESS;
}

/*END OF switch_merge_strings.cxx*/
