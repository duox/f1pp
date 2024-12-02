/*parser_include.cxx*/
#include "pch.h"
#include "main.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include files information
/*
const status_t f1::c_pp::parser::register_file( file_desc ** file, const char * full_path_name, include_flags_t flags )
{
	// Check whether descriptor already added; add new if wasn't
	const file_desc * existing_desc;
	__debugbreak_if( find_file( existing_desc, full_path_name ).succeeded() )
		return STATUS_ERROR_ALREADY_EXISTS;

	// Add new descriptor
	file_desc * desc = m_file_desc_list.append_new();
	desc->m_full_path_name = full_path_name;
	desc->m_flags = flags;

	if( nullptr != file )
		*file = desc;

	// Exit
	return STATUS_SUCCESS;
}
const f1::c_pp::parser::file_desc_list_t & f1::c_pp::parser::get_file_list() const
{
	return m_file_desc_list;
}
// TODO: use hashmap<file_desc *, core::string>
const status_t f1::c_pp::parser::find_file( const file_desc *& res, const char * full_path_name, size_t length ) const
{
	if( size_t(-1) == length )
		length = core::string::length( full_path_name );
	array_foreach( file_desc_list_t::const_iterator, it, m_file_desc_list )
	{
		if( (*it).m_full_path_name.equal( full_path_name, length ) )
		{
			res = &*it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}
*/

/*const status_t f1::c_pp::parser::register_include( file_desc & file, const char * specified_name, const f1::parser::Isource * src, include_flags_t flags )
{
	// Check whether descriptor already added
	include_desc * existing_desc = nullptr;
	if( find_include( existing_desc, specified_name ).succeeded() )
	{
		if( nullptr != existing_desc->m_source )
			return STATUS_ERROR_ALREADY_EXISTS;
		existing_desc->m_source = src;
		__assert( include_flags_t::null == flags );
		return STATUS_SUCCESS;
	}

	// Append new descriptor
	include_desc * desc = m_include_desc_list.append_new();
	desc->m_file_desc = &file;
	desc->m_source = src;
	desc->m_specified_name = specified_name;
	desc->m_flags = flags;

	// Exit
	return STATUS_SUCCESS;
}*/

