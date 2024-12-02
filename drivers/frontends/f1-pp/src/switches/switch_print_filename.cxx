/*switch_print_filename.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_print_filename::switch_print_filename( unsigned flags ) NOEXCEPT:
	super( flags ),
	m_filename_print( pp::context::Efilename_print_invalid )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_print_filename::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_print_filename::get_long_name() const NOEXCEPT
{
	return "print_filename";
}
const char * f1::pp::frontend::f1pp::switch_print_filename::get_desc() const NOEXCEPT
{
	return "Print file name mode";
}
/*const char * f1::pp::frontend::f1pp::switch_print_filename::get_params() const NOEXCEPT
{
	return "each|warning|error|none";
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_print_filename::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static struct {
	const char *	m_name;
	ssize_t			m_name_len;
	f1::pp::context::Efilename_print	m_type;
} print_type[] = {
	{ STR_LEN_PAIR("none"),		f1::pp::context::Efilename_print_none },
	{ STR_LEN_PAIR("warning"),	f1::pp::context::Efilename_print_warning },
	{ STR_LEN_PAIR("error"),	f1::pp::context::Efilename_print_error },
	{ STR_LEN_PAIR("each"),		f1::pp::context::Efilename_print_each },
};

status_t f1::pp::frontend::f1pp::switch_print_filename::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	// Fetch identifier
	m_filename_print = pp::context::Efilename_print_invalid;
	if( nullchar != *rest )
	{
		for( size_t i = 0; i < _countof(print_type); ++ i )
		{
			if( print_type[i].m_name_len != rest.length() )
				continue;
			if( core::string::equal_nocase( rest.data(), print_type[i].m_name, print_type[i].m_name_len ) )
			{
				m_filename_print = print_type[i].m_type;
				break;
			}
		}
	}
	if( pp::context::Efilename_print_invalid == m_filename_print )
	{
		application::g_app->fatal_error( _T("%c%s: expected one of each|warning|error|none"), switchar, core::string(name).c_str() );
		return STATUS_ERROR_INVALID_SYNTAX;
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::pp::frontend::f1pp::switch_print_filename::apply( pp::context & ctx )
{
	if( pp::context::Efilename_print_invalid != m_filename_print )
		ctx.set_filename_print( m_filename_print );
	return STATUS_SUCCESS;
}

/*END OF switch_print_filename.cxx*/
