/*include_next_directive.cxx*/
/** @file
 *
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::comext::include_next_directive::include_next_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::comext::include_next_directive::get_name() const
{
	return "include_next";
}
// {069D8FC9-5B49-45A9-9620-1574F26055EF}
const f1::guid_t f1::c_pp::comext::include_next_directive::m_guid =
{ 0x069d8fc9, 0x5b49, 0x45a9, { 0x96, 0x20, 0x15, 0x74, 0xf2, 0x60, 0x55, 0xef } };
const f1::guid_t & f1::c_pp::comext::include_next_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::comext::include_next_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return cap_keep_text_eol & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::comext::include_next_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::c_pp::comext::include_next_directive> drc = type_cast<f1::c_pp::comext::include_next_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::c_pp::comext::include_next_directive *>( param )->execute( parser, buf );
#endif
}

const status_t f1::c_pp::comext::include_next_directive::parse( pp::Iparser & parser )
{
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;
	c_pp_par->modify_c_flags( c_pp::parser::c_flags_t::fl_disable_string_escapes );

	return c_pp_par->capture_line( line_capture_callback, this );
}
const status_t f1::c_pp::comext::include_next_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;
	c_pp_par->modify_c_flags( c_pp::parser::c_flags_t::null, c_pp::parser::c_flags_t::fl_disable_string_escapes );

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tconst_ref<f1::pp::context> ctx = parser.get_context();
	__debugbreak_if( nullptr == ctx )
		return STATUS_ERROR_NOT_INITIALIZED;

	const char * s = (const char *) out_buf.get_byte_ptr();
	const char * const end_s = s + out_buf.get_data_size();

	// Parse include file name
	c_pp::parser::include_flags_t include_flags = null;
	pp::string file_path_name;
	if( c_pp_par->parse_include_multi_filename( file_path_name, include_flags, s, end_s ).failed_status( status ) )
		return STATUS_SUCCESS;
	if( nullptr == file_path_name.data() )
		return c_pp_par->message( f1::c_pp::syntax::IMSG_FATAL_ERROR_INVALID_FILE_NAME );
	core::pathname file_name( file_path_name.data(), file_path_name.length() );

	// For each entry in the search directory list, check whether file on this entry has already been included
	const f1::parser::Isource * src = nullptr;
	core::string full_file_name;
	if( !(include_flags & c_pp::parser::include_flags_t::fl_system) )
	{
		// Generate full file name
		if( parser.resolve_source_relative_file_path( full_file_name, file_name.get_string() ).failed() )
			return c_pp_par->message( f1::c_pp::syntax::IMSG_FATAL_ERROR_INVALID_FILE_NAME );

		// Find the file in the list of included sources
		for( src = par->get_current_source(); nullptr != src; src = src->get_parent() )
		{
			if( full_file_name.equal_nocase( src->get_filename() ) )
				break;
		}
	}

	if( include_flags & c_pp::parser::include_flags_t::fl_system || nullptr != src )
	{
		//array_foreach( core::Tarray<core::string>::const_iterator, it, ctx->get_include_dirs().get_search_path_list() )
		array_foreach( core::Tarray<core::string>::const_iterator, it, parser.get_search_dir_list( pp::Iparser::include_path_flags_t::fl_system ).get_search_path_list() )
		{
			const core::string & path = *it;

			// Get full path to the file
			core::string path_string;
			if( parser.get_fs_context().get_full_pathname( path_string, path.data(), path.length() ).failed() )
				continue;
			file_name.get_full_path( full_file_name, path_string.c_str() );

			if( !parser.get_fs_context().exists( full_file_name ) )
				continue;

			// Find the file in the list of included sources
			for( src = par->get_current_source(); nullptr != src; src = src->get_parent() )
			{
				if( full_file_name.equal_nocase( src->get_filename() ) )
					break;
			}
			if( nullptr == src )
				break;
		}
	}

	// File has not been yet included, include it now
	if( nullptr == src )
	{
		return c_pp_par->include_file( include_flags, full_file_name.c_str() );
	}

	// Ignore directive if no more files found
	// Exit
	return STATUS_SUCCESS;
}

/*END OF include_next_directive.cxx*/
