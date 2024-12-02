/*switch_max_include_depth.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_max_include_depth::switch_max_include_depth( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_max_include_depth::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_max_include_depth::get_long_name() const NOEXCEPT
{
	return "max-include-depth";
}
const char * f1::pp::frontend::f1pp::switch_max_include_depth::get_desc() const NOEXCEPT
{
	return "Specify maximum include depth";
}
/*const char * f1::pp::frontend::f1pp::switch_max_include_depth::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	"Specify maximum include depth limit; if include depth will grow larger than the limit,"
	"a warning is issued."
};
const char * f1::pp::frontend::f1pp::switch_max_include_depth::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_max_include_depth::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	status_t status;

	intmax_t depth = 0;
	if( super::process_numeric_arg( depth, switchar, name, rest ).failed_status( status ) )
		return status;

	application::get_app()->get_context()->set_limit_value( limit_t::li_maximum_include_depth, intmax_t(depth) );

	return STATUS_SUCCESS;
}

/*END OF switch_max_include_depth.cxx*/
