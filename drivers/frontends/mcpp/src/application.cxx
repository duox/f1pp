/*application.cxx*/
#include "pch.h"
#include "main.h"

static mcpp_application::arg_desc args[] =
{
	{ f1::util::cmdline::switch_handler(&mcpp_application::switch_comments),nullcstr,
		_T("C"), nullcstr,  nullcstr,
		_T("output also comments.") },
	{ f1::util::cmdline::switch_handler(&mcpp_application::switch_define),	nullcstr,
		_T("D"), nullcstr, _T("<macro>[=<value>]"),
		_T("define <macro> as <value> (default:1).") },
	{ f1::util::cmdline::switch_handler(&mcpp_application::switch_define),	nullcstr,
		_T("D"), nullcstr,	_T("<macro(args)>[=<replace>]"),
		_T("Define <macro(args)> as <replace>.") },
	{ f1::util::cmdline::switch_handler(&mcpp_application::switch_encoding),nullcstr,
		_T("e"), nullcstr,	_T("<encoding>"),
		_T("Change the default multi-byte character encoding to one of:\n")
		_T("euc_jp, gb2312, ksc5601, big5, sjis, iso2022_jp, utf8.") },
	{ f1::util::cmdline::switch_handler(&mcpp_application::switch_include),	nullcstr,
		_T("I"), nullcstr, "<directory>",
		"Add <directory> to the #include search list." },
	{ f1::util::cmdline::switch_handler(&mcpp_application::switch_include),	nullcstr,
		_T("I"), nullcstr, "<directory>",
		"Add <directory> to the #include search list." },
};
mcpp_application::application():
	super( _T("mcpp") ),
	m_context( nullptr )
{
}

status_t mcpp_application::preinitialize_application()
{
	m_context = new pp::context;
//	m_context->set_message_sink( &m_message_sink );

	set_switches( args, countof(args) );

	return STATUS_SUCCESS;
}
status_t mcpp_application::uninitialize_application()
{
	delete m_context;
	return STATUS_SUCCESS;
}
status_t mcpp_application::run_application()
{
	if( !get_flags( fl_fatal_error ) )
	{
		if( 0 == m_files_processed && !get_flags( fl_no_help_needed ) )
			print_usage();
		else
			m_context->process();
	}
	return STATUS_SUCCESS;
}

status_t mcpp_application::on_filename( pcsz_t filename )
{
	if( !DBG_VERIFY_INPLACE( nullptr != filename ) )
		return STATUS_ERROR_INVALID_PARAMETER;
	if( !DBG_VERIFY_INPLACE( nullptr != m_context ) )
		return STATUS_ERROR_NOT_INITIALIZED;

	m_context->append_file( filename );
	++ m_files_processed;

	return STATUS_SUCCESS;
}

/*END OF application.cxx*/
