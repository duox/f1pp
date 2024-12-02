/*switch_dump.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_dump::switch_dump( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_dump::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_dump::get_long_name() const NOEXCEPT
{
	return "dump";
}
const char * f1::pp::frontend::f1pp::switch_dump::get_desc() const NOEXCEPT
{
	return "Dump entities (macros, pragmas, includes...)";
}
/*const char * f1::pp::frontend::f1pp::switch_dump::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	"General format: --dump:item,...[:filename]\n"
	"Dumps items to console and debug output or, it filename is not omitted, to file.\n"
	"Items are:\n"
	" directives	: dump all usable directives;\n"
	" includes		: dump all parsed files;\n"
	" macros		: dump all macros except special pseudo-macros such as __has_include;\n"
	" pragmas		: dump all pragmas that pp supports;\n"
	" known-pragmas	: dump all pragmas that pp doesn't support but recognizes;\n"
	" standards		: dump all standards.\n"
	"Note that switch type '-' or '+' is ignored; switch always assumes '+'.\n"
};
const char * f1::pp::frontend::f1pp::switch_dump::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const struct {
	const char *	m_name;
	size_t			m_name_len;
	f1::pp::context::dump_flags_t	m_dump_flags;
} param_name_flags_list[] = {
	{ STR_LEN_PAIR("directives"),	f1::pp::context::dump_flags_t::fl_dump_directives },
	{ STR_LEN_PAIR("includes"),		f1::pp::context::dump_flags_t::fl_dump_includes },
	{ STR_LEN_PAIR("macros"),		f1::pp::context::dump_flags_t::fl_dump_macros },
	{ STR_LEN_PAIR("pragmas"),		f1::pp::context::dump_flags_t::fl_dump_pragmas },
	{ STR_LEN_PAIR("known-pragmas"),f1::pp::context::dump_flags_t::fl_dump_known_pragmas },
	{ STR_LEN_PAIR("standards"),	f1::pp::context::dump_flags_t::fl_dump_standards },
};

status_t f1::pp::frontend::f1pp::switch_dump::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	// Check current state
	if( nullchar == *rest )
	{
		application::g_app->fatal_error( _T("%c%s: expected comma-delimited list of items"), switchar, core::string(name).c_str() );
		return STATUS_ERROR_INVALID_SYNTAX;
	}

	// Parse all comma-delimited list of flags
	f1::pp::context::dump_flags_t	combined_dump_flags = pp::context::dump_flags_t::null;
	const char * s = rest.data();
	const char * end_s = rest.data_end();
	size_t i;
	while( s < end_s )
	{
		// Find the end of item identifier
		const char * const item_start = s;
		for( ; s < end_s && ',' != *s && ':' != *s; ++ s )
			;
		const size_t item_len = s - item_start;

		if( 0 == item_len )
		{
			application::g_app->fatal_error( _T("%c%s: expected item name"), switchar, core::string(name).c_str() );
			return STATUS_ERROR_INVALID_SYNTAX;
		}

		// Find the object
		for( i = 0; i < _countof(param_name_flags_list); ++ i )
		{
			if( item_len != param_name_flags_list[i].m_name_len )
				continue;
			if( !_memicmp( item_start, param_name_flags_list[i].m_name, item_len ) )
			{
				if( combined_dump_flags & param_name_flags_list[i].m_dump_flags )
				{
					application::g_app->warning( _T("%c%s: item already used: %s"), switchar, core::string(name).c_str(),
						core::string( item_start, item_len ).c_str() );
				}
				else
					combined_dump_flags |= param_name_flags_list[i].m_dump_flags;
				break;
			}
		}
		if( i >= _countof(param_name_flags_list) )
		{
			application::g_app->fatal_error( _T("%c%s: unknown item name: %s"), switchar, core::string(name).c_str(),
				core::string( item_start, item_len ).c_str() );
			return STATUS_ERROR_INVALID_SYNTAX;
		}

		// Iterate
		if( ',' != *s )
			break;
		++ s;
	}
	if( pp::context::dump_flags_t::null == combined_dump_flags )
	{
		application::g_app->fatal_error( _T("%c%s: expected comma-delimited list of items"), switchar, core::string(name).c_str() );
		return STATUS_ERROR_INVALID_SYNTAX;
	}

	// Parse file name
	core::string filename;
	if( ':' == *s )
	{
		++ s;
		filename.assign( s, end_s - s );
	}

	// Add new dump task
	application::get_app()->get_context()->append_dump_task(
		combined_dump_flags,
		filename.data(), filename.length() );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF switch_dump.cxx*/
