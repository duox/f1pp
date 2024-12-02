/*switch_deprecate_include.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_deprecate_include::switch_deprecate_include( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_deprecate_include::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_deprecate_include::get_long_name() const NOEXCEPT
{
	return "deprecate-include";
}
const char * f1::pp::frontend::f1pp::switch_deprecate_include::get_desc() const NOEXCEPT
{
	return "Specify deprecated include file";
}
/*const char * f1::pp::frontend::f1pp::switch_deprecate_include::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_deprecate_include::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_deprecate_include::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( nullchar == *rest )
		application::g_app->fatal_error( _T("%c%s: expected include file name"), switchar, core::string(name).c_str() );
	else
	{
		if( _T('-') != switchar )
			m_files_to_deprecate.append( rest );
		else
			m_files_to_undeprecate.append( rest );
	}
	return STATUS_SUCCESS;
}
const status_t f1::pp::frontend::f1pp::switch_deprecate_include::apply( pp::context & ctx )
{
	if( !m_files_to_deprecate.empty() )
		ctx.append_include_files( m_files_to_deprecate, pp::context::include_file_t::fl_deprecate );
	if( !m_files_to_undeprecate.empty() )
		ctx.append_include_files( m_files_to_undeprecate, 0, pp::context::include_file_t::fl_deprecate );

	return STATUS_SUCCESS;
}

/*END OF switch_deprecate_include.cxx*/
