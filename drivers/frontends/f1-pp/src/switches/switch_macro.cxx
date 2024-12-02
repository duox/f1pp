/*switch_macro.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_macro::switch_macro( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_macro::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_macro::get_long_name() const NOEXCEPT
{
	return "macro";
}
const char * f1::pp::frontend::f1pp::switch_macro::get_desc() const NOEXCEPT
{
	return "Specify a macro's characteristics";
}
/*const char * f1::pp::frontend::f1pp::switch_macro::get_params() const NOEXCEPT
{
	return "deprecated disabled ignore";
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_macro::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static struct {
	const char *name;
	size_t		name_len;
	unsigned	flags_to_set;
	unsigned	flags_to_clear;
} param_name_flags_list[] = {
	{ STR_LEN_PAIR("deprecated"),	f1::pp::Imacro::macro_flags_t::fl_deprecated, 0 },
	{ STR_LEN_PAIR("disabled"),		f1::pp::Imacro::macro_flags_t::fl_disabled, 0 },
	{ STR_LEN_PAIR("ignore"),		f1::pp::Imacro::macro_flags_t::fl_ignore, 0 },
	{ STR_LEN_PAIR("immutable"),	f1::pp::Imacro::macro_flags_t::fl_immutable, 0 },
};
status_t f1::pp::frontend::f1pp::switch_macro::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( nullchar == *rest )
	{
		application::g_app->fatal_error( _T("%c%s: expected integer value"), switchar, core::string(name).c_str() );
		return STATUS_ERROR_INVALID_SYNTAX;
	}
	const char * s = rest.data();
	const char * end_s = rest.data_end();

	// Fetch macro name
	const char * macro_start = s;
	for( ; s < end_s && (isalnum( *s ) || '_' == *s || '$' == *s); ++ s )
		;
	if( ':' != *s )
	{
		application::g_app->fatal_error( _T("%c%s: expected ':' before list of flags"), switchar, core::string(name).c_str() );
		return STATUS_ERROR_INVALID_SYNTAX;
	}
	const char * macro_end = s;
	++ s;

	// Process all flags
	unsigned flags_to_clear = 0;
	unsigned flags_to_set = 0;
	for(;;)
	{
		// Fetch parameters
		const char * param_start = s;
		for( ; s < end_s && isalnum( *s ); ++ s )
			;
		const char * param_end = s;

		// Find flag
		size_t i;
		size_t param_len = param_end - param_start;
		for( i = 0; i < _countof(param_name_flags_list); ++ i )
		{
			if( param_name_flags_list[i].name_len != param_len )
				continue;
			if( core::string::equal_nocase( param_name_flags_list[i].name, param_start, param_len ) )
				break;
		}
		if( i >= _countof(param_name_flags_list) )
		{
			application::g_app->fatal_error( _T("%c%s: unknown macro flag: %s"), switchar, core::string(name).c_str(),
				core::string(param_start, param_len).c_str() );
			return STATUS_ERROR_INVALID_SYNTAX;
		}
		flags_to_clear	|= param_name_flags_list[i].flags_to_clear;
		flags_to_set	|= param_name_flags_list[i].flags_to_set;

		// Skip ','
		if( s >= end_s )
			break;
		if( ',' != *s )
		{
			application::g_app->fatal_error( _T("%c%s: expected ',' after '%s'"), switchar, core::string(name).c_str(),
				core::string(param_start, param_len).c_str() );
			return STATUS_ERROR_INVALID_SYNTAX;
		}
		++ s;
	}

	// Set macro flags
	macro_state * ms = m_macro_states.append_new();
	__debugbreak_if( nullptr == ms )
		return STATUS_ERROR_INSUFFICIENT_MEMORY;
	ms->m_macro_name.assign( macro_start, macro_end - macro_start );
	ms->m_flags_to_clear= flags_to_clear;
	ms->m_flags_to_set	= flags_to_set;

	// Exit
	return STATUS_SUCCESS;
}

/*END OF switch_macro.cxx*/
