/*switch_include.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_include::switch_include( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_include::get_name() const NOEXCEPT
{
	return "i";
}
const char * f1::pp::frontend::f1pp::switch_include::get_long_name() const NOEXCEPT
{
	return "include";
}
const char * f1::pp::frontend::f1pp::switch_include::get_desc() const NOEXCEPT
{
	return "Specify include file";
}
/*const char * f1::pp::frontend::f1pp::switch_include::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_include::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_include::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( nullchar == *rest )
		application::g_app->fatal_error( _T("%c%s: expected include file name"), switchar, core::string(name).c_str() );
	else
	{
		if( _T('-') != switchar )
			application::get_app()->get_context()->append_include_files( ';', core::static_string(rest), pp::context::include_file_t::fl_auto_include );
			//m_files_to_include.append( rest );
		else
			application::get_app()->get_context()->remove_include_files( ';', core::static_string(rest) );
			//m_files_to_exclude.append( rest );
	}
	return STATUS_SUCCESS;
}
/*const status_t f1::pp::frontend::f1pp::switch_include::apply( pp::context & ctx )
{
	if( !m_files_to_include.empty() )
		ctx.append_include_files( m_files_to_include, pp::context::include_file_t::fl_auto_include );
	if( !m_files_to_exclude.empty() )
		ctx.append_include_files( m_files_to_exclude, 0, pp::context::include_file_t::fl_auto_include );

	return STATUS_SUCCESS;
}*/

/*END OF switch_include.cxx*/
