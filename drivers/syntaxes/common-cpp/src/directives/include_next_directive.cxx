/*include_next_directive.cxx*/
/** @file
 *
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::include_next_directive::include_next_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::include_next_directive::get_name() const
{
	return "include_next";
}
// {4AA3B5D3-59B8-4121-98A4-C5047061BEE8}
const f1::guid_t f1::c_pp::include_next_directive::m_guid =
{ 0x4aa3b5d3, 0x59b8, 0x4121, { 0x98, 0xa4, 0xc5, 0x4, 0x70, 0x61, 0xbe, 0xe8 } };
const f1::guid_t & f1::c_pp::include_next_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::include_next_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::include_next_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tconst_ref<f1::pp::context> ctx = parser.get_context();
	__debugbreak_if( nullptr == ctx )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Parse include_next file name and get full pathname to file
	bool system_path = false;
	pp::string file_path_name;
	if( c_pp_par->parse_include_next_filename( file_path_name, system_path ).failed_status( status ) || file_path_name.is_empty() )
		return STATUS_SUCCESS;

	core::string full_file_path_name;
	if( c_pp_par->resolve_include_next_filename( full_file_path_name, file_path_name, system_path ).failed_status( status ) )
		return c_pp_par->on_include_next_not_found( file_path_name, system_path );

	c_pp::parser::file_desc * file_desc = nullptr;
	c_pp_par->find_file( file_desc, full_file_path_name.data(), full_file_path_name.size() );	// try to find file descriptor

	// Display include_next file name
	if( ctx->get_flags( f1::pp::context::fl_show_include_nexts ) )
		parser.message( f1::c_pp::syntax::IMSG_RAW_INDENT, par->get_nesting_level(), full_file_path_name.c_str() );

	// Check end of directive is empty
	size_t cur_pos = par->get_pos();
	c_pp_par->skip_to_newline();
	if( cur_pos != par->get_pos() )
		return parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );
	par->skip_nl();

	// Pass include_next file unmodified if required
	if( nullptr != file_desc && (file_desc->m_flags & parser::file_flags_t::fl_pass_unmodified) )
		return c_pp_par->pass_on_include_next( file_path_name, system_path );

	// If this file was once-include_nextd, silently quit
	//if( c_pp_par->is_once_include_next( system_path, full_file_path_name.data(), full_file_path_name.size() ) )
	if( nullptr != file_desc && (file_desc->m_flags & parser::file_flags_t::fl_once_include_nextd) )
		return STATUS_SUCCESS;

	// Create new source object
	f1::core::Tref<f1::parser::Isource> file;
	if( parser.get_parser_context().create_file_source( file, parser.get_fs_context(), full_file_path_name.c_str() ).failed_status( status ) )
		return c_pp_par->on_include_next_not_found( file_path_name, system_path );

	// Include source
	if( par->insert_source( *file ).failed_status( status ) )
		return c_pp_par->on_include_next_not_found( file_path_name, system_path );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF include_next_directive.cxx*/