const status_t f1::c_pp::parser::register_include( include_desc *& res,
	const char * specified_name, size_t length, include_flags_t include_flags )
{
	status_t status;

	// Check for aliases
	const alias_desc * al_desc = nullptr;
	find_alias( al_desc, specified_name, length );

	// Get full path to the file
	core::string full_path_name;
	if( nullptr == al_desc )
		status = resolve_include_filename( full_path_name, core::stringslice(specified_name, length), include_flags );
	else
		status = resolve_include_filename( full_path_name, core::stringslice(al_desc->m_aliased_name), include_flags );
	if( full_path_name.empty() )
		return STATUS_ERROR_NOT_FOUND;

	// Check whether descriptor already added
	const include_desc * existing_inc_desc = nullptr;
	find_include_full_path_name( existing_inc_desc, full_path_name.c_str(), full_path_name.length() );
	if( nullptr == existing_inc_desc )//|| !(existing_inc_desc->m_flags & include_flags_t::fl_once_included) )
	{
		// Create include descriptor
		include_desc * inc_desc = m_include_desc_list.append_new();
		inc_desc->m_specified_name = specified_name;
		inc_desc->m_full_path_name = full_path_name;
		inc_desc->m_storage = nullptr;

		if( nullptr != existing_inc_desc  )
			inc_desc->m_flags = existing_inc_desc->m_flags;
		inc_desc->m_flags |= include_flags;

		existing_inc_desc = inc_desc;
	}

	// Exit
	res = const_cast<include_desc *>(existing_inc_desc);
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::set_include_file( include_desc & desc, const f1::parser::Istorage & stg )
{
	// Check whether descriptor already added
	__debugbreak_if( nullptr != desc.m_storage )
		return STATUS_ERROR_ALREADY_EXISTS;

	// Set the source
	desc.m_storage = &stg;

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::modify_include_flags( include_desc & desc, include_flags_t flags_to_set, include_flags_t flags_to_clear )
{
	// Set the source
	desc.m_flags = core::Tbitflags<include_flags_t>::modify_flags( desc.m_flags, flags_to_set, flags_to_clear );

	// Exit
	return STATUS_SUCCESS;
}

/*const status_t f1::c_pp::parser::register_include( const char * specified_name, const f1::parser::Isource * src, include_flags_t flags )
{
	status_t status;

	// Check whether descriptor already added
	include_desc * inc_desc;
	__debugbreak_if( nullptr != src && find_include( inc_desc, *src ).succeeded() )
		return STATUS_ERROR_ALREADY_EXISTS;

	if( find_include( inc_desc, specified_name ).succeeded() )
	{
		if( nullptr != inc_desc->m_source )
			return STATUS_ERROR_ALREADY_EXISTS;
		inc_desc->m_source = src;
		__assert( include_flags_t::null == flags );
		return STATUS_SUCCESS;
	}

	// Get full path to the file
	core::string full_path_name;
	resolve_include_filename( full_path_name, core::stringslice(specified_name), !!(flags & include_flags_t::fl_system) );

	// Find file descriptor and create new one if file descriptor was not found
//	file_desc * file;
//	if( find_file( *const_cast<const file_desc **>(&file), full_path_name.c_str(), full_path_name.length() ).failed() )
//	{
//		if( register_file( &file, full_path_name.c_str() ).failed_status( status ) )
//			return status;
//	}

	// Create include descriptor
	inc_desc = m_include_desc_list.append_new();
	inc_desc->m_specified_name = specified_name;
	inc_desc->m_full_path_name = full_path_name;
	inc_desc->m_source = src;
	inc_desc->m_flags = flags;

	// Exit
	return STATUS_SUCCESS;
}*/
const f1::c_pp::parser::include_desc_list_t & f1::c_pp::parser::get_include_list() const
{
	return m_include_desc_list;
}

// TODO: use hashmap<include_desc *, f1::parser::Isource *>
const status_t f1::c_pp::parser::find_include( const include_desc *& res, const f1::parser::Istorage & stg ) const
{
	array_foreach( include_desc_list_t::const_iterator, it, m_include_desc_list )
	{
		if( (*it).m_storage == &stg )
		{
			res = &*it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}
// TODO: use hashmap<include_desc *, core::string>
const status_t f1::c_pp::parser::find_include( const include_desc *& res, const char * specified_name, size_t length ) const
{
	if( size_t(-1) == length )
		length = core::string::length( specified_name );
	array_foreach( include_desc_list_t::const_iterator, it, m_include_desc_list )
	{
		if( (*it).m_specified_name.equal_nocase( specified_name, length ) )
		{
			res = &*it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}
// TODO: use hashmap<include_desc *, core::string>
const status_t f1::c_pp::parser::find_include_full_path_name( const include_desc *& res, const char * full_path_name, size_t length ) const
{
	if( size_t(-1) == length )
		length = core::string::length( full_path_name );
	array_foreach( include_desc_list_t::const_iterator, it, m_include_desc_list )
	{
		if( (*it).m_full_path_name.equal( full_path_name, length ) )
		{
			res = &*it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}

const status_t f1::c_pp::parser::dump_includes( msgout::context & ctx, dump_flags_t flags ) const
{
	status_t status;

	if( super::dump_includes( ctx, flags ).failed_status( status ) )
		return status;

	if( flags & dump_flags_t::fl_dump_aliases )
	{
		array_foreach( alias_desc_list_t::const_iterator, it, get_alias_list() )
		{
			const alias_desc & alias = *it;
			ctx.print( "ALIAS \"%s\" = \"%s\"\n", alias.m_alias_name.c_str(), alias.m_aliased_name.c_str() );
		}
	}

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::dump_include_info( msgout::context & ctx, f1::parser::Isource & src, dump_flags_t flags ) const
{
	// Calculate identation
	unsigned ident = 0;
	for( core::Tref<f1::parser::Isource> s = src.get_parent(); nullptr != s; s = s->get_parent() )
		++ ident;

	const include_desc * inc_desc = nullptr;
	find_include( inc_desc, *src.get_storage() );

	// Print string
	if( nullptr != inc_desc )
	{
		if( inc_desc->m_flags & include_flags_t::fl_system )
			ctx.print( "S " );
		else
			ctx.print( "U " );
	}
	else
		ctx.print( "X " );

	if( 0 != ident )
		ctx.print( "%*c", ident - 1, 0x20, src.get_filename() );
	ctx.print( "%s\n", src.get_filename() );

	// Exit
	__unreferenced_parameter( flags );
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Aliases

const status_t f1::c_pp::parser::register_alias( alias_desc ** file, const core::static_string & source_file_name, const core::static_string & target_file_name )
{
	alias_desc * als;
	if( find_alias( *(const alias_desc **)&als, source_file_name.data(), source_file_name.length() ).succeeded() )
		/* TODO: warn user about double alias */;
	else
	{
		als = m_alias_desc_list.append_new();
		als->m_alias_name = source_file_name;
	}
	als->m_aliased_name = target_file_name;

	if( nullptr != file )
		*file = als;
	return STATUS_SUCCESS;
}
const f1::c_pp::parser::alias_desc_list_t & f1::c_pp::parser::get_alias_list() const
{
	return m_alias_desc_list;
}
const status_t f1::c_pp::parser::find_alias( const alias_desc *& res, const char * source_name, size_t length ) const
{
	if( size_t(-1) == length )
		length = core::string::length( source_name );
	array_foreach( alias_desc_list_t::const_iterator, it, m_alias_desc_list )
	{
		const alias_desc & desc = *it;
		if( desc.m_alias_name.equal_nocase( source_name, length ) )
		{
			res = &desc;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Included sources that were marked with with `pragma once'

const status_t f1::c_pp::parser::register_once_include()
{
	status_t status;

#if 1
	// Check current state
	__debugbreak_if( nullptr == m_parser )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Find registered include file
	const include_desc * res;
	if( find_include( res, *m_parser->get_current_source()->get_storage() ).failed_status( status ) )
		return status;

	// Set "once" flag
	const_cast<include_desc *>(res)->m_flags |= include_flags_t::fl_once_included;
#else
	// Check current state
	__debugbreak_if( nullptr == m_parser )
		return STATUS_ERROR_NOT_INITIALIZED;
	__debugbreak_if( nullptr == m_context )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Get full path to the path
	f1::core::string full_path;
	if( get_fs_context().get_full_pathname( full_path, m_parser->get_current_file_name() ).failed_status( status ) )
		return status;

#if 1
	// Check whether this file pathname was already added
	if( m_once_include_set.find( full_path ) != m_once_include_set.end() )
		return STATUS_WARNING_ALREADY_EXISTS;

	// Append path
	m_once_include_set.insert( full_path );
#else
	// Check whether this file pathname was already added
	// TO-DO-DONE: hash map
	// TO-DO-DONE: compare pathnames instead of strings to maintain case sensitivity specific for coreesponding FS
	array_foreach( core::Tarray<core::string>::const_iterator, it, m_once_include_list )
	{
		if( (*it).equal_nocase( full_path ) )
			return STATUS_WARNING_ALREADY_EXISTS;
	}

	// Append path
	m_once_include_list.append( full_path );
#endif
#endif

	// Exit
	return STATUS_SUCCESS;
}
bool f1::c_pp::parser::is_once_include( bool system, const char * name, size_t size ) const
{
	include_flags_t include_flags = system ? include_flags_t::fl_system : include_flags_t(0);
	return is_once_include( include_flags, name, size );
}
bool f1::c_pp::parser::is_once_include( include_flags_t include_flags, const char * name, size_t size ) const
{
#if 1
	// Get full path to the file
	core::string full_path_name;
	if( resolve_include_filename( full_path_name, core::stringslice(name, size), include_flags ).failed() )
		return false;

	// Find file descrptor
	const include_desc * inc_desc;
	if( find_include_full_path_name( inc_desc, full_path_name.c_str(), full_path_name.length() ).failed() )
		return false;

	// Check the flag and exit
	return !!(inc_desc->m_flags & include_flags_t::fl_once_included);
#else
	// Get full path to the path
	f1::core::string full_path;
	if( get_fs_context().get_full_pathname( full_path, name, size ).failed() )
		return false;

	// Find pathname
#if 1
	if( m_once_include_set.find( full_path ) == m_once_include_set.end() )
		return false;
#else
	// TO-DO-DONE: hash map
	// TO-DO-DONE: compare pathnames instead of strings to maintain case sensitivity specific for coreesponding FS
	array_foreach( core::Tarray<core::string>::const_iterator, it, m_once_include_list )
	{
		if( (*it).equal_nocase( full_path ) )
			return true;
	}
#endif
	return true;
#endif
}

void f1::c_pp::parser::check_include_dirs( fs::search_dir_list & list )
{
	array_foreach( core::Tarray<core::string>::const_iterator, it, list.get_search_path_list() )
		check_include_dir( *it );
}
void f1::c_pp::parser::check_include_dirs( int separator, const core::string & name )
{
	check_include_dirs( separator, name.data(), name.length() );
}
void f1::c_pp::parser::check_include_dirs( int separator, const char * name, size_t len )
{
	if( size_t(-1) == len )
		len = core::string::length( name );

	bool in_quotes = false;
	const char * data = name;
	const char * data_end = data + len;
	const char * start = data;
	for( ; data < data_end; ++ data )
	{
		if( '\"' == *data )
			in_quotes = !in_quotes;
		else if( separator == *data && !in_quotes && data != start )
			check_include_dir( start, data - start );
	}
	if( data != start )
		check_include_dir( start, data - start );
}
void f1::c_pp::parser::check_include_dir( const core::string & str )
{
	const char * name = str.data();
	size_t len = str.length();

	if( 0 != len && '\"' == *name )
		++ name,
		-- len;
	if( 0 != len && '\"' == name[len - 1] )
		-- len;

	if( !get_fs_context().exists( name, len ) )
		message( syntax::IMSG_WARNING_SEARCH_PATH_NOT_FOUND, str.c_str() );
}
void f1::c_pp::parser::check_include_dir( const char * name, size_t len )
{
	if( size_t(-1) == len )
		len = core::string::length( name );

	if( 0 != len && '\"' == *name )
		++ name,
		-- len;
	if( 0 != len && '\"' == name[len - 1] )
		-- len;

	if( !get_fs_context().exists( name, len ) )
		message( syntax::IMSG_WARNING_SEARCH_PATH_NOT_FOUND, core::string( name, len ).c_str() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Parsing

const status_t f1::c_pp::parser::parse_include_multi_filename( pp::string & buf, c_pp::parser::include_flags_t & include_flags, const char * & s, const char * end_s, const char * string_separator )
{
	status_t status;

	skip_real_spaces_and_comments( s, end_s );
	if( s >= end_s )
		return message( syntax::IMSG_ERROR_EXPECTED_INCLUDE_FILENAME );

	// Parse multiple components of path
	pp::string file_path_name;
	while( s < end_s )
	{
		if( '\"' != *s && '<' != *s )
			break;

		c_pp::parser::include_flags_t path_flags = null;
		pp::string file_part_name;
		if( parse_include_filename( file_part_name, path_flags, s, end_s ).failed_status( status ) )
			return status;
		if( file_part_name.is_empty() )
			break;

		if( file_path_name.empty() )
			include_flags = path_flags;
		else if( path_flags != include_flags )
			message( f1::c_pp::syntax::IMSG_WARNING_DIFFERENT_INCLUDE_TYPES );

		if( nullcstr != string_separator && !file_path_name.empty() )
			file_path_name.add_part( string_separator );
		file_path_name.add_part( file_part_name );

		skip_real_spaces_and_comments( s, end_s );
	}

	// Trim left and right spaces if present
	if( isspace( file_path_name.peek( 0 ) ) || isspace( file_path_name.rpeek( 0 ) ) )
	{
		const char * path_start = file_path_name.data();
		size_t path_length = file_path_name.length();
		pp::service::trim_spaces( path_start, path_length );

		file_path_name.clear();
		file_path_name.add_part( path_start, path_length );
	}

	// Check result
	buf.move( file_path_name );
	//if( buf.is_empty() )	// at least one part should be present
	//	return message( syntax::IMSG_ERROR_EXPECTED_INCLUDE_FILENAME );	//- already printed
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::parse_include_multi_filename( pp::string & buf, c_pp::parser::include_flags_t & include_flags, const char * string_separator )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	status_t status = parse_include_multi_filename( buf, include_flags, s, end_s, string_separator );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}

const status_t f1::c_pp::parser::parse_include_filename( pp::string & buf, c_pp::parser::include_flags_t & include_flags )
{
	status_t status;

	skip_real_spaces_and_comments();

	// Parse opening character and get the ending character
	int end_ch;
	if( m_parser->skip_char( '<' ) )
		end_ch = '>',
		include_flags = c_pp::parser::include_flags_t::fl_system;
	else if( m_parser->skip_char( '\"' ) )
		end_ch = '\"',
		include_flags = c_pp::parser::include_flags_t::fl_user;
	else
		return message( syntax::IMSG_ERROR_EXPECTED_INCLUDE_FILENAME );

	// Parse file name
	if( parse_string( buf, end_ch, string_parse_flags_t::fl_parse_string_no_escapes | string_parse_flags_t::fl_parse_string_ignore_unterm ).failed_status( status ) )
		return status;

	// Exit
	return status;
}
const status_t f1::c_pp::parser::parse_include_filename( pp::string & buf, c_pp::parser::include_flags_t & include_flags, const char *& s, const char * end_s )
{
	status_t status;

	skip_real_spaces_and_comments( s, end_s );

	// Parse opening character and get the ending character
	int end_ch;
	if( '<' == *s )
		end_ch = '>',
		include_flags = c_pp::parser::include_flags_t::fl_system;
	else if( '\"' == *s )
		end_ch = '\"',
		include_flags = c_pp::parser::include_flags_t::fl_user;
	else
		return message( syntax::IMSG_ERROR_EXPECTED_INCLUDE_FILENAME );
	++ s;

	// Parse file name
	// Ignore unterminated string errors because #include expands macros and has already reported about unterminated strings
	if( skip_string( s, end_s, end_ch, &buf, string_parse_flags_t::fl_parse_string_no_escapes | string_parse_flags_t::fl_parse_string_ignore_unterm ).failed_status( status ) )
		return status;

	if( buf.empty() )
		return message( syntax::IMSG_ERROR_EXPECTED_INCLUDE_FILENAME );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::resolve_include_filename( core::string & full_file_name, const pp::string & file_name, include_flags_t include_flags ) const
{
	return resolve_include_filename( full_file_name, file_name.get_stringslice(), include_flags );
}
const status_t f1::c_pp::parser::resolve_include_filename( core::string & full_file_name, const core::stringslice & file_name, include_flags_t include_flags ) const
{
	status_t status;

	// Get search directory list
	const fs::search_dir_list & search_list = get_search_dir_list(
		include_path_flags_t(include_flags & include_flags_t::fl_common)
		);

	// Get full path name
	core::string buf;
	if( include_flags & include_flags_t::fl_system && !core::pathname::is_absolute( file_name.data(), file_name.length() ) )
	{
		core::string full_file_path;
//		if( !system_path )
//			resolve_source_relative_file_path( full_file_path, file_name );
//		else
			full_file_path = file_name;

		if( search_list.find_file( buf, full_file_path ).failed_status( status ) )
		{
			if( /*system_path && */get_search_dir_list( include_path_flags_t::fl_user ).find_file( buf, full_file_path ).failed_status( status ) )
//			if( m_context->get_include_dirs().find_file( buf, file_name ).failed_status( status ) )
				return status;
		}
	}
	if( buf.empty() )
	{
		fs::path initial_pathname( m_parser->get_initial_file_name() );
		initial_pathname.remove_last();
		initial_pathname.combine( file_name.data(), file_name.length() );
		initial_pathname.move_to( buf );
	}

	// Get full path name
	core::string full_path;
	if( get_fs_context().get_full_pathname( full_path, buf.data(), buf.length() ).failed_status( status ) )
		return status;

	// Check the path is allowed
	if( !is_path_allowed( full_path ) )
		return STATUS_ERROR_ACCESS_DENIED;

	// Exit
	full_file_name.move( full_path );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::pass_on_include( include_flags_t include_flags, const pp::string & filename )
{
	bool system_path = !!(include_flags & include_flags_t::fl_system);

	// Output include directive with the filename specified by user
	output( "#include " );
	output( system_path ? "<" : "\"" );
	output( filename );
	output( system_path ? ">" : "\"" );

	// Output any additional comments that follow the #include directive
	pp::string buf;
	parse_to_newline( buf );
	output( buf );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::on_include_not_found( include_flags_t include_flags, const pp::string & filename )
{
	if( 0 != (m_c_flags & c_flags_t::fl_pass_invalid_includes) )
	{
		pass_on_include( include_flags, filename );
	}
	else
		message( syntax::IMSG_FATAL_ERROR_FILE_NOT_FOUND, filename.c_str() );
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::include_file( include_flags_t include_flags, const char * file_path_name, size_t len, size_t offset, include_desc ** inc_desc )
{
//	status_t status;

//	core::string full_file_path_name;
//	if( resolve_include_filename( full_file_path_name, core::stringslice(file_path_name, len), system_path ).failed_status( status ) )
//		return status;

//	return include_file( full_file_path_name, system_path, pp::string( file_path_name, len ), source );
	return include_file( include_flags, pp::string( file_path_name, len ), offset, inc_desc );
}
/*const status_t f1::c_pp::parser::include_file( const core::string & full_file_path_name, bool system_path, const char * file_path_name, size_t len, core::Tref<f1::parser::Isource> * source, include_desc ** inc_desc )
{
	return include_file( full_file_path_name, system_path, pp::string( file_path_name, len ), source, inc_desc );
}*/
const status_t f1::c_pp::parser::include_file( include_flags_t include_flags, const pp::string & file_path_name, size_t offset, c_pp::parser::include_desc ** inc_desc_ptr )
{
	status_t status;

	// Check for the first character to be a letter
	if( isdigit( file_path_name.peek( 0 ) ) )
		message( f1::c_pp::syntax::IMSG_WARNING_INCLUDE_PATH_DIGIT_STARTED );

	// Find wheter the file has already been loaded
	include_desc * inc_desc = nullptr;
	if( register_include( inc_desc, file_path_name.data(), file_path_name.length(), include_flags ).failed_status( status ) )
		return status;
	__debugbreak_if( nullptr == inc_desc )
		return STATUS_ERROR_INTERNAL;
	if( nullptr != inc_desc_ptr )
		*inc_desc_ptr = inc_desc;

	// Pass include file unmodified if required
	if( nullptr != inc_desc && (inc_desc->m_flags & parser::include_flags_t::fl_pass_unmodified) )
		return pass_on_include( include_flags, pp::string(file_path_name) );

	// If this file was once-included, silently quit
	//if( c_pp_par->is_once_include( system_path, full_file_path_name.data(), full_file_path_name.size() ) )
	if( nullptr != inc_desc && (inc_desc->m_flags & parser::include_flags_t::fl_once_included) )
		return STATUS_SUCCESS;

	// Exit
	return include_file( *inc_desc, offset );
}
const status_t f1::c_pp::parser::include_file( const include_desc & inc_desc, size_t offset )
{
	status_t status;

	// Display include file name
	if( m_context->get_flags( f1::pp::context::fl_show_includes ) )
		message( f1::c_pp::syntax::IMSG_RAW_INDENT, m_parser->get_nesting_level(), inc_desc.m_full_path_name.c_str() );

	// Create new source object
	f1::core::Tref<f1::parser::Istorage> file = inc_desc.m_storage;
	if( nullptr == file )
	{
		if( get_parser_context().create_file_storage( file, get_fs_context(), inc_desc.m_full_path_name.c_str(), inc_desc.m_full_path_name.length() ).failed_status( status ) )
			return status;
		__debugbreak_if( nullptr == file )
			return STATUS_ERROR_INTERNAL;
		set_include_file( *const_cast<include_desc *>(&inc_desc), *file );
	}

	// Check file for deprecation
	if( inc_desc.m_flags & include_flags_t::fl_deprecated )
		message( syntax::IMSG_WARNING_DEPRECATED_INCLUDE_S, inc_desc.m_full_path_name.c_str() );

	// Create source
	f1::core::Tref<f1::parser::Isource> src;
	get_parser_context().create_source( src, *file );	// TODO: use create_file_source

	// Include source
	if( m_parser->insert_source( *src, offset ).failed_status( status ) )
		return status;
	on_filename_changed();

	// Update once-inclusion information
	if( get_c_flags( c_pp::parser::c_flags_t::fl_auto_once_includes ) )
	{
		register_once_include();
	}

	// Exit
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::parser::append_allowed_base_dir( const core::static_string & base_path )
{
	return m_allowed_base_dirs.append_unique( base_path );
}
const status_t f1::c_pp::parser::remove_allowed_base_dir( const core::static_string & base_path )
{
	return m_allowed_base_dirs.remove( base_path );
}
const status_t f1::c_pp::parser::remove_all_allowed_base_dirs()
{
	return m_allowed_base_dirs.remove_all();
}
const f1::core::Tarray<f1::fs::path> & f1::c_pp::parser::get_allowed_base_dirs() const
{
	return m_allowed_base_dirs;
}

const status_t f1::c_pp::parser::append_disallowed_base_dir( const core::static_string & base_path )
{
	return m_disallowed_base_dirs.append_unique( base_path );
}
const status_t f1::c_pp::parser::remove_disallowed_base_dir( const core::static_string & base_path )
{
	return m_disallowed_base_dirs.remove( base_path );
}
const status_t f1::c_pp::parser::remove_all_disallowed_base_dirs()
{
	return m_disallowed_base_dirs.remove_all();
}
const f1::core::Tarray<f1::fs::path> & f1::c_pp::parser::get_disallowed_base_dirs() const
{
	return m_disallowed_base_dirs;
}

bool f1::c_pp::parser::is_path_allowed( const core::static_string & full_path ) const
{
	if( !m_allowed_base_dirs.empty() || !m_disallowed_base_dirs.empty() )
	{
		fs::path path( full_path );

		array_foreach( fs::path_list_t::const_iterator, it, m_allowed_base_dirs )
		{
			if( !path.is_base_path( *it ) )
				return false;
		}

		array_foreach( fs::path_list_t::const_iterator, it, m_disallowed_base_dirs )
		{
			if( path.is_base_path( *it ) )
				return false;
		}
	}
	return true;
}

/*END OF parser_include.cxx*/
