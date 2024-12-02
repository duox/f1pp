/*switch_undefine.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_undefine::switch_undefine( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_undefine::get_name() const NOEXCEPT
{
	return "u";
}
const char * f1::pp::frontend::f1pp::switch_undefine::get_long_name() const NOEXCEPT
{
	return "undef";
}
const char * f1::pp::frontend::f1pp::switch_undefine::get_desc() const NOEXCEPT
{
	return "Undefine a macro";
}
/*const char * f1::pp::frontend::f1pp::switch_undefine::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_undefine::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_undefine::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( nullchar == *rest )
		application::g_app->fatal_error( _T("%c%s: expected macro name"), switchar, core::string(name).c_str() );

	else
	{
		application::get_app()->get_context()->remove_macro( rest.data(), rest.length() );
	}

	return STATUS_SUCCESS;
}
/*const status_t f1::pp::frontend::f1pp::switch_undefine::apply( pp::context & ctx )
{
	ctx.get_macros_to_undefine().append( m_macros_to_undefine );

	return STATUS_SUCCESS;
}*/

/*END OF switch_undefine.cxx*/
