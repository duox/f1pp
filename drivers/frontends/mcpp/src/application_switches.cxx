/*args.cpp*/
#include "pch.h"
#include "main.h"


status_t f1::pp::frontends::mcpp::application::switch_comments( char_t switchar, pcsz_t name, pcsz_t rest )
{

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}

status_t f1::pp::frontends::mcpp::application::switch_define( char_t switchar, pcsz_t name, pcsz_t rest )
{

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}

status_t f1::pp::frontends::mcpp::application::switch_encoding( char_t switchar, pcsz_t name, pcsz_t rest )
{

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}

status_t f1::pp::frontends::mcpp::application::switch_include( char_t switchar, pcsz_t name, pcsz_t rest )
{

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}

#if 0
status_t f1::pp::frontends::mcpp::application::switch_add( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( !DBG_VERIFY_INPLACE( NULL != m_context ) )
	{
		error( _T("internal error (%e): %08x"), __LINE__, STATUS_ERROR_NOT_INITIALIZED );
		return STATUS_ERROR_NOT_INITIALIZED;
	}
	status_t status = m_context->append_module( name );	//m_modules_to_append.push_back( name );
	if( FAILED(status) )
	{
		error( _T("cannot append module: %08x"), status );
		return status;
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_delete( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( !DBG_VERIFY_INPLACE( NULL != m_context ) )
	{
		error( _T("internal error (%e): %08x"), __LINE__, STATUS_ERROR_NOT_INITIALIZED );
		return STATUS_ERROR_NOT_INITIALIZED;
	}
	status_t status = m_context->delete_symbol( name );	//m_symbols_to_delete.push_back( name )
	if( FAILED(status) )
	{
		error( _T("cannot delete symbol: %08x"), status );
		return status;
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_dump( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( !DBG_VERIFY_INPLACE( NULL != m_context ) )
	{
		error( _T("internal error (%e): %08x"), __LINE__, STATUS_ERROR_NOT_INITIALIZED );
		return STATUS_ERROR_NOT_INITIALIZED;
	}
	status_t status = m_context->dump_module( name );	//m_modules_to_dump.push_back( name );
	if( FAILED(status) )
	{
		error( _T("cannot dump module: %08x"), status );
		return status;
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_export( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( !DBG_VERIFY_INPLACE( NULL != m_context ) )
	{
		error( _T("internal error (%e): %08x"), __LINE__, STATUS_ERROR_NOT_INITIALIZED );
		return STATUS_ERROR_NOT_INITIALIZED;
	}
	status_t status = m_context->export_symbol( name );	//m_symbols_to_export.push_back( name );
	if( FAILED(status) )
	{
		error( _T("cannot export symbol: %08x"), status );
		return status;
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_extract( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( !DBG_VERIFY_INPLACE( NULL != m_context ) )
	{
		error( _T("internal error (%e): %08x"), __LINE__, STATUS_ERROR_NOT_INITIALIZED );
		return STATUS_ERROR_NOT_INITIALIZED;
	}
	status_t status = m_context->extract_module( name );	//m_modules_to_extract.push_back( name );
	if( FAILED(status) )
	{
		error( _T("cannot extract module: %08x"), status );
		return status;
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_list( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( !DBG_VERIFY_INPLACE( NULL != m_context ) )
	{
		error( _T("internal error (%e): %08x"), __LINE__, STATUS_ERROR_NOT_INITIALIZED );
		return STATUS_ERROR_NOT_INITIALIZED;
	}
	status_t status = m_context->list_symbols( name );	//m_symbols_to_list.push_back( name );
	if( FAILED(status) )
	{
		error( _T("cannot list symbol(s): %08x"), status );
		return status;
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_modify( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( !DBG_VERIFY_INPLACE( NULL != m_context ) )
	{
		error( _T("internal error (%e): %08x"), __LINE__, STATUS_ERROR_NOT_INITIALIZED );
		return STATUS_ERROR_NOT_INITIALIZED;
	}
	status_t status = m_context->modify_module( name );	//m_modules_to_modify.push_back( name );
	if( FAILED(status) )
	{
		error( _T("cannot modify module: %08x"), status );
		return status;
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_out( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected output file name"), switchar, name );
	else
		m_context->set_output_file_name( rest );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_output( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected output file name"), switchar, name );
	else
		m_context->set_output_file_name( rest );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_outdir( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( nullchar == *rest )
		fatal_error( _T("%c%s: expected output file name"), switchar, name );
	else
		m_context->set_output_directory( rest );

	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_remove( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( !DBG_VERIFY_INPLACE( NULL != m_context ) )
	{
		error( _T("internal error (%e): %08x"), __LINE__, STATUS_ERROR_NOT_INITIALIZED );
		return STATUS_ERROR_NOT_INITIALIZED;
	}
	status_t status = m_context->remove_module( name );	//m_modules_to_remove.push_back( name );
	if( FAILED(status) )
	{
		error( _T("cannot remove module: %08x"), status );
		return status;
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_replace( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( !DBG_VERIFY_INPLACE( NULL != m_context ) )
	{
		error( _T("internal error (%e): %08x"), __LINE__, STATUS_ERROR_NOT_INITIALIZED );
		return STATUS_ERROR_NOT_INITIALIZED;
	}
	status_t status = m_context->remove_module( name );	//m_symbols_to_replace.push_back( name );
	if( FAILED(status) )
	{
		error( _T("cannot remove module: %08x"), status );
		return status;
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}


status_t f1::pp::frontends::mcpp::application::switch_backup( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( _T('-') == *rest )
		m_context->enable_backup( false );
	else
	{
		if( nullchar != *rest )
			m_context->set_backup_file_name( rest );
		m_context->enable_backup( true );
	}
	UNREFERENCED_PARAMETER( name );
	UNREFERENCED_PARAMETER( switchar );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_case( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( _T('-') == *rest )
		m_context->modify_flags( 0, context::flCaseSensitive );
	else if( nullchar == *rest || _T('+') == *rest )
		m_context->modify_flags( context::flCaseSensitive );
	else
	{
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	}
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_date( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( !_tcsicmp( rest, _T("CURRENT") ) )
		m_context->set_date_mode( context::LIB_DATE_CURRENT );
	else if( !_tcsicmp( rest, _T("FILE") ) )
		m_context->set_date_mode( context::LIB_DATE_FILE );
	else if( !_tcsicmp( rest, _T("NONE") ) )
		m_context->set_date_mode( context::LIB_DATE_NONE );
	else
	{
		fatal_error( _T("%c%s: expected one of CURRENT, FILE or NONE"), switchar, name );
	}
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_extended( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( _T('-') == *rest )
		m_context->modify_flags( 0, context::flExtended );
	else if( nullchar == *rest || _T('+') == *rest )
		m_context->modify_flags( context::flExtended );
	else
	{
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	}
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_newer( char_t switchar, pcsz_t name, pcsz_t rest )
{
	m_context->modify_flags( context::flReplaceOlder );
	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_pagesize( char_t switchar, pcsz_t name, pcsz_t rest )
{
	size_t page_size;
	psz_t ep;

	page_size = _tcstoul( rest, &ep, 0 );

	if( nullchar != *ep )
	{
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
		return STATUS_SUCCESS;
	}

	m_context->set_page_size( page_size );

	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_purge( char_t switchar, pcsz_t name, pcsz_t rest )
{
	bool disable;
	pcsz_t s = rest;
	bitflags_t flags;

	for(;;)
	{
		disable = false;
		if( _T('-') == *s )
		{
			disable = true;
			++ s;
		}

		if( !_tcsicmp( s, _T("COMMENTS") ) )
		{
			flags = context::flStripComments;
			s += 8;
		}
		else if( !_tcsicmp( s, _T("LOCALS") ) )
		{
			flags = context::flStripLocals;
			s += 6;
		}
		else
		{
			fatal_error( _T("%c%s: expected one of COMMENTS or LOCALS"), switchar, name );
			break;
		}

		if( _T('-') == *s )
		{
			disable = true;
			++ s;
		}

		if( disable )
			m_context->modify_flags( 0, flags );
		else
			m_context->modify_flags( flags, 0 );

		if( nullchar == *s )
			break;
		if( _T(',') == *s )
			++ s;
		else
		{
			fatal_error( _T("%c%s: unexpected characters after end of switch"), switchar, name );
			break;
		}
		if( nullchar == *s )
			break;
	}
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_short( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( nullchar != *rest )
	{
		fatal_error( _T("%c%s: unexpected characters after end of switch"), switchar, name );
		return STATUS_SUCCESS;
	}

	m_context->modify_flags( context::flShortMembers );

	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_strip( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( nullchar != *rest )
	{
		fatal_error( _T("%c%s: unexpected characters after end of switch"), switchar, name );
		return STATUS_SUCCESS;
	}

	m_context->modify_flags( context::flStripCppNames );

	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_unmangle( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( nullchar != *rest )
	{
		fatal_error( _T("%c%s: unexpected characters after end of switch"), switchar, name );
		return STATUS_SUCCESS;
	}

	m_context->modify_flags( context::flUnmangleCppNames );

	return STATUS_SUCCESS;
}


status_t f1::pp::frontends::mcpp::application::switch_help( char_t switchar, pcsz_t name, pcsz_t rest )
{
	print_usage();

	UNREFERENCED_PARAMETER(switchar);
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(rest);
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_nologo( char_t switchar, pcsz_t name, pcsz_t rest )
{
	SET_FLAG(m_message_flags, MF_HEADER_PRINTED);
	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_quiet( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( _T('-') == switchar )
		CLR_FLAG(m_message_flags, MF_QUIET_MODE);
	else
	{
		SET_FLAG(m_message_flags, MF_QUIET_MODE);
		CLR_FLAG(m_message_flags, MF_VERBOSE_MODE);
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );

	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_ver( char_t switchar, pcsz_t name, pcsz_t rest )
{
	int major = 0, minor = 0;
	status_t status = get_version( major, minor );
	if( SUCCEEDED(status) )
		message( _T("%d.%d\n%hs"), major, minor, __DATE__ );
	UNREFERENCED_PARAMETER(switchar);
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(rest);
	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_verbose( char_t switchar, pcsz_t name, pcsz_t rest )
{
	if( _T('-') == switchar )
		CLR_FLAG(m_message_flags, MF_VERBOSE_MODE);
	else
	{
		SET_FLAG(m_message_flags, MF_VERBOSE_MODE);
		CLR_FLAG(m_message_flags, MF_QUIET_MODE);
	}

	if( nullchar != *rest )
		fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, name );

	return STATUS_SUCCESS;
}
status_t f1::pp::frontends::mcpp::application::switch_warn( char_t switchar, pcsz_t name, pcsz_t rest )
{
	bool disable = false;
	unsigned number;
	char_t * ep = nullptr;

	for( pcsz_t s = rest; ; )
	{
		if( _T('-') == *s )
			disable = true;
		else if( !_istdigit( *s ) )
		{
			fatal_error( _T("%c%s: expected warning number"), switchar, name );
			break;
		}

		number = _tcstoul( rest, &ep, 10 );
		if( s == ep )
			break;

		if( _T('-') == *s )
		{
			disable = true;
			++ s;
		}

		//enable_warning( number, disable );	// TODO

		if( nullchar == *s )
			break;
		if( _T(',') != *s )
		{
			fatal_error( _T("%c%s: expected warning number"), switchar, name );
			++ s;
			break;
		}
	}

	return STATUS_SUCCESS;
}
#endif
/*END OF args.cpp*/
