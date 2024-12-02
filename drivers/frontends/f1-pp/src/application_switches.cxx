/*application_switches.cxx*/
#include "pch.h"
#include "main.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common switches

status_t f1::pp::frontend::f1pp::application::switch_help( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	print_usage();

	UNREFERENCED_PARAMETER(switchar);
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(rest);
	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::switch_nologo( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( '-' == switchar )
		CLR_FLAG(super::m_flags, fl_header_printed);
	else
		SET_FLAG(super::m_flags, fl_header_printed);

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::switch_quiet( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( _T('-') == switchar )
		CLR_FLAG(super::m_flags, fl_quiet_mode);
	else
	{
		SET_FLAG(super::m_flags, fl_quiet_mode);
		CLR_FLAG(super::m_flags, fl_verbose_mode);
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );

	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::switch_ver( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	core::version ver;
	status_t status = get_version( ver );
	if( status.succeeded() )
		message( _T("%d.%d.%d.%d\n%hs\n"), ver.m_major, ver.m_minor, ver.m_sub, ver.m_build, __DATE__ );

	SET_FLAG(super::m_flags, fl_no_help_needed);

	UNREFERENCED_PARAMETER(switchar);
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(rest);
	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::switch_verbose( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( _T('-') == switchar )
		CLR_FLAG(super::m_flags, fl_verbose_mode);
	else
	{
		SET_FLAG(super::m_flags, fl_verbose_mode);
		CLR_FLAG(super::m_flags, fl_quiet_mode);
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );

	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Preproessor switches

status_t f1::pp::frontend::f1pp::application::switch_comments( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	// TODO

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}

/*status_t f1::pp::frontend::f1pp::application::switch_define( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected macro name"), switchar, core::string(name).c_str() );

	else
	{
		size_t len = name.length();
		const char * start = name.data();
		const char * end = start + len;
		const char * pos = _strnchr( start, len, '=' );

		if( '+' == switchar )
		{
			if( nullcstr == pos )
				m_context->append_macro( start, len );
			else
				m_context->append_macro( start, pos - start, pos + 1, end - pos );
		}
		else
			m_context->get_macros_to_undefine().remove( core::string(start, len) );
	}

	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::switch_undefine( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected macro name"), switchar, core::string(name).c_str() );
	else
	{
		if( '+' == switchar )
			m_context->get_macros_to_undefine().append( core::string(rest) );
		else
			m_context->get_macros_to_undefine().remove( core::string(rest) );
	}

	return STATUS_SUCCESS;
}*/

/*status_t f1::pp::frontend::f1pp::application::switch_endianness( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	core::string value( rest );
	if( value.equal_nocase( "le" ) || value.equal_nocase( "little" ) )
	{
		m_context->modify_flags( pp::context::fl_little_endian, pp::context::fl_big_endian );
	}
	else if( value.equal_nocase( "be" ) || value.equal_nocase( "big" ) )
	{
		m_context->modify_flags( pp::context::fl_big_endian, pp::context::fl_little_endian );
	}
	else
		fatal_error( _T("%c%s: expected LE, little or BE, big"), switchar, core::string(name).c_str() );

	return STATUS_SUCCESS;
}*/
status_t f1::pp::frontend::f1pp::application::parse_encoding( core::Tconst_ref<nls::Iencoding> & enc, const char * name, size_t len )
{
	status_t status;

	if( nls::service::get().find_encoding( enc, name, len ).failed() )
	{
		core::Tconst_ref<nls::Icodepage> cp;
		if( nls::service::get().find_codepage( cp, name, len ).failed_status( status ) )
			return status;

		core::Tref<nls::encoding_codepage> enc_cp;
		enc_cp.createT<nls::encoding_codepage>();
		enc_cp->set_codepage( cp );

		enc = enc_cp;
	}

	return STATUS_SUCCESS;
}
/*status_t f1::pp::frontend::f1pp::application::switch_input_encoding( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected encoding or codepage name"), switchar, core::string(name).c_str() );
	else
	{
		core::Tconst_ref<nls::Iencoding> enc;
		if( parse_encoding( enc, rest.data(), rest.length() ).failed() )
			fatal_error( _T("%c%s: expected encoding or codepage name"), switchar, core::string(name).c_str() );
		else
			m_context->set_default_input_encoding( enc );
	}

	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::switch_output_encoding( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected encoding or codepage name"), switchar, core::string(name).c_str() );
	else
	{
		core::Tconst_ref<nls::Iencoding> enc;
		if( parse_encoding( enc, name.data(), name.length() ).failed() )
			fatal_error( _T("%c%s: expected encoding or codepage name"), switchar, core::string(name).c_str() );
		else
			m_context->set_default_output_encoding( enc );
	}

	return STATUS_SUCCESS;
}*/

/*status_t f1::pp::frontend::f1pp::application::switch_full_paths( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( _T('-') == switchar )
		m_context->modify_flags( 0, pp::context::fl_output_full_path );
	else
		m_context->modify_flags( pp::context::fl_output_full_path );

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}*/

/*status_t f1::pp::frontend::f1pp::application::switch_include( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected include file name"), switchar, core::string(name).c_str() );
	else
	{
		if( '+' == switchar )
			m_context->get_include_files().append( core::string(rest) );
		else
			m_context->get_include_files().remove( core::string(rest) );
	}

	return STATUS_SUCCESS;
}*/

/*status_t f1::pp::frontend::f1pp::application::switch_include_dir( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected directory name"), switchar, core::string(name).c_str() );
	else
	{
		if( '+' == switchar )
			m_context->get_include_dirs().append_search_path( rest );
		else
			m_context->get_include_dirs().remove_search_path( rest );
	}

	return STATUS_SUCCESS;
}*/

status_t f1::pp::frontend::f1pp::application::switch_out( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected output file name"), switchar, core::string(name).c_str() );
	else
		m_context->set_output_file_name( rest.data(), rest.length() );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::switch_outdir( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected output file name"), switchar, core::string(name).c_str() );
	else
		m_context->set_output_directory( rest.data(), rest.length() );
	return STATUS_SUCCESS;
}

status_t f1::pp::frontend::f1pp::application::switch_compress_whitespace( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	core::Tref<pp::Ioutput> output = m_context->get_output();
	__debugbreak_ifnot( nullptr != output )
	{
		if( _T('-') == switchar )
			output->modify_flags( Ioutput::output_flags_t::null, Ioutput::output_flags_t::fl_compress_whitespace );
		else
			output->modify_flags( Ioutput::output_flags_t::fl_compress_whitespace );
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}
/*status_t f1::pp::frontend::f1pp::application::switch_keep_comments( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( _T('-') == switchar )
		m_context->append_parser_parameter( c_pp::parser::m_guid_clear_c_flags, uintmax_t(c_pp::parser::c_flags_t::fl_keep_comments) );
	else
		m_context->append_parser_parameter( c_pp::parser::m_guid_set_c_flags, uintmax_t(c_pp::parser::c_flags_t::fl_keep_comments) );

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}*/
status_t f1::pp::frontend::f1pp::application::switch_nested_comments( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( _T('-') != switchar )
		application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) c_pp::parser::modify_c_flags_parser_command(
			c_pp::parser::c_flags_t::fl_nested_comments ) );
	else
		application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) c_pp::parser::modify_c_flags_parser_command(
			c_pp::parser::c_flags_t::null, c_pp::parser::c_flags_t::fl_nested_comments ) );

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::switch_maximum_char_len( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected integer value"), switchar, core::string(name).c_str() );
	else
	{
		const char * ep;
		unsigned long n = _strntoul( rest.data(), rest.length(), &ep, 0 );
		if( ep != rest.data_end() )
			fatal_error( _T("%c%s: unexpected characters after integer"), switchar, core::string(name).c_str() );
		else
			m_context->set_limit_value( limit_t::li_maximum_charconst_length, intmax_t(n) );
	}
	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::switch_maximum_ident_len( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected integer value"), switchar, core::string(name).c_str() );
	else
	{
		const char * ep;
		unsigned long n = _strntoul( rest.data(), rest.length(), &ep, 0 );
		if( ep != rest.data_end() )
			fatal_error( _T("%c%s: unexpected characters after integer"), switchar, core::string(name).c_str() );
		else
			m_context->set_limit_value( limit_t::li_maximum_identifier_length, intmax_t(n) );
	}
	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::switch_maximum_iterations( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected integer value"), switchar, core::string(name).c_str() );
	else
	{
		const char * ep;
		unsigned long n = _strntoul( rest.data(), rest.length(), &ep, 0 );
		if( ep != rest.data_end() )
			fatal_error( _T("%c%s: unexpected characters after integer"), switchar, core::string(name).c_str() );
		else
			m_context->set_limit_value( limit_t::li_maximum_iteration_count, intmax_t(n) );
	}
	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::switch_maximum_recursion( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected integer value"), switchar, core::string(name).c_str() );
	else
	{
		const char * ep;
		unsigned long n = _strntoul( rest.data(), rest.length(), &ep, 0 );
		if( ep != rest.data_end() )
			fatal_error( _T("%c%s: unexpected characters after integer"), switchar, core::string(name).c_str() );
		else
			m_context->set_limit_value( limit_t::li_maximum_recursion_depth, intmax_t(n) );
	}
	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::switch_maximum_errors( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected integer value"), switchar, core::string(name).c_str() );
	else
	{
		const char * ep;
		unsigned long n = _strntoul( rest.data(), rest.length(), &ep, 0 );
		if( ep != rest.data_end() )
			fatal_error( _T("%c%s: unexpected characters after integer"), switchar, core::string(name).c_str() );
		else
			m_context->set_limit_value( limit_t::li_error_maximum_count, intmax_t(n) );
	}
	return STATUS_SUCCESS;
}
status_t f1::pp::frontend::f1pp::application::switch_maximum_warnings( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected integer value"), switchar, core::string(name).c_str() );
	else
	{
		const char * ep;
		unsigned long n = _strntoul( rest.data(), rest.length(), &ep, 0 );
		if( ep != rest.data_end() )
			fatal_error( _T("%c%s: unexpected characters after integer"), switchar, core::string(name).c_str() );
		else
			m_context->set_limit_value( limit_t::li_warning_maximum_count, intmax_t(n) );
	}
	return STATUS_SUCCESS;
}

/*END OF application_switches.cxx*/
