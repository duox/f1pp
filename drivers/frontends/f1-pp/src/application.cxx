/*application.cxx*/
#include "pch.h"
#include "main.h"

static f1::pp::frontend::f1pp::application::arg_desc args[] =
{
/*	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_comments),	nullcstr,
		_T("C"), nullcstr,  nullcstr,
		_T("output also comments."), 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_define),	nullcstr,
		_T("D"), nullcstr, _T("<macro>[=<value>]"),
		_T("define <macro> as <value> (default:1)."), 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_define),	nullcstr,
		_T("D"), nullcstr,	_T("<macro(args)>[=<replace>]"),
		_T("Define <macro(args)> as <replace>."), 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_encoding),	nullcstr,
		_T("e"), nullcstr,	_T("<encoding>"),
		_T("Change the default multi-byte character encoding to one of:\n")
		_T("euc_jp, gb2312, ksc5601, big5, sjis, iso2022_jp, utf8."), 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_include),	nullcstr,
		_T("I"), nullcstr, "<directory>",
		"Add <directory> to the #include search list.", 0 },
*/
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_help),				_T("General"),
		_T("?"), nullcstr, nullcstr,
		"Display command line help", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_help),				_T("General"),
		_T("h"), _T("help"), nullcstr,
		"Display command line help", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_nologo),			_T("General"),
		nullcstr, _T("nologo"), nullcstr,
		"Suppress application start up logo", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_quiet),			_T("General"),
		_T("q"), _T("quiet"), nullcstr,
		"Operate quietly", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_verbose),			_T("General"),
		_T("v"), _T("verbose"), nullcstr,
		"Operate verbosely", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_ver),				_T("General"),
		nullcstr, _T("version"), nullcstr,
		"Display application version", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_ver),				_T("General"),
		nullcstr, _T("ver"), nullcstr,
		"Display application version", 0 },

//	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_endianness),		_T("Input"),
//		nullptr, _T("endianness"), nullcstr,
//		"Specify input characters endianness", 0 },
//	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_input_encoding),	_T("Input"),
//		nullptr, _T("input-encoding"), nullcstr,
//		"Specify input encoding name", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_maximum_char_len),	_T("Input"),
		nullptr, _T("max-char-len"), nullcstr,
		"Specify maximum character constant length", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_maximum_ident_len),_T("Input"),
		nullptr, _T("max-ident-len"), nullcstr,
		"Specify maximum identifier length", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_maximum_iterations),_T("Input"),
		nullptr, _T("max-iterations"), nullcstr,
		"Specify maximum iteration count", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_maximum_recursion),_T("Input"),
		nullptr, _T("max-resursion"), nullcstr,
		"Specify maximum recursion depth", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_nested_comments),	_T("Input"),
		nullptr, _T("nested-comments"), nullcstr,
		"Allow nested comments", 0 },

//	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_define),			_T("Macros"),
//		_T("d"), _T("define"), nullcstr,
//		"Define a macro and optionally assign a value" },
//	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_undefine),			_T("Macros"),
//		_T("u"), _T("undef"), nullcstr,
//		"Undefine a macro", 0 },

//	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_full_paths),		_T("Messages"),
//		nullptr, _T("full-path"), nullcstr,
//		"Display full path in message headers", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_maximum_errors),	_T("Messages"),
		nullptr, _T("max-errors"), nullcstr,
		"Set maximum error count", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_maximum_warnings),	_T("Messages"),
		nullptr, _T("max-warnings"), nullcstr,
		"Set maximum warning count", 0 },

//	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_include),			_T("Includes"),
//		_T("i"), _T("include-file"), nullcstr,
//		"Specify include file", 0 },
//	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_include_dir),		_T("Includes"),
//		_T("I"), _T("include-dir"), nullcstr,
//		"Specify include directories", 0 },

//	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_keep_comments),	_T("Output"),
//		_T("k"), _T("keep-comments"), nullcstr,
//		"Keep comments in output", 0 },
//	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_output_encoding),	_T("Output"),
//		nullptr, _T("output-encoding"), nullcstr,
//		"Specify output encoding name", 0 },
	{ f1::util::cmdline::switch_handler(&f1::pp::frontend::f1pp::application::switch_compress_whitespace),	_T("Output"),
		_T("w"), _T("compress-whitespace"), nullcstr,
		"Output whitespace as small as possible", 0 },
};
f1::pp::frontend::f1pp::application::application():
	super( _T("f1pp") ),
	m_total_time( 0 ), m_context( nullptr )
{
	m_default_output_class_guid = f1::pp::output::linenum::output_class::m_guid;
}

const f1::pp::context * f1::pp::frontend::f1pp::application::get_context() const
{
	return m_context;
}
f1::pp::context * f1::pp::frontend::f1pp::application::get_context()
{
	return m_context;
}

