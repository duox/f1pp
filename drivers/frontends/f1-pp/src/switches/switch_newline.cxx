/*switch_newline.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_newline::switch_newline( unsigned flags ) NOEXCEPT:
	super( flags ),
	m_value( false )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_newline::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_newline::get_long_name() const NOEXCEPT
{
	return "newline";
}
const char * f1::pp::frontend::f1pp::switch_newline::get_desc() const NOEXCEPT
{
	return "Set output newline type: nl|cr|nlcr|crnl";
}
/*const char * f1::pp::frontend::f1pp::switch_newline::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_newline::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_newline::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	const pp::context::newline_type_t newline_type = f1::pp::context::get_newline_type_from_string( rest.data(), rest.length() );
	if( f1::pp::context::newline_error == newline_type )
	{
		application::g_app->fatal_error( _T("%c%s: expected one of cr|crlf|lf|lfcr"), switchar, core::string(name).c_str() );
		return STATUS_SUCCESS;
	}
	application::get_app()->get_context()->set_newline_type( newline_type );
	return STATUS_SUCCESS;
}

/*END OF switch_newline.cxx*/
