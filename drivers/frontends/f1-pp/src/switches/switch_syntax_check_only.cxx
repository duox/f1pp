/*switch_syntax_check_only.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_syntax_check_only::switch_syntax_check_only( unsigned flags ) NOEXCEPT:
	super( flags ),
	m_value( false )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_syntax_check_only::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_syntax_check_only::get_long_name() const NOEXCEPT
{
	return "syntax";
}
const char * f1::pp::frontend::f1pp::switch_syntax_check_only::get_desc() const NOEXCEPT
{
	return "Syntax check only";
}
/*const char * f1::pp::frontend::f1pp::switch_syntax_check_only::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_syntax_check_only::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_syntax_check_only::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	m_value = _T('-') != switchar;

	if( nullchar != *rest )
		application::g_app->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}
const status_t f1::pp::frontend::f1pp::switch_syntax_check_only::apply( pp::context & ctx )
{
	if( m_value )
	{
		core::Tref<pp::null_storage> stg;
		stg.createT<pp::null_storage>();
		stg->initialize( ctx );

		ctx.set_storage( stg );
	}
	return STATUS_SUCCESS;
}

/*END OF switch_syntax_check_only.cxx*/
