/*exec_directive.cxx*/
#include "../pch.h"
#include "../main.h"

// {257C00F8-AE89-445D-A1FF-3BC9F0C9227C}
//f1::guid_t f1::f1pp::exec_directive::m_exec_attribute_class_guid =
//{ 0x257c00f8, 0xae89, 0x445d, { 0xa1, 0xff, 0x3b, 0xc9, 0xf0, 0xc9, 0x22, 0x7c } };
// {D523EB55-1D61-4A2A-B3CE-212D7F69811F}
//f1::guid_t f1::f1pp::exec_directive::m_soft_attribute_class_guid =
//{ 0xd523eb55, 0x1d61, 0x4a2a, { 0xb3, 0xce, 0x21, 0x2d, 0x7f, 0x69, 0x81, 0x1f } };

f1::f1pp::exec_directive::exec_directive( unsigned flags ):
	super( flags ),
	m_soft( false ), m_raw( false )
{
//	append_attribute_class( m_exec_attribute_class_guid,		pp::string("exec") );
//	append_attribute_class( m_soft_attribute_class_guid,	pp::string("soft") );
}

// Properties
const char * f1::f1pp::exec_directive::get_name() const
{
	return "exec";
}
// {4E85423D-E86F-4280-AE5E-90FDB8C8C62D}
const f1::guid_t f1::f1pp::exec_directive::m_guid =
{ 0x4e85423d, 0xe86f, 0x4280, { 0xae, 0x5e, 0x90, 0xfd, 0xb8, 0xc8, 0xc6, 0x2d } };
const f1::guid_t & f1::f1pp::exec_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::exec_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

// Operations
const status_t f1::f1pp::exec_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	m_soft = f1par->exists_directive_attribute( "soft" );
	m_raw  = f1par->exists_directive_attribute( "raw" );

	// Fetch macro identifier, type and equal sign
	core::Tref<pp::Imacro>		mac;
	m_append = f1par->exists_directive_attribute( "append" );
	f1par->parse_identifier_and_type( mac, &m_append );
	m_macro = mac;

	// Check whether is it a multi-line exec
	f1par->skip_real_spaces_and_comments();
	if( par->is_nl() )
	{
		frame * frm = new(std::nothrow) frame( *f1par, *this );
		__debugbreak_if( nullptr == frm )
			return STATUS_ERROR_INTERNAL;
		frm->m_output.initialize();
		frm->m_storage.initialize( *parser.get_context() );
		f1par->modify_c_flags(
			//f1::c_pp::parser::c_flags_t::fl_execute_exec_top_only
			f1::c_pp::parser::c_flags_t::null,
			f1::c_pp::parser::c_flags_t::null,
			&frm->m_old_c_flags
			);
		parser.set_output( &frm->m_output, &frm->m_old_output );
		parser.set_storage( &frm->m_storage, &frm->m_old_storage );
		parser.enter_directive( *frm );
	}
	else
	{
		// Go to expanding macros
		f1par->capture_line( line_capture_callback, this );
	}

	// Exit
	return STATUS_SUCCESS;
}

f1::pp::Iframe * f1::f1pp::exec_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return nullptr;
	return new(std::nothrow) frame( *f1par, *this );
}

f1::f1pp::exec_directive::frame::frame( f1pp::parser & par, exec_directive & drct ):
	super( par ),
	m_exec_directive( &drct )
{
}
const char * f1::f1pp::exec_directive::frame::get_openning_directive_name() const
{
	return "exec";
}
const char * f1::f1pp::exec_directive::frame::get_closing_directive_name() const
{
	return "endexec";
}
const f1::guid_t & f1::f1pp::exec_directive::frame::get_openning_directive_guid() const
{
	return exec_directive::m_guid;
}
const f1::guid_t & f1::f1pp::exec_directive::frame::get_closing_directive_guid() const
{
	return endexec_directive::m_guid;
}

