/*includepath_directive.cxx*/
/** @file
 *
 * TODO:
 * - #includepath in style of GENER8: without quotes
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::comext::includepath_directive::includepath_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::comext::includepath_directive::get_name() const
{
	return "includepath";
}
// {45C67E68-F2E2-4f16-8508-94870429950E}
const f1::guid_t f1::c_pp::comext::includepath_directive::m_guid =
{ 0x45c67e68, 0xf2e2, 0x4f16, { 0x85, 0x8, 0x94, 0x87, 0x4, 0x29, 0x95, 0xe } };
const f1::guid_t & f1::c_pp::comext::includepath_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::comext::includepath_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return cap_keep_text_eol & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::comext::includepath_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::c_pp::comext::includepath_directive> drc = type_cast<f1::c_pp::comext::includepath_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::c_pp::comext::includepath_directive *>( param )->execute( parser, buf );
#endif
}

const status_t f1::c_pp::comext::includepath_directive::parse( pp::Iparser & parser )
{
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;
	c_pp_par->modify_c_flags( c_pp::parser::c_flags_t::fl_disable_string_escapes );

	return c_pp_par->capture_line( line_capture_callback, this );
}
const status_t f1::c_pp::comext::includepath_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
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

	core::Tref<f1::pp::context> ctx = parser.get_context();
	__debugbreak_if( nullptr == ctx )
		return STATUS_ERROR_NOT_INITIALIZED;

#if 1
	// Parse include file name
	pp::string file_path_name;
	pp::Iparser::include_path_flags_t include_flags;
	{
		const char * s = (const char *) out_buf.get_ptr();
		const char * end_s = (const char *) out_buf.get_end_ptr();
		c_pp::parser::include_flags_t include_flags = null;
		if( c_pp_par->parse_include_multi_filename( file_path_name, include_flags, s, end_s, ";" ).failed_status( status ) )
			return STATUS_SUCCESS;
		if( nullptr == file_path_name.data() || include_flags & c_pp::parser::include_flags_t::fl_system )
		{
			parser.message( f1::c_pp::syntax::IMSG_FATAL_ERROR_INVALID_FILE_NAME );
			return STATUS_SUCCESS;
		}
	}

	// Parse path sequence
	const char * pos = file_path_name.data();
	const char * end_data = file_path_name.data_end();
	for( ;;)
	{
		// Skip leading spaces
		while( pos < end_data && isspace( *pos ) )
			++ pos;
		if( pos >= end_data )
			break;

		// Get path separator
		const char * end_pos = _strnchr( pos, end_data - pos, ';' );
		if( nullcstr == end_pos )
			end_pos = end_data;

		// Skip trailing spaces
		const char * rpos = end_pos;
		while( pos <= rpos && isspace( rpos[-1] ) )
			-- rpos;

		// Append search path
		parser.append_include_path( include_flags, pos, rpos - pos );
		if( end_pos >= end_data )
			break;

		// Iterate
		pos = end_pos + 1;
	}
#else
	// Add all include file names
	pp::string filenames;
	c_pp_par->unstringify_chars( filenames, pp::string( out_buf ) );
	parser.get_search_dir_list().append_search_paths( ';', filenames.data(), filenames.size() );
		// reinterpret_cast<const char *>(out_buf.get_ptr()), out_buf.get_data_size() );
	c_pp_par->check_include_dirs( ';', filenames.data(), filenames.length() );
#endif

	// Exit
	return STATUS_SUCCESS;
}

/*END OF includepath_directive.cxx*/
