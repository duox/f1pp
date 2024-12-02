/*include_directive.cxx*/
/** @file
 *
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::c99::include_directive::include_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::c99::include_directive::get_name() const
{
	return "include";
}
// {612163F2-2FCE-407A-8B03-DF4F70FB47D4}
const f1::guid_t f1::c_pp::c99::include_directive::m_guid =
{ 0x612163f2, 0x2fce, 0x407a, { 0x8b, 0x03, 0xdf, 0x4f, 0x70, 0xfb, 0x47, 0xd4 } };
const f1::guid_t & f1::c_pp::c99::include_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::c99::include_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return cap_keep_text_eol & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::c99::include_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::c_pp::c99::include_directive> drc = type_cast<f1::c_pp::c99::include_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::c_pp::c99::include_directive *>( param )->execute( parser, buf );
#endif
}

const status_t f1::c_pp::c99::include_directive::parse( pp::Iparser & parser )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Skip spaces and check for EOL
	c_pp_par->skip_real_spaces_and_comments();
	if( c_pp_par->get_parser()->is_nl() )
		return c_pp_par->message( f1::c_pp::syntax::IMSG_ERROR_EXPECTED_INCLUDE_FILENAME );

	// Capture line
	c_pp_par->modify_c_flags( c_pp::parser::c_flags_t::fl_disable_string_escapes );
	return c_pp_par->capture_line( line_capture_callback, this );
}
const status_t f1::c_pp::c99::include_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;
	c_pp_par->modify_c_flags( c_pp::parser::c_flags_t::null, c_pp::parser::c_flags_t::fl_disable_string_escapes );

	// Exit
	const char * s = (const char *) out_buf.get_ptr();
	return execute( *c_pp_par, s, (const char *) out_buf.get_end_ptr() );
}
const status_t f1::c_pp::c99::include_directive::execute( f1::c_pp::parser & c_pp_par, const char * & s, const char * end_s )
{
	status_t status;

	// Check current state
	core::Tref<f1::parser::Iparser> par = c_pp_par.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tconst_ref<f1::pp::context> ctx = c_pp_par.get_context();
	__debugbreak_if( nullptr == ctx )
		return STATUS_ERROR_NOT_INITIALIZED;

	if( nullcstr == s )
		return STATUS_SUCCESS;

	// Parse include file name
	c_pp::parser::include_flags_t include_flags = null;
	pp::string file_path_name;
	if( parse_include_filename( c_pp_par, file_path_name, include_flags, s, end_s ).failed_status( status ) )
		return STATUS_SUCCESS;
	if( nullptr == file_path_name.data() )
	{
		if( s < end_s )
			return c_pp_par.message( f1::c_pp::syntax::IMSG_ERROR_EXPECTED_INCLUDE_FILENAME );
		return STATUS_SUCCESS;
		//return c_pp_par.message( f1::c_pp::syntax::IMSG_FATAL_ERROR_INVALID_FILE_NAME );
	}

	// Include file
	if( include_file( c_pp_par, file_path_name, include_flags ).failed_status( status ) )
		return on_include_not_found( c_pp_par, file_path_name, include_flags );

	// Update state
	//update_once_include( c_pp_par );			// update once-inclusion information
	on_complete( c_pp_par, s, end_s );			// complete the directive
	check_for_newline( c_pp_par, s, end_s );	// check for end of directive

	// Exit
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hooks

const status_t f1::c_pp::c99::include_directive::parse_include_filename( f1::c_pp::parser & par, pp::string & buf, c_pp::parser::include_flags_t & include_flags, const char *& s, const char * end_s )
{
	return par.parse_include_multi_filename( buf, include_flags, s, end_s );
}
/*const status_t f1::c_pp::c99::include_directive::include_file( f1::c_pp::parser & par, c_pp::parser::include_desc & inc_desc )
{
	return par.include_file( inc_desc );
}*/
const status_t f1::c_pp::c99::include_directive::include_file( f1::c_pp::parser & par, const pp::string & file_path_name, c_pp::parser::include_flags_t include_flags, c_pp::parser::include_desc ** inc_desc_ptr )
{
	return par.include_file( include_flags, file_path_name, size_t(-1), inc_desc_ptr );
}
const status_t f1::c_pp::c99::include_directive::on_include_not_found( f1::c_pp::parser & par, const pp::string & file_path_name, c_pp::parser::include_flags_t include_flags )
{
	return par.on_include_not_found( include_flags, file_path_name );
}
/*const status_t f1::c_pp::c99::include_directive::update_once_include( f1::c_pp::parser & par )
{
	if( par.get_c_flags( c_pp::parser::c_flags_t::fl_auto_once_includes ) )
	{
		par.register_once_include();
	}
	return STATUS_SUCCESS;
}*/
const status_t f1::c_pp::c99::include_directive::on_complete( f1::c_pp::parser & par, const char *& s, const char * end_s )
{
	__unreferenced_parameter( par );
	__unreferenced_parameter( s );
	__unreferenced_parameter( end_s );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c99::include_directive::check_for_newline( f1::c_pp::parser & par, const char *& s, const char * end_s )
{
	par.skip_real_spaces_and_comments( s, end_s );
	if( s < end_s )
		on_unfinished_line( par, s, end_s );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c99::include_directive::on_unfinished_line( f1::c_pp::parser & par, const char *& s, const char * end_s )
{
	par.message( f1::c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );
	__unreferenced_parameter( s );
	__unreferenced_parameter( end_s );
	return STATUS_SUCCESS;
}

/*END OF include_directive.cxx*/