const status_t f1::f1pp::exec_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	return execute( parser, pp::string( out_buf ) );
}
const status_t f1::f1pp::exec_directive::execute( f1::pp::Iparser & parser, const pp::string & out_buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_INVALID_TYPE;

	// Parse buffer
	if( 0 == out_buf.size() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_FILENAME );
		return STATUS_SUCCESS;
	}

	// Dequotize string if required (this involves exact un-stringification)
	pp::string val;
	if( f1par->unstringify_chars( val, out_buf ).failed_status( status ) )
		return status;

	// Launch all processes
	if( execute_commands( *f1par, val ).failed_status( status ) )
		return status;

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::exec_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::f1pp::exec_directive> drc = type_cast<f1::f1pp::exec_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::f1pp::exec_directive *>( param )->execute( parser, buf );
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
const status_t f1::f1pp::exec_directive::get_macro_value( pp::Imacro & mac, pp::variant & value, pp::variant::type_t ty )
{
	value = mac.get_value_variant();
	if( pp::variant::ty_none != ty && value.get_type() != ty )
		value.set_type( ty );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::exec_directive::set_macro_value( pp::Imacro & mac, const pp::variant & value )
{
	core::string	str;
	switch( value.get_type() )
	{
	case pp::variant::ty_none:
		break;	// leave str empty
	case pp::variant::ty_bool:
	case pp::variant::ty_int:
		str.format( "%d", value.as_int() );
		break;
	case pp::variant::ty_uint:
		str.format( "%u", value.as_uint() );
		break;
	case pp::variant::ty_float:
		str.format( "%f", value.as_float() );
		break;
	case pp::variant::ty_string:
		str.format( "\"%s\"", value.as_string() );
		break;
	case pp::variant::ty_text:
		str.format( "%s", value.as_string() );
		break;
	default:
		__assert( false );
		return STATUS_ERROR_INVALID_TYPE;
	}

	// Set the value
	pp::string	str_value = mac.get_value();
	if( pp::variant::ty_string == value.get_type() || pp::variant::ty_string == value.get_type() )
		;
	str_value.add_part( str );
	mac.set_value_variant( value, str_value );

	// Exit
	return STATUS_SUCCESS;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::f1pp::exec_directive::execute_commands(
	f1::f1pp::parser & f1par, const pp::string & cmd )
{
	status_t status;

	// Launch all processes
	const char * s = cmd.data();
	const char * const end_s = cmd.data_end();
	pp::string buf;
	while( s < end_s )
	{
		// Skip to new line or end of string
		while( s < end_s && isspace( *s ) )
			++ s;
		const char * const start = s;
		while( s < end_s && '\n' != *s && '\r' != *s )
			++ s;
		if( s == start )
			continue;

		// Create the process
		if( create_process_and_wait( f1par, nullcstr, core::string( start, s - start ).c_str(), buf ).failed_status( status ) )
			return status;
		//if( STATUS_WARNING_ERROR_ISSUED == status )
		if( STATUS_SUCCESS != status )
			return STATUS_SUCCESS;
	}

	if( nullptr != m_macro )
	{
		if( !m_append )
			m_macro->set_value( buf );		// reset macro value
		else
			m_macro->append_value( buf );	// modify macro value
	}
	else
	{
		if( m_raw )
			f1par.output( buf );
		else
		{
			//f1par.begin_subsection();
			f1par.get_parser()->insert_patch( buf );
		}
	}

	// Exit
	return STATUS_SUCCESS;
}

#ifdef _WIN32
const status_t f1::f1pp::exec_directive::create_process_and_wait(
	f1::f1pp::parser & f1par, const char * executable, const char * cmdl, pp::string & out_buf )
{
	status_t status = STATUS_SUCCESS;

	// Create temp file
	_TCHAR szTempFileName[1024] = "";
	HANDLE hTempFile = CreateTempFile( TEXT("f1pp"), szTempFileName, countof(szTempFileName) );
    if( INVALID_HANDLE_VALUE == hTempFile )
	{
		return _T('\0') == szTempFileName[0] ?
			f1par.message( syntax::IMSG_ERROR_CANNOT_CREATE_TEMPFILE ):
			f1par.message( syntax::IMSG_ERROR_CANNOT_CREATE_TEMPFILE_S, szTempFileName );
	}

	// Create process
	STARTUPINFO si = {};
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = GetStdHandle( STD_INPUT_HANDLE );
	si.hStdOutput = hTempFile;
	si.hStdError = GetStdHandle( STD_ERROR_HANDLE );

	PROCESS_INFORMATION pi = {};
	if( FALSE == CreateProcess( (LPSTR) executable, (LPSTR) cmdl, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi ) )
	{
		if( !m_soft )
		{
			f1par.message( syntax::IMSG_ERROR_CANNOT_START_PROCESS_S, nullptr != executable ? executable : cmdl );
			status = STATUS_WARNING_ERROR_ISSUED;
		}
		else
			status = STATUS_WARNING_NOT_FOUND;
	}
	else
	{
		WaitForSingleObject( pi.hProcess, INFINITE );
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );

		// Read data
		{
		f1::io::file_istream fis( szTempFileName );
		if( fis.bad() )
		{
			f1par.message( syntax::IMSG_ERROR_CANNOT_OPEN_TEMPFILE_S, szTempFileName );
			status = STATUS_WARNING_ERROR_ISSUED;
		}
		else if( 0 != fis.get_size() )
		{
			f1::core::buffer buf;
			fis.load( buf );
			out_buf.add_part( buf );
		}}
	}
	CloseHandle( hTempFile );
	DeleteFile( szTempFileName );

	// Exit
	return status;
#else
# error Platform is not yet supported!
#endif
}
/*END OF exec_directive.cxx*/
