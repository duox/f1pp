/*switch_write_bom.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_write_bom::switch_write_bom( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_write_bom::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_write_bom::get_long_name() const NOEXCEPT
{
	return "write-bom";
}
const char * f1::pp::frontend::f1pp::switch_write_bom::get_desc() const NOEXCEPT
{
	return "Write BOM to output";
}
/*const char * f1::pp::frontend::f1pp::switch_write_bom::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_write_bom::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_write_bom::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( _T('-') != switchar )
		application::get_app()->get_context()->modify_output_flags( pp::context::output_flags_t::fl_write_bom );
	else
		application::get_app()->get_context()->modify_output_flags( null, pp::context::output_flags_t::fl_write_bom );

	if( nullchar != *rest )
		application::g_app->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}

/*END OF switch_write_bom.cxx*/
