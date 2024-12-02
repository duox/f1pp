/*lib_message_sink.cxx*/
#include "pch.h"
#include "main.h"

//LNK4093
//LNK4087	CONSTANT keyword is obsolete; use DATA
//LNK1146	no argument specified with option "option"
//LNK4236	%s statement is obsolete. Use %s.
struct  
{
	lib_message_id	lib_id;
	int				mslib_id;
} mslib_warning_pair[] =
{
	{ lib_message_job_in_progress,					-1 },
/*	{ lib_message_def_unrecognized_statement,		4017 },
	{ lib_message_def_syntax_error_in_statement,	1118 },
	{ lib_message_libing_in_progress,				-1 },
	{ lib_message_def_option_ignored,				-1 },
	{ lib_message_def_option_unrecognized,			4017 },
	{ lib_message_def_string_break,				1118 },
	{ lib_message_commit_exceeds_reserve,			4015 },	//LNK4015 for VC6, LNK1229 for VC2003
	{ lib_message_def_expected,					1118 },
	{ lib_message_def_unsupported_statement,		1118 },
	{ lib_message_invalid_section_name,			1185 },
	{ lib_message_file_not_found,					1181 },
*/	// TODO: add other warnings
};
void lib_exe_message_sink::print( pcsz_t file, long line, lib_message_id message_id, va_list ap )
{
	return static_cast<lib_application*>(application::g_app)->print( file, line, message_id, ap );
}

void lib_application::print( pcsz_t file, long line, lib_message_id message_id, va_list ap )
{
	pcsz_t message_kind_string;

	// Display the message
	if( nullptr != file )
		message( _T("%s(%d) : "), file, line );

	// Check message handling
	switch( lib_context::get_default_message_kind( message_id ) )
	{
	case lib_message_kind_fatal_error:
		SET_FLAG(m_system_flags, SF_FATAL_ERROR);
		SET_FLAG(m_message_flags, MF_NO_HELP_NEEDED);
		//m_lib_context->abort_parsing_file( file );
		message_kind_string = _T("fatal error");
		break;
	case lib_message_kind_error:
		++ m_error_count;
		message_kind_string = _T("error");
		break;
	case lib_message_kind_warning:
		++ m_warning_count;
		message_kind_string = _T("warning");
		break;
	case lib_message_kind_informational:
		message_kind_string = _T("\b");
		break;
	default:
		DBG_ASSERT(FALSE);
		message_kind_string = _T("\b");
		break;
	}

	// Find the warning
	size_t i;
	for( i = 0; i < countof(mslib_warning_pair); ++ i )
	{
		if( mslib_warning_pair[i].lib_id == message_id )
		{
			if( -1 != mslib_warning_pair[i].mslib_id )
				message( _T("%s LNK%04d: "), message_kind_string, mslib_warning_pair[i].mslib_id );
			break;
		}
	}
	if( !DBG_VERIFY_INPLACE( i < countof(mslib_warning_pair) ) )
		message( _T("%s: "), message_kind_string );

	// Print the rest
	messageAP( lib_context::get_default_message_string( message_id ), ap );
	message( _T("\n") );

	// Exit
	return;
}

/*END OF lib_message_sink.cxx*/
