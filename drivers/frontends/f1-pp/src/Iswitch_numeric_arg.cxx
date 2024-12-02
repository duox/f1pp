/*Iswitch_numeric_arg.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::frontend::f1pp::Iswitch_numeric_arg::Iswitch_numeric_arg( bitflags_t flags ) NOEXCEPT:
	super( flags )
{
}

status_t f1::pp::frontend::f1pp::Iswitch_numeric_arg::process_numeric_arg( intmax_t & value, char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( nullchar == *rest )
	{
		application::g_app->fatal_error( _T("%c%s: expected integer value"), switchar, core::string(name).c_str() );
		return STATUS_ERROR_INVALID_SYNTAX;
	}

	const char * ep;
	intmax_t n = _strntol( rest.data(), rest.length(), &ep, 0 );
	if( ep != rest.data_end() )
	{
		application::g_app->fatal_error( _T("%c%s: unexpected characters after integer"), switchar, core::string(name).c_str() );
		return STATUS_ERROR_INVALID_SYNTAX;
	}
	value = n;

	return STATUS_SUCCESS;
}

/*END OF Iswitch_numeric_arg.cxx*/