const status_t f1::pp::frontend::f1pp::application::get_version( core::version & ver ) const NOEXCEPT
{
	return f1::pp::service::get().get_version( ver );
}

const status_t f1::pp::frontend::f1pp::application::set_default_output_guid( const f1::guid_t & guid )
{
	m_default_output_class_guid = guid;
	m_context->set_output( nullptr );		// reset output to let the application to recreate it later
	return STATUS_SUCCESS;
}
const f1::guid_t & f1::pp::frontend::f1pp::application::get_default_output_guid() const
{
	return m_default_output_class_guid;
}
const status_t f1::pp::frontend::f1pp::application::set_output_class( pp::Ioutput_class & output_class )
{
	status_t status;
	core::Tref<pp::Ioutput> output;
	__debugbreak_if( service::get().create_output( output, output_class ).failed_status( status ) )
		return status;
	m_context->set_output( output );
	return STATUS_SUCCESS;
}
const status_t f1::pp::frontend::f1pp::application::set_output_class_guid( const f1::guid_t & guid )
{
	status_t status;
	core::Tref<pp::Ioutput> output;
	__debugbreak_if( service::get().create_output( output, guid ).failed_status( status ) )
		return status;
	m_context->set_output( output );
	return STATUS_SUCCESS;
}

status_t f1::pp::frontend::f1pp::application::preinitialize_application() noexcept
{
	status_t status;

	super::preinitialize_application();

	f1::msgout::service::get().set_defaults();

	// Initialize context
	f1::fs::service::get().append_file_system( *new(std::nothrow) f1::fs::windows::file_system );

	// Append outputs and set requested output
	//service::get().append_output_class( m_gnucpp_output_class );
	service::get().append_output_class( m_nullout_output_class );
	service::get().append_output_class( m_linenum_output_class );
	service::get().append_output_class( m_ppline_output_class );
	service::get().append_output_class( m_puretext_output_class );

	// Append supported syntaxes and standards
	m_syntax_cpp.initialize();		service::get().append_syntax( m_syntax_cpp );

	m_standard_c99.initialize();	service::get().append_standard( m_standard_c99 );
	m_standard_c11.initialize();	service::get().append_standard( m_standard_c11 );
	m_standard_c23.initialize();	service::get().append_standard( m_standard_c23 );
	m_standard_cpp98.initialize();	service::get().append_standard( m_standard_cpp98 );
	m_standard_cpp11.initialize();	service::get().append_standard( m_standard_cpp11 );
	m_standard_cpp14.initialize();	service::get().append_standard( m_standard_cpp14 );
	m_standard_cpp17.initialize();	service::get().append_standard( m_standard_cpp17 );
	m_standard_cpp20.initialize();	service::get().append_standard( m_standard_cpp20 );
	m_standard_cpp23.initialize();	service::get().append_standard( m_standard_cpp23 );
	m_standard_comext.initialize();	service::get().append_standard( m_standard_comext );
	m_standard_gcc.initialize();	service::get().append_standard( m_standard_gcc );
	m_standard_clang.initialize();	service::get().append_standard( m_standard_clang );
	m_standard_msvc.initialize();	service::get().append_standard( m_standard_msvc );

	m_syntax_f1pp.initialize();		service::get().append_syntax( m_syntax_f1pp );

	service::get().set_default_standard( &m_standard_comext );

	// Initialize command line
	create_switch_group( m_group_general,	"General" );
		m_group_general->append_switch( m_switch_debug );
		m_group_general->append_switch( m_switch_syntax_check_only );
		m_group_general->append_switch( m_switch_timings );
	create_switch_group( m_group_include,	"Includes" );
		m_group_include->append_switch( m_switch_auto_once_includes );
		m_group_include->append_switch( m_switch_base_dir_allow );
		m_group_include->append_switch( m_switch_base_dir_disallow );
		m_group_include->append_switch( m_switch_deprecate_include );
		m_group_include->append_switch( m_switch_deprecate_system_include );
		m_group_include->append_switch( m_switch_include );
		m_group_include->append_switch( m_switch_include_dir );
		m_group_include->append_switch( m_switch_system_include_base );
		m_group_include->append_switch( m_switch_system_include_dir );
		m_group_include->append_switch( m_switch_user_include_dir );
	create_switch_group( m_group_input,		"Input" );
		m_group_input->append_switch( m_switch_atsigns_in_identifiers );
		m_group_input->append_switch( m_switch_character_encoding );
		m_group_input->append_switch( m_switch_digraphs );
		m_group_input->append_switch( m_switch_directive );
		m_group_input->append_switch( m_switch_dollars_in_identifiers );
		m_group_input->append_switch( m_switch_endianness );
		m_group_input->append_switch( m_switch_input_encoding );
		m_group_input->append_switch( m_switch_long_wide_char );
		m_group_input->append_switch( m_switch_known_pragma );
		m_group_input->append_switch( m_switch_standard );
		m_group_input->append_switch( m_switch_trigraphs );
		m_group_input->append_switch( m_switch_unsigned_char );
	create_switch_group( m_group_macros,	"Macros" );
		m_group_macros->append_switch( m_switch_define );
		m_group_macros->append_switch( m_switch_ignore_macros );
		m_group_macros->append_switch( m_switch_macro );
		m_group_macros->append_switch( m_switch_undefine );
	create_switch_group( m_group_messages,	"Messages" );
		m_group_messages->append_switch( m_switch_dump_stack_on_error );
		m_group_messages->append_switch( m_switch_dump_stack_on_warning );
		m_group_messages->append_switch( m_switch_dump_macros_on_error );
		m_group_messages->append_switch( m_switch_dump_macros_on_warning );
		m_group_messages->append_switch( m_switch_export_messages );
		m_group_messages->append_switch( m_switch_full_paths );
		m_group_messages->append_switch( m_switch_import_messages );
		m_group_messages->append_switch( m_switch_message );
		m_group_messages->append_switch( m_switch_message_filename_base );
		m_group_messages->append_switch( m_switch_message_filename_only );
		m_group_messages->append_switch( m_switch_message_severity );
	create_switch_group( m_group_output,	"Output" );
		m_group_output->append_switch( m_switch_disable_output );
		m_group_output->append_switch( m_switch_dump );
		m_group_output->append_switch( m_switch_newline );
		m_group_output->append_switch( m_switch_output_filename );
		m_group_output->append_switch( m_switch_output_filename_base );
		m_group_output->append_switch( m_switch_output_filename_only );
		m_group_output->append_switch( m_switch_keep_comments );
		m_group_output->append_switch( m_switch_keep_doxygen_comments );
		m_group_output->append_switch( m_switch_merge_strings );
		m_group_output->append_switch( m_switch_print_filename );
		m_group_output->append_switch( m_switch_output_encoding );
		m_group_output->append_switch( m_switch_output_format );
		m_group_output->append_switch( m_switch_output_full_paths );
		m_group_output->append_switch( m_switch_write_bom );

	set_cmdline_parser( &m_cmdline_parser );
	set_switches( args, countof(args) );

	// Exit
	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::initialize_application() noexcept
{
	status_t status;

	super::initialize_application();

	if( m_context.createT<pp::context>().failed_status( status ) )
		return status;
	if( m_context->initialize().failed_status( status ) )
		return status;
//	m_context->set_frontend_context( &m_frontend_context );

	// Select default output
	core::Tref<pp::Ioutput> output;
	__debugbreak_if( service::get().create_output( output, m_default_output_class_guid ).failed_status( status ) )
		return status;
	m_context->set_output( output );

	// Exit
	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::uninitialize_application() noexcept
{
	m_context = nullptr;
	f1::fs::service::get().remove_all_file_systems();
	f1::msgout::service::get().try_delete();

	return super::uninitialize_application();
}
status_t f1::pp::frontend::f1pp::application::run_application() noexcept
{
	if( !get_flags( fl_fatal_error ) )
	{
		if( 0 == m_files_processed && !get_flags( fl_no_help_needed ) )
			print_usage();
	}

	if( 0 != m_total_time )
		printf( "Total time: %g msec\n", m_total_time * 1000.0 / CLOCKS_PER_SEC );

	return STATUS_SUCCESS;
}

status_t f1::pp::frontend::f1pp::application::on_filename( const f1::core::const_string & filename ) noexcept
{
	status_t status;

	// Check current state
	__debugbreak_if( filename.is_empty() )
		return STATUS_ERROR_INVALID_PARAMETER;
	__debugbreak_if( nullptr == m_context )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Set up switches
		// Gneral
		m_switch_syntax_check_only.apply( *m_context );
		// Include
//		m_switch_auto_once_includes.apply( *m_context );
		m_switch_deprecate_include.apply( *m_context );
		m_switch_deprecate_system_include.apply( *m_context );
//		m_switch_include.apply( *m_context );
//		m_switch_include_dir.apply( *m_context );
		// Input
//		m_switch_digraphs.apply( *m_context );
//		m_switch_directive.apply( *m_context );
//		m_switch_endianness.apply( *m_context );
//		m_switch_input_encoding.apply( *m_context );
//		m_switch_standard.apply( *m_context );
//		m_switch_trigraphs.apply( *m_context );
//		m_switch_unsigned_char.apply( *m_context );
		// Macros
//		m_switch_define.apply( *m_context );
//		m_switch_macro.apply( *m_context );
//		m_switch_undefine.apply( *m_context );
		// Messages
//		m_switch_full_paths.apply( *m_context );
		// Output
//		m_switch_output_encoding.apply( *m_context );
//		m_switch_output_format.apply( *m_context );
		m_switch_print_filename.apply( *m_context );
		m_switch_merge_strings.apply( *m_context );

	// Perform all defines and undefines
/*	array_foreach( core::Tarray<pp::context::define_op_desc_t>::const_iterator, it, m_define_ops )
	{
		const pp::context::define_op_desc_t & op = *it;
		if( op.m_is_define )
			m_context->append_macro( op.m_name.data(), op.m_name.length(), op.m_value.data(), op.m_value.length() );
		else
			m_context->remove_macro( *it );
	}
*/
	// Initialize message output
	m_context->get_msgout_context().append_output( f1::msgout::console_output::m_guid );
	m_context->get_msgout_context().set_formatter( f1::msgout::msvs_formatter::m_guid );

	// Check if the input file exists
	core::string input_file( filename.data(), filename.get_size() );
	{
	f1::io::file_istream inp( input_file.c_str() );
	if( inp.bad() )
	{
		fatal_error( "file not found: \"%s\"", input_file.c_str() );
		return status;
	}
	}

	// Set up input
	if( m_context->append_source( input_file.c_str() ).failed_status( status ) )
	{
		error( "cannot process file: %s", core::string( filename.data(), filename.get_size() ).c_str() );
		return status;
	}

	// Select requested output
	if( nullptr == m_context->get_output() )
	{
		core::Tref<pp::Ioutput> output;
		__debugbreak_if( service::get().create_output( output, m_default_output_class_guid ).failed_status( status ) )
		{
			fatal_error( "cannot create selected output class instance" );
			return status;
		}
		m_context->set_output( output );
	}

	// Set up event handler
	m_context->append_event_handler( pp::context::event_id_t::ev_status_text_changed, m_pp_context_event_handler );

	// Set up storage
	core::string output_file;
	m_context->make_output_file_name( output_file, ".ii", input_file.c_str(), input_file.length() );

	core::Tref<pp::Istorage> stg = nullptr;
	if( m_context->create_storage( stg, output_file ).failed_status( status ) )
	{
		fatal_error( "cannot create output file: \"%s\"", output_file.c_str() );
		return status;
	}
	__debugbreak_if( nullptr == stg )
		return STATUS_ERROR_NOT_INITIALIZED;
	m_context->set_storage( stg );

	// Set up output
	core::Tref<pp::Ioutput> outp = m_context->get_output();
	__debugbreak_if( nullptr == outp )
		return STATUS_ERROR_NOT_INITIALIZED;

	outp->begin_output();

	// Process file
	clock_t start_time = 0;
	if( m_switch_timings.m_value )
		start_time = clock();

	status = m_context->process();

	++ m_files_processed;
	if( m_switch_timings.m_value )
	{
		clock_t diff = clock() - start_time + 1;
		m_total_time += diff;
		printf( "Time: %g msec\n", diff * 1000.0 / CLOCKS_PER_SEC );
	}

//	if( status.failed() )
//		error( "cannot parse \"%s\"", input_file.c_str() ); 

	// Clean up
	if( nullptr != outp )
		outp->end_output();

	m_context->remove_all_event_handlers( m_pp_context_event_handler );

	// Exit
	return status;
}
status_t f1::pp::frontend::f1pp::application::on_unrecognized_switch( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & param ) noexcept
{

	array_foreach( syntax_list_t::const_iterator, it, service::get().get_syntax_list() )
	{
		if( STATUS_SUCCESS == (*it).ref_const_cast()->process_command_line_switch( switchar, core::static_string(name), core::static_string(param) ) )
			return STATUS_SUCCESS;
	}

	array_foreach( standard_list_t::const_iterator, it, service::get().get_standard_list() )
	{
		if( STATUS_SUCCESS == (*it).ref_const_cast()->process_command_line_switch( switchar, core::static_string(name), core::static_string(param) ) )
			return STATUS_SUCCESS;
	}

	return super::on_unrecognized_switch( switchar, name, param );
}

void f1::pp::frontend::f1pp::application::pp_context_event_handler::invoke( pp::context::event_id_t event_id, void * param )
{
	switch( event_id )
	{
	case pp::context::event_id_t::ev_status_text_changed:{
		const pp::string & title = *reinterpret_cast<const pp::string *>( param );
		SetConsoleTitle( title.c_str() );
		}break;
	default:
		DBG_FAIL();
		break;
	}
}

/*END OF application.cxx*/
