/*context.cxx*/
#include "pch.h"
#include "main.h"

const f1::msgout::Igroup::message_desc f1::pp::context::m_msgout_messages[] = {
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, MSGID_STANDARD_NOT_FOUND,		"Standard not found: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, MSGID_MACRO_NOT_AVAILABLE,	"Macro is not available: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID( severity_none, 0 )
};

f1::pp::context::context() __noexcept:
	m_flags( 0 ),// pp::context::fl_show_includes ),
//	m_default_error_maximum_count( 0 ), m_default_warning_maximum_count( 0 ), m_maximum_identifier_length( 0 ), m_maximum_charconst_length( 0 ),
//	m_maximum_include_depth( 0 ),
	m_filename_print( Efilename_print_each ), m_newline_type( newline_crnl )
{
	m_fs_context.initialize();
	m_parser_context.initialize();

	m_file_storage_class.initialize();
	m_storage_class = &m_file_storage_class;

	m_nls_context.initialize();
	m_nls_context.set_default_codepage( f1::nls::eid_mibe_iso8859_1 );
	m_nls_context.set_default_codepage_encoding( f1::nls::eid_mibe_iso8859_1 );
	m_default_input_encoding = m_nls_context.get_default_codepage_encoding();
	m_default_output_encoding = m_nls_context.get_default_codepage_encoding();

	m_message_group.append_messages( m_msgout_messages, _countof(m_msgout_messages) - 1 );

	memset( m_limit, 0, sizeof(m_limit) );

	if( _is_little_endian_machine() )
		m_flags |= fl_little_endian;
	else
		m_flags |= fl_big_endian;

//	m_default_standard.initialize();
//	pp::service::get().append_standard( m_default_standard );
}
f1::pp::context::~context() __noexcept
{
	remove_all_event_handlers();
	m_parsers.clear();
	set_syntax( nullptr );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::context::modify_flags( unsigned flags_to_set, unsigned flags_to_clear, unsigned * prev_flags )
{
	if( (flags_to_set & ~flags_to_clear) & fl_big_endian )			// if BE flag is to set,
		m_flags &= ~fl_little_endian;								// clear LE flags
	else if( (flags_to_set & ~flags_to_clear) & fl_little_endian )	// if LE flag is to set,
		m_flags &= ~fl_big_endian;									// clear BE flags

	m_flags.modify( flags_to_set, flags_to_clear, prev_flags );

	return STATUS_SUCCESS;
}
unsigned f1::pp::context::get_flags( unsigned mask ) const
{
	return m_flags & mask;
}

const status_t f1::pp::context::set_provider( Iprovider * prov ) __noexcept
{
	m_provider = prov;
	return STATUS_SUCCESS;
}
f1::pp::Iprovider * f1::pp::context::get_provider() __noexcept
{
	return m_provider;
}

/*const status_t f1::pp::context::set_root_source( const char * file )
{
	status_t	status;

	core::Tref<parser::file_source> f;
	if( m_parser_context.create_file_source( f, m_fs_context, file ).failed_status( status ) )
		return status;

	return set_root_source( *f );
}
const status_t f1::pp::context::set_root_source( parser::Isource & src )
{
	m_root_source = &src;
	return STATUS_SUCCESS;
}
f1::parser::Isource * f1::pp::context::get_root_source()
{
	return m_root_source;
}*/

const f1::fs::context & f1::pp::context::get_fs_context() const
{
	return m_fs_context;
}
f1::fs::context & f1::pp::context::get_fs_context()
{
	return m_fs_context;
}

const f1::parser::context & f1::pp::context::get_parser_context() const
{
	return m_parser_context;
}
f1::parser::context & f1::pp::context::get_parser_context()
{
	return m_parser_context;
}
/*const status_t f1::pp::context::set_parser( parser::Iparser & parser )
{
	m_parser = &parser;
	return STATUS_SUCCESS;
}
f1::parser::Iparser * f1::pp::context::get_parser()
{
	return m_parser;
}*/

const status_t f1::pp::context::set_syntax( const f1::guid_t & syntax_guid ) __noexcept
{
	status_t status;
	core::Tref<pp::Isyntax> syntax;
	if( pp::service::get().get_syntax( syntax, syntax_guid ).failed_status( status ) )
		return status;
	return set_syntax( syntax );
}
const status_t f1::pp::context::set_syntax( Isyntax * syntax ) __noexcept
{
	if( m_syntax == syntax )
		return STATUS_SUCCESS;

	if( nullptr != m_syntax )
		m_syntax->detach_from_context( *this );
	m_syntax = syntax;
	if( nullptr != m_syntax )
		m_syntax->attach_to_context( *this );

//	if( nullptr != m_syntax )	-???
//	{
//		__ignore_retval m_standard_list.remove( syntax );
//	}

	return STATUS_SUCCESS;
}
f1::pp::Isyntax * f1::pp::context::get_syntax() __noexcept
{
	return m_syntax;
}
const status_t f1::pp::context::disable_syntax( const Isyntax & syntax ) __noexcept
{
	return m_disabled_syntaxes.append( &syntax );
}
bool f1::pp::context::is_syntax_disabled( const Isyntax & syntax ) const __noexcept
{
	return m_disabled_syntaxes.exists( &syntax );
}
bool f1::pp::context::is_syntax_disabled( const f1::guid_t & syntax_guid ) const __noexcept
{
	array_foreach( const_syntax_list_t::const_iterator, it, m_disabled_syntaxes )
	{
		if( syntax_guid == it->get_guid() )
			return true;
	}
	return false;
}
const status_t f1::pp::context::get_preferred_syntax( core::Tconst_ref<pp::Isyntax> & res ) const
{
	array_foreach( syntax_list_t::const_reverse_iterator, it, pp::service::get().get_syntax_list() )
	{
		if( !is_syntax_disabled( **it ) )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}
f1::pp::Isyntax * f1::pp::context::extract_syntax( pp::Istandard & standard )
{
	if( nullptr != type_cast<f1::pp::Isyntax *>( &standard ) )
		return static_cast<f1::pp::Isyntax *>( &standard );

	size_t guid_count = 0;
	const f1::guid_t * guid_list = standard.get_base_guid_list( guid_count );
	if( nullptr == guid_list || 0 == guid_count )
		return nullptr;

	core::Tref<pp::Isyntax> base_syntax = nullptr;
	for( size_t i = 0; i < guid_count; ++ i )
	{
		pp::service::get().get_syntax( base_syntax, guid_list[i] );
		if( nullptr != base_syntax )
			return base_syntax;
	}

	for( size_t i = 0; i < guid_count; ++ i )
	{
		core::Tref<pp::Istandard> base_standard = nullptr;
		pp::service::get().get_standard( base_standard, guid_list[i] );
		if( nullptr != base_standard )
		{
			base_syntax = extract_syntax( *base_standard );
			if( nullptr != base_syntax )
				return base_syntax;
		}
	}

	return nullptr;
}

const status_t f1::pp::context::set_newline_type( newline_type_t newline_type )
{
	__debugbreak_if( unsigned(newline_type) >= newline_count )
		return STATUS_ERROR_INVALID_PARAMETER;
	m_newline_type = newline_type;
	return STATUS_SUCCESS;
}
f1::pp::context::newline_type_t f1::pp::context::get_newline_type() const
{
	return m_newline_type;
}
static const struct {
	const char *	m_seq;
	const char *	m_name;
	size_t			m_length;
	f1::pp::context::newline_type_t		m_newline_type;
} s_newline_type_string[] = {
	{ "\x20",	STR_LEN_PAIR("none"),	f1::pp::context::newline_none },
	{ "\r",		STR_LEN_PAIR("cr"),		f1::pp::context::newline_cr },
	{ "\r\n",	STR_LEN_PAIR("crnl"),	f1::pp::context::newline_crnl },
	{ "\n",		STR_LEN_PAIR("nl"),		f1::pp::context::newline_nl },
	{ "\n\r",	STR_LEN_PAIR("nlcr"),	f1::pp::context::newline_nlcr },

	{ "\r\n",	STR_LEN_PAIR("crlf"),	f1::pp::context::newline_crlf },
	{ "\n",		STR_LEN_PAIR("lf"),		f1::pp::context::newline_lf },
	{ "\n\r",	STR_LEN_PAIR("lfcr"),	f1::pp::context::newline_lfcr },
};
f1::pp::context::newline_type_t f1::pp::context::get_newline_type_from_string( const char * str, size_t len )
{
	if( size_t(-1) == len )
		len = __small_strlen( str );
	for( size_t i = 0; i < _countof(s_newline_type_string); ++ i )
	{
		if( s_newline_type_string[i].m_length != len )
			continue;
		if( !__small_strncmp( str, s_newline_type_string[i].m_name, len ) )
			return s_newline_type_string[i].m_newline_type;
	}
	return newline_error;
}
const char * f1::pp::context::get_newline_type_string( newline_type_t type )
{
	__debugbreak_if( unsigned(type) >= newline_count )
		return nullcstr;
	return s_newline_type_string[type].m_name;
}
const char * f1::pp::context::get_current_newline_type_string() const
{
	return get_newline_type_string( m_newline_type );
}
const char * f1::pp::context::get_newline_type_seq( newline_type_t type )
{
	__debugbreak_if( unsigned(type) >= newline_count )
		return nullcstr;
	return s_newline_type_string[type].m_seq;
}
const char * f1::pp::context::get_current_newline_type_seq() const
{
	return get_newline_type_seq( m_newline_type );
}
unsigned f1::pp::context::count_newlines( const char * s, const char * end_s, unsigned * new_line_types ) const
{
	unsigned count = 0;
	unsigned type = 0;

	while( s < end_s )
	{
		if( '\n' == *s )
		{
			++ s;
			if( s < end_s && '\r' == *s )
			{
				++ s;
				type |= fl_newline_type_mask_nlcr;
			}
			else
			{
				type |= fl_newline_type_mask_nl;
			}
			++ count;
		}
		else if( '\r' == *s )
		{
			++ s;
			if( s < end_s && '\n' == *s )
			{
				++ s;
				type |= fl_newline_type_mask_crnl;
			}
			else
			{
				type |= fl_newline_type_mask_cr;
			}
			++ count;
		}
		else
			++ s;
	}
	if( nullptr != new_line_types )
		*new_line_types = type;
	return count;
}

const status_t f1::pp::context::set_message_base_directory( const pp::string & base_path )
{
	m_message_base_path.set_string( base_path.data(), base_path.length() );
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::set_message_base_directory( const core::string & base_path )
{
	m_message_base_path = base_path;
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::set_message_base_directory( const fs::path & base_path )
{
	m_message_base_path = base_path;
	return STATUS_SUCCESS;
}
const f1::fs::path & f1::pp::context::get_message_base_directory() const
{
	return m_message_base_path;
}

const status_t f1::pp::context::set_output_base_directory( const pp::string & base_path )
{
	m_output_base_path.set_string( base_path.data(), base_path.length() );
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::set_output_base_directory( const core::string & base_path )
{
	m_output_base_path = base_path;
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::set_output_base_directory( const fs::path & base_path )
{
	m_output_base_path = base_path;
	return STATUS_SUCCESS;
}
const f1::fs::path & f1::pp::context::get_output_base_directory() const
{
	return m_output_base_path;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Outputs

const status_t f1::pp::context::modify_output_flags( output_flags_t flags_to_set, output_flags_t flags_to_clear, output_flags_t * prev_flags )
{
	m_output_flags.modify( flags_to_set, flags_to_clear, prev_flags ); 
	return STATUS_SUCCESS;
}
f1::pp::context::output_flags_t f1::pp::context::get_output_flags( output_flags_t mask ) const
{
	return m_output_flags & mask;
}

const status_t f1::pp::context::set_output_class( pp::Ioutput_class * output_class ) __noexcept
{
	if( m_output_class == output_class )
		return STATUS_SUCCESS;

	m_output_class = output_class;
	m_output = nullptr;

	return STATUS_SUCCESS;
}
f1::pp::Ioutput_class * f1::pp::context::get_output_class() __noexcept
{
	return m_output_class;
}

const status_t f1::pp::context::make_output_file_name( core::string & buf, const char * ext, const char * src_filename, size_t src_len ) const
{
	const core::string * filename = nullptr;
	const core::string * directory = nullptr;

	// Generate output file name
	core::string filename_buf;
	if( m_output_file_name.empty() )
	{
		fs::path source_path( src_filename, src_len );
		source_path.remove_directory();
		source_path.move_to( filename_buf );
		filename_buf.append( ext );
		filename = &filename_buf;
	}
	else
	{
		filename = &m_output_file_name;
	}

	// Generate output directory
	core::string directory_buf;
	if( m_output_directory.empty() )
	{
		fs::path source_path( src_filename, src_len );
		source_path.remove_filename();
		source_path.remove_backslash();

		if( source_path.is_empty() )
		{
			char sz[_MAX_PATH];
			GetCurrentDirectory( countof(sz), sz );
			directory_buf = sz;
		}
		else
			source_path.move_to( directory_buf );
		directory = &directory_buf;
	}
	else
	{
		directory = &m_output_directory;
	}

	// Generate path
	fs::path p( *directory );
	p.combine( *filename );

	p.move_to( buf );

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::pp::context::set_output_file_name( const char * filename, size_t len )
{
	return set_output_file_name( fs::path( filename, len ) );
}
const status_t f1::pp::context::set_output_file_name( const pp::string & filename )
{
	return set_output_file_name( fs::path( filename.data(), filename.length() ) );
}
const status_t f1::pp::context::set_output_file_name( const core::string & filename )
{
	return set_output_file_name( fs::path( filename ) );
}
const status_t f1::pp::context::set_output_file_name( const fs::path & filename )
{
	f1::core::string	buf;
	fs::path			path( filename );

	// Extract file name
	if( f1::fs::path::ct_target == path.get_last( &buf ) )
	{
		m_output_file_name.assign( buf );
		path.remove_last();
	}

	// Extract directory
	if( !path.is_empty() )
	{
		path.remove_backslash();
		set_output_directory( path.get_string() );
	}

	// Exit
	return STATUS_SUCCESS;
}
const f1::core::string & f1::pp::context::get_output_file_name() const
{
	return m_output_file_name;
}

const status_t f1::pp::context::set_output_directory( const char * name, size_t len )
{
	return m_output_directory.assign( name, len );
}
const status_t f1::pp::context::set_output_directory( const core::string & name )
{
	return m_output_directory.assign( name );
}
const status_t f1::pp::context::set_output_directory( const pp::string & name )
{
	return m_output_directory.assign( name );
}
const status_t f1::pp::context::set_output_directory( const fs::path & name )
{
	return m_output_directory.assign( name.get_string() );
}
const f1::core::string & f1::pp::context::get_output_directory() const
{
	return m_output_directory;
}

const status_t f1::pp::context::set_output( Ioutput * output ) __noexcept
{
	if( m_output == output )
		return STATUS_SUCCESS;

	m_output = output;

	return STATUS_SUCCESS;
}
f1::pp::Ioutput * f1::pp::context::get_output() __noexcept
{
	if( nullptr == m_output )
	{
		status_t status;

		__debugbreak_if( nullptr == m_output_class )
			return nullptr;//STATUS_ERROR_NOT_INITIALIZED;

		core::Tref<Ioutput> outp = nullptr;
		if( m_output_class->new_output( outp ).failed_status( status ) )
			return nullptr;
		if( outp->initialize().failed_status( status ) )
			return nullptr;

		set_output( outp );
	}
	return m_output;
}

const status_t f1::pp::context::set_storage_class( pp::Istorage_class * storage_class ) __noexcept
{
	if( m_storage_class == storage_class )
		return STATUS_SUCCESS;

	m_storage_class = storage_class;
	m_storage = nullptr;

	return STATUS_SUCCESS;
}
f1::pp::Istorage_class * f1::pp::context::get_storage_class() __noexcept
{
	return m_storage_class;
}

const status_t f1::pp::context::create_storage( core::Tref<pp::Istorage> & res,
	const core::string & output_file_name, const f1::guid_t * preferred_class, Istorage::storage_flags_t flags,
	const core::Iidentifier * ident
	)
{
	status_t status;

	__debugbreak_if( nullptr == m_storage_class )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<Istorage> stg = nullptr;
	if( nullptr != preferred_class )
		status = service::get().create_storage( stg, *preferred_class, *this, output_file_name, flags, ident );
	else
		status = service::get().create_storage( stg, *m_storage_class, *this, output_file_name, flags, ident );
	if( status.failed() )
		return status;
	__debugbreak_if( nullptr == stg )
		return STATUS_ERROR_INTERNAL;

	res = stg;
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::set_storage( Istorage * storage ) __noexcept
{
	if( m_storage == storage )
		return STATUS_SUCCESS;

	m_storage = storage;

	return STATUS_SUCCESS;
}
f1::pp::Istorage * f1::pp::context::get_storage() __noexcept
{
	return m_storage;
}

/*f1::fs::context & f1::pp::context::get_fs_context()
{
	return m_fs_context;
}*/
/*const f1::fs::search_dir_list & f1::pp::context::get_search_dir_list() const
{
	return m_search_dir_list;
}*/

const f1::nls::context & f1::pp::context::get_nls_context() const
{
	return m_nls_context;
}

const status_t f1::pp::context::append_include_file( const core::static_string & filename, unsigned include_flags_to_set, unsigned include_flags_to_clear )
{
	include_file_t * desc = m_include_files.append_new();	// TODO: use it when creating parser
	desc->m_file_name = filename;
	desc->m_flags_to_set = include_flags_to_set;
	desc->m_flags_to_clear = include_flags_to_clear;
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::append_include_files( int sep, const core::static_string & filename, unsigned include_flags_to_set, unsigned include_flags_to_clear )
{
	const char * path = filename.data();
	size_t cch = filename.length();
	bool	in_quote = false;
	const char * path_end = path + (size_t(-1) == cch ? strlen( path ) : cch );
	const char * start = path;
	for(; path < path_end; ++ path )
	{
		if( '\"' == *path )
			in_quote = !in_quote;
		else if( sep == *path && !in_quote )
		{
			append_include_file( core::static_string(start, path - start), include_flags_to_set, include_flags_to_clear );
			start = ++ path;	// skip separator
		}
	}
	if( start != path )
		append_include_file( core::static_string(start, path - start), include_flags_to_set, include_flags_to_clear );
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::append_include_files( const core::Tarray<core::string> & filenames, unsigned include_flags_to_set, unsigned include_flags_to_clear )
{
	array_foreach( core::Tarray<core::string>::const_iterator, it, filenames )
		append_include_file( core::static_string(*it), include_flags_to_set, include_flags_to_clear );
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::remove_include_file( const core::static_string & filename )
{
	array_foreach( core::Tarray<include_file_t>::iterator, it, m_include_files )
	{
		include_file_t & desc = *it;
		__debugbreak_if( desc.m_file_name.equal_nocase( filename ) )
		{
			m_include_files.erase( it );
			return STATUS_SUCCESS;
		}
	}
	return STATUS_ERROR_NOT_FOUND;
}
const status_t f1::pp::context::remove_include_files( int sep, const core::static_string & filename )
{
	const char * path = filename.data();
	size_t cch = filename.length();
	bool	in_quote = false;
	const char * path_end = path + (size_t(-1) == cch ? strlen( path ) : cch );
	const char * start = path;
	for(; path < path_end; ++ path )
	{
		if( '\"' == *path )
			in_quote = !in_quote;
		else if( sep == *path && !in_quote )
		{
			remove_include_file( core::static_string(start, path - start) );
			start = ++ path;	// skip separator
		}
	}
	if( start != path )
		remove_include_file( core::static_string(start, path - start) );
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::remove_include_files( const core::Tarray<core::string> & filenames )
{
	array_foreach( core::Tarray<core::string>::const_iterator, it, filenames )
		remove_include_file( core::static_string(*it) );
	return STATUS_SUCCESS;
}
/*const status_t f1::pp::context::set_maximum_include_depth( unsigned maximum_depth )
{
	m_maximum_include_depth = maximum_depth;
	return STATUS_SUCCESS;
}
unsigned f1::pp::context::get_maximum_include_depth() const
{
	return m_maximum_include_depth;
}*/

const status_t f1::pp::context::append_macro( const char * name, size_t name_len )
{
	if( size_t(-1) == name_len )
		name_len = core::string::length( name );

	array_foreach( core::Tarray<macro_def_t>::const_iterator, it, m_macros_to_define )
	{
		const macro_def_t & mac = *it;
		__debugbreak_if( mac.m_name.equal( name, name_len ) )
			return STATUS_ERROR_ALREADY_EXISTS;
	}

	macro_def_t * mac = m_macros_to_define.append_new();
	mac->m_name.assign( name, name_len );

	return STATUS_SUCCESS;
}
const status_t f1::pp::context::append_macro( const char * name, size_t name_len, const char * value, size_t value_len )
{
	if( size_t(-1) == name_len )
		name_len = core::string::length( name );

	array_foreach( core::Tarray<macro_def_t>::const_iterator, it, m_macros_to_define )
	{
		const macro_def_t & mac = *it;
		if( mac.m_name.equal( name, name_len ) )
			return STATUS_ERROR_ALREADY_EXISTS;
	}

	macro_def_t * mac = m_macros_to_define.append_new();
	mac->m_name.assign( name, name_len );
	mac->m_value.assign( value, value_len );

	return STATUS_SUCCESS;
}
const status_t f1::pp::context::remove_macro( const char * name, size_t name_len )
{
	array_foreach( core::Tarray<macro_def_t>::iterator, it, m_macros_to_define )
	{
		const macro_def_t & mac = *it;
		if( mac.m_name.equal( name, name_len ) )
		{
			m_macros_to_define.erase( it );
			return STATUS_SUCCESS;
		}
	}
	return STATUS_ERROR_NOT_FOUND;
}

intmax_t f1::pp::context::get_limit_value( limit_t lim ) const
{
	__debugbreak_if( _countof(m_limit) <= size_t(lim) )
		return 0;
	return m_limit[lim];
}
const status_t f1::pp::context::set_limit_value( limit_t lim, intmax_t value )
{
	__debugbreak_if( _countof(m_limit) <= size_t(lim) )
		return STATUS_ERROR_INVALID_PARAMETER;
	m_limit[lim] = value;
	return STATUS_SUCCESS;
}

/*const status_t f1::pp::context::set_default_error_maximum_count( unsigned count )
{
	m_default_error_maximum_count = count;
	return STATUS_SUCCESS;
}
unsigned f1::pp::context::get_default_error_maximum_count() const
{
	return m_default_error_maximum_count;
}
const status_t f1::pp::context::set_default_warning_maximum_count( unsigned count )
{
	m_default_warning_maximum_count = count;
	return STATUS_SUCCESS;
}
unsigned f1::pp::context::get_default_warning_maximum_count() const
{
	return m_default_warning_maximum_count;
}

const status_t f1::pp::context::set_maximum_identifier_length( unsigned count )
{
	m_maximum_identifier_length = count;
	return STATUS_SUCCESS;
}
unsigned f1::pp::context::get_maximum_identifier_length() const
{
	return m_maximum_identifier_length;
}

const status_t f1::pp::context::set_maximum_charconst_length( unsigned count )
{
	m_maximum_charconst_length = count;
	return STATUS_SUCCESS;
}
unsigned f1::pp::context::get_maximum_charconst_length() const
{
	return m_maximum_charconst_length;
}*/

const status_t f1::pp::context::set_frontend_context( const f1::pp::frontend::context * frontend_context )
{
	m_frontend_context = frontend_context;
	return STATUS_SUCCESS;
}
const f1::pp::frontend::context * f1::pp::context::get_frontend_context() const
{
	return m_frontend_context;
}

const status_t f1::pp::context::set_character_encoding( const nls::Iencoding * enc )
{
	m_character_encoding = enc;
	return STATUS_SUCCESS;
}
const f1::nls::Iencoding * f1::pp::context::get_character_encoding() const
{
	return m_character_encoding;
}
const status_t f1::pp::context::set_default_input_encoding( const nls::Iencoding * enc )
{
	m_default_input_encoding = enc;
	return STATUS_SUCCESS;
}
const f1::nls::Iencoding * f1::pp::context::get_default_input_encoding() const
{
	return m_default_input_encoding;
}
const status_t f1::pp::context::set_default_output_encoding( const nls::Iencoding * enc )
{
	m_default_output_encoding = enc;
	return STATUS_SUCCESS;
}
const f1::nls::Iencoding * f1::pp::context::get_default_output_encoding() const
{
	return m_default_output_encoding;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Messages

const f1::msgout::context & f1::pp::context::get_msgout_context() const
{
	return m_msgout_context;
}
f1::msgout::context & f1::pp::context::get_msgout_context()
{
	return m_msgout_context;
}

f1::pp::context::Efilename_print f1::pp::context::get_filename_print() const
{
	return m_filename_print;
}
const status_t f1::pp::context::set_filename_print( Efilename_print val )
{
	m_filename_print = val;
	return STATUS_SUCCESS;
}

const status_t f1::pp::context::append_message_sink( Imessage_sink & sink )
{
	return m_message_sink_list.append_unique( &sink );
}
const status_t f1::pp::context::remove_message_sink( Imessage_sink & sink )
{
	return m_message_sink_list.remove( &sink );
}
const status_t f1::pp::context::default_message_sink::output_message( msgout::context & ctx, f1::msgout::msgid_t format, va_list ap )
{
	// Display message and exit
	return ctx.message_ap( format, ap );
}

const status_t f1::pp::context::message( f1::msgout::msgid_t format, ... )
{
	va_list ap;
	va_start( ap, format );
	status_t status = message_ap( format, ap, m_msgout_context );
	va_end( ap );
	return status;
}
const status_t f1::pp::context::message_ap( f1::msgout::msgid_t format, va_list ap, msgout::context & ctx )
{
	status_t status;

	// If no client sinks installed, just call the default one
	if( 0 == m_message_sink_list.size() )
		return m_default_message_sink.output_message( ctx, format, ap );

	// Call all sinks
	array_foreach( core::Tweak_object_array<Imessage_sink>::iterator, it, m_message_sink_list )
	{
		if( nullptr == *it )
			continue;
		it->output_message( ctx, format, ap ).assert_if_failed();
	}

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Storage

/*const status_t f1::pp::context::include( const char * data, size_t len )
{
	return m_parser->insert_data( data, len );
}
const status_t f1::pp::context::include( parser::Isource & src )
{
	return m_parser->insert_source( src );
}
*/
/*const status_t f1::pp::context::append_file( const char * file )
{
	return m_files.append_unique( file );
}*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sources

const status_t f1::pp::context::append_source( const char * filename, size_t len )
{
#if 1
	source_desc * desc = m_sources.append_new();
	desc->m_filename.assign( filename, len );
	return STATUS_SUCCESS;
#else
	status_t	status;

	core::Tref<parser::file_source> f;
	if( m_parser_context.create_file_source( f, m_fs_context, file ).failed_status( status ) )
		return status;

	return append_source( *f );
#endif
}
const status_t f1::pp::context::append_source( parser::Isource & src )
{
#if 1
	source_desc * desc = m_sources.append_new();
	desc->m_source = &src;
	return STATUS_SUCCESS;
#else
	return m_sources.append_unique( &src );
#endif
}
const status_t f1::pp::context::remove_all_sources()
{
	return m_sources.remove_all();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Attached standards

const status_t f1::pp::context::append_standard( pp::Istandard & standard )
{
	return m_standard_list.append( &standard );
}
const status_t f1::pp::context::append_standards( standard_list_t & standards )
{
	return m_standard_list.append( standards );
}
const status_t f1::pp::context::remove_standard( pp::Istandard & standard )
{
	return m_standard_list.remove( &standard );
}
const status_t f1::pp::context::remove_all_standards()
{
	return m_standard_list.remove_all();
}

const f1::pp::standard_list_t & f1::pp::context::get_standard_list() const
{
	return m_standard_list;
}
f1::pp::standard_list_t & f1::pp::context::get_standard_list()
{
	return m_standard_list;
}

const status_t f1::pp::context::set_default_standard( const pp::Istandard & standard )
{
	m_default_standard = &standard;
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::set_default_standard( const f1::guid_t & guid )
{
	return pp::service::get().get_standard( m_default_standard, guid );
}
const status_t f1::pp::context::set_default_standard( const char * name, ptrdiff_t len )
{
	return pp::service::get().get_standard( m_default_standard, name, len );
}

const f1::pp::Istandard * f1::pp::context::find_item_standard( const f1::guid_t & item_class_guid, const pp::string & name ) const
{
	// Check this standard first
	array_foreach( standard_list_t::const_iterator, it, m_standard_list )
	{
		const f1::pp::Istandard * standard = find_item_standard( item_class_guid, **it, name );
		if( nullptr != standard )
			return standard;
	}

	// Exit
	return nullptr;
}
const f1::pp::Istandard * f1::pp::context::find_item_standard( const f1::guid_t & item_class_guid, const Istandard & standard, const pp::string & name ) const
{
	// Check in base standards as well
	size_t count = 0;
	const f1::guid_t * guids = standard.get_base_guid_list( count );
	if( nullptr != guids )
	{
		core::Tref<pp::Istandard> parent_standard;
		for( size_t i = 0; i < count; ++ i )
		{
			if( pp::service::get().get_standard( parent_standard, guids[i] ).failed() )
				continue;
			const f1::pp::Istandard * base_standard = find_item_standard( item_class_guid, *parent_standard, name );
			if( nullptr != base_standard )
				return base_standard;
		}
	}

	// Check self after all base standards are checked
	if( standard.is_item_supported( item_class_guid, name ) )
		return &standard;

	// Exit
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Parser commands

const status_t f1::pp::context::add_preinit_parser_command( Iparser::Icommand & cmd )
{
	return m_preinit_parser_commands.append( &cmd );
}
const status_t f1::pp::context::add_postinit_parser_command( Iparser::Icommand & cmd )
{
	return m_postinit_parser_commands.append( &cmd );
}

#if 0
const status_t f1::pp::context::append_parser_parameter( const f1::guid_t & param_guid, bool val )
{
	parser_param_desc_t * desc = m_parser_param_list.append_new();
	desc->m_param_guid = param_guid;
	desc->m_value.set_type( core::variant::type_bool );
	desc->m_value.set_bool( val );
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::append_parser_parameter( const f1::guid_t & param_guid, uintmax_t val )
{
	parser_param_desc_t * desc = m_parser_param_list.append_new();
	desc->m_param_guid = param_guid;
	desc->m_value.set_type( core::variant::type_uint64 );
	desc->m_value.set_uint64( val );
	return STATUS_SUCCESS;
}
/*const status_t f1::pp::context::append_parser_parameter( const f1::guid_t & param_guid, const char * val, size_t len )
{
	if( size_t(-1) == len )
		len = core::string::length( val );
	parser_param_desc_t * desc = m_parser_param_list.append_new();
	desc->m_param_guid = param_guid;
	desc->m_value.set_type( core::variant::type_string );
	desc->m_value.set_char( val, 0, len );
	return STATUS_SUCCESS;
}*/
const status_t f1::pp::context::append_parser_parameter( const f1::guid_t & param_guid, const core::variant & val )
{
	parser_param_desc_t * desc = m_parser_param_list.append_new();
	desc->m_param_guid = param_guid;
	desc->m_value = val;
	return STATUS_SUCCESS;
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::context::append_dump_task( dump_flags_t flags, const char * target_filename, size_t length )
{
	dump_task * task = m_dump_task_list.append_new();
	task->m_dump_flags = flags;
	task->m_target_filename.assign( target_filename, length );
	return STATUS_SUCCESS;
}
const f1::core::Tarray<f1::pp::context::dump_task> & f1::pp::context::get_dump_task_list() const
{
	return m_dump_task_list;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: make this function parallel
const status_t f1::pp::context::process()
{
	status_t common_status = STATUS_SUCCESS;
	status_t status;

	// Check current state
//	__debugbreak_if( nullptr == m_root_source )
//		return STATUS_ERROR_NOT_INITIALIZED;
//	__debugbreak_if( nullptr == m_syntax )
//		return STATUS_ERROR_NOT_INITIALIZED;
	__debugbreak_if( nullptr == m_output )
		return STATUS_ERROR_NOT_INITIALIZED;

//	__debugbreak_if( m_root_source->is_invalid() )
//		return STATUS_ERROR_INVALID_STATE;

	// Call parsers in order
	array_foreach( core::Tarray<source_desc>::iterator, it, m_sources )
	{
		source_desc & desc = *it;
		if( nullptr != desc.m_source )
			status = _process( *desc.m_source );
		else
			status = _process( desc.m_filename.c_str() );
		if( status.failed() && common_status.succeeded() )
			common_status = status;
		desc.m_source = nullptr;
	}

	// Exit
	return common_status;
}
const status_t f1::pp::context::_process( const char * base_source )
{
	status_t	status;

	// Select requested syntax
	if( nullptr == get_syntax() )
	{
		core::Tref<pp::Isyntax> syn;
		__debugbreak_if( get_preferred_syntax( syn ).failed_status( status ) )
			return STATUS_ERROR_DISABLED;
		__debugbreak_if( nullptr == syn )
			return STATUS_ERROR_INTERNAL;
		set_syntax( syn );
	}

	// Create syntax parser
	core::Tref<pp::Iparser> parser;
	if( m_syntax->new_parser( parser ).failed_status( status ) )
		return status;

	if( parser->initialize( *this, *m_syntax, *get_output(), *get_storage(), nullptr, base_source ).failed_status( status ) )
		return status;

	// Process source by the parser and exit
	return _process_common( *parser );
}
const status_t f1::pp::context::_process( parser::Isource & src )
{
	status_t	status;

	// Select requested syntax
	core::Tref<pp::Isyntax> syn;
	__debugbreak_if( get_preferred_syntax( syn ).failed_status( status ) )
		return STATUS_ERROR_DISABLED;
	__debugbreak_if( nullptr == syn )
		return STATUS_ERROR_INTERNAL;
	set_syntax( syn );

	// Create low-level parser, if needed
/*	core::Tref<parser::Iparser> ll_parser = m_parser;
	if( nullptr == ll_parser )
	{
		// Create new parser and load root source
		if( m_parser_context.create_parser( ll_parser, &src ).failed_status( status ) )
			return status;
	}
	else
	{
		// Load data from the root source
		ll_parser->reset();
		if( ll_parser->insert_source( src ).failed_status( status ) )
			return status;
	}
*/
	// Create syntax parser
	core::Tref<pp::Iparser> parser;
	if( m_syntax->new_parser( parser ).failed_status( status ) )
		return status;

	if( parser->initialize( *this, *m_syntax, *get_output(), *get_storage(), src ).failed_status( status ) )
		return status;

	// Process source by the parser
	status = _process_common( *parser );

	// Clean up
//	set_syntax( nullptr );

	// Exit
	return status;
}
const status_t f1::pp::context::_process_common( pp::Iparser & parser )
{
	status_t status;

	// Append parser
	if( on_parser_attached( parser ).failed_status( status ) )
		return status;
	m_parsers.append( &parser );

	// Initialize parser
	if( nullptr != m_syntax )
		m_syntax->attach_to_parser( parser );

	if( !m_standard_list.empty() )
	{
		array_foreach( standard_list_t::const_iterator, it, m_standard_list )
			it->attach_to_parser( parser );
	}
	else if( nullptr != m_default_standard )
		m_default_standard->attach_to_parser( parser );
	else if( nullptr != m_syntax )
	{
		size_t count = 0;
		const f1::guid_t * guid_list = m_syntax->get_preferred_standard_list( count );
		if( nullptr != guid_list )
		{
			for( size_t i = 0; i < count; ++ i )
			{
				core::Tref<pp::Istandard> standard;
				__debugbreak_if( pp::service::get().get_standard( standard, guid_list[i] ).failed() )
				{
					core::string buf;
					guid_list[i].format( buf );
					message( MSGID_STANDARD_NOT_FOUND, buf.c_str() );
					continue;
				}
				__debugbreak_if( nullptr == standard )
					continue;
				standard->attach_to_parser( parser );
			}
		}
		else
			m_syntax->attach_to_parser( parser );
	}

	// Set up parser
//	array_foreach( core::Tarray<parser_param_desc_t>::const_iterator, it, m_parser_param_list )
//		parser->set_parameter( (*it).m_param_guid, (*it).m_value );
	array_foreach( core::Tobject_array<Iparser::Icommand>::iterator, it, m_postinit_parser_commands )
		it->execute( parser );
	m_postinit_parser_commands.clear();

	//core::Tref<pp::Imacro> mac;
	//array_foreach( core::Tarray<macro_def_t>::const_iterator, it, m_macros_to_define ){
	//	const macro_def_t & def = *it;
	//	parser.create_macro( mac, pp::string(def.m_name), nullptr, nullptr, pp::string(def.m_value) );
	//}
	//array_foreach( core::Tarray<core::string>::const_iterator, it, m_macros_to_undefine ){
	//	const core::string & def = *it;
	//	parser.find_macro( mac, pp::string(def), nullptr );
	//	if( nullptr != mac )
	//		parser.remove_macro( *mac );
	//}

	/*if( 0 != m_default_error_maximum_count )
		parser.set_error_maximum_count( m_default_error_maximum_count );
	if( 0 != m_default_warning_maximum_count )
		parser.set_warning_maximum_count( m_default_warning_maximum_count );*/
	for( unsigned i = 0; i < unsigned(_countof(m_limit)); ++ i )
		parser.set_limit_value( limit_t(i), m_limit[i] );
	m_msgout_context.reset_severity_counts();

	// Parse file
//	__debugbreak_ifnot( nullptr != m_output )
//		m_output->on_attach_to_parser( parser );
	__debugbreak_ifnot( nullptr != m_storage )
		m_storage->open();

	invoke_event( event_id_t::ev_begin_task, &parser );
	//parser.get_parser()->invoke_event( f1::parser::etype_source_included, parser.get_parser()->get_root_source() );

	status = parser.parse();
	invoke_event( event_id_t::ev_end_task, &parser );

	__debugbreak_ifnot( nullptr != m_storage )
		m_storage->close();

//	if( nullptr != m_syntax )
//		m_syntax->detach_from_parser( parser );
	on_parser_detached( parser );

	// Exit
	return status;
}
const status_t f1::pp::context::on_parser_attached( pp::Iparser & parser )
{
	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::on_parser_detached( pp::Iparser & parser )
{
	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}

/*END OF context.cxx*/
