/*switch_define.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_define::switch_define( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_define::get_name() const NOEXCEPT
{
	return "d";
}
const char * f1::pp::frontend::f1pp::switch_define::get_long_name() const NOEXCEPT
{
	return "define";
}
const char * f1::pp::frontend::f1pp::switch_define::get_desc() const NOEXCEPT
{
	return "Define a macro and optionally assign a value";
}
/*const char * f1::pp::frontend::f1pp::switch_define::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_define::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_define::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	status_t status;

	if( nullchar == *rest )
		application::g_app->fatal_error( _T("%c%s: expected macro name"), switchar, core::string(name).c_str() );

	else
	{
		size_t len = rest.length();
		const char * start = rest.data();
		const char * end = start + len;
		const char * pos = _strnchr( start, len, '=' );

		// Append/remove macro
		if( '+' == switchar )
		{
			if( nullcstr != pos )
				status = application::get_app()->get_context()->append_macro( start, pos - start, pos + 1, end - pos - 1 );
			else
				status = application::get_app()->get_context()->append_macro( start, len );

			if( STATUS_ERROR_ALREADY_EXISTS == status )
			{
				application::g_app->fatal_error( _T("%c%s: macro already defined: %s (use -u to undefine first)"),
					switchar, core::string(name).c_str(),
					core::string(start, len).c_str()
					);
			}
		}
		else
		{
			if( nullptr != pos )
			{
				application::g_app->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
				len = pos - start;
			}
			application::get_app()->get_context()->remove_macro( start, len );
		}
	}

	return STATUS_SUCCESS;
}

/*END OF switch_define.cxx*/
