/*file_directive.cxx*/
/** @file
 *
 *
 * #line line "linename"
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::comext::file_directive::file_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::comext::file_directive::get_name() const
{
	return "file";
}
// {81DD7BA4-4A0F-46CC-B902-58A8A949C1F4}
const f1::guid_t f1::c_pp::comext::file_directive::m_guid =
{ 0x81dd7ba4, 0x4a0f, 0x46cc, { 0xb9, 0x02, 0x58, 0xa8, 0xa9, 0x49, 0xc1, 0xf4 } };
const f1::guid_t & f1::c_pp::comext::file_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::comext::file_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::comext::file_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#if 1 //def _DEBUG
	core::Tref<f1::c_pp::comext::file_directive> drc = type_cast<f1::c_pp::comext::file_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, pp::string(buf) );
#else
	return static_cast<f1::c_pp::comext::file_directive *>( param )->execute( parser, buf );
#endif
}

const status_t f1::c_pp::comext::file_directive::parse( pp::Iparser & parser )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_par = type_cast<f1::c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == c_par )
		return STATUS_ERROR_INVALID_TYPE;

	// Start capturing preprocessed line and exit
	return c_par->capture_line( line_capture_callback, this );
}

const status_t f1::c_pp::comext::file_directive::execute( pp::Iparser & parser, const pp::string & buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	const char * s = (const char *) buf.data();
	const char * const end_s = (const char *) buf.data_end();

	// Parse file name if it present
	pp::string filename;
	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	if( s < end_s )
	{
		if( '\"' == *s )
		{
			++ s;
			if( c_pp_par->skip_string( s, end_s, '\"', &filename, c_pp::parser::string_parse_flags_t::fl_parse_string_ignore_unterm ).failed_status( status ) )
				return status;
		}
		else
			filename.add_part( s, end_s - s );
	}
	if( filename.empty() )
		return c_pp_par->message( syntax::IMSG_ERROR_EXPECTED_FILENAME );

	// Set current file
	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	par->set_current_file_name( filename );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF file_directive.cxx*/
