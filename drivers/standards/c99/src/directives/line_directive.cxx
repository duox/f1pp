/*line_directive.cxx*/
/** @file
 *
 *
 * #line line "linename"
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::c99::line_directive::line_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::c99::line_directive::get_name() const
{
	return "line";
}
// {381734EB-7C7F-4d99-A937-CD35219D651C}
const f1::guid_t f1::c_pp::c99::line_directive::m_guid =
{ 0x381734eb, 0x7c7f, 0x4d99, { 0xa9, 0x37, 0xcd, 0x35, 0x21, 0x9d, 0x65, 0x1c } };
const f1::guid_t & f1::c_pp::c99::line_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::c99::line_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::c99::line_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#if 1 //def _DEBUG
	core::Tref<f1::c_pp::c99::line_directive> drc = type_cast<f1::c_pp::c99::line_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, pp::string(buf) );
#else
	return static_cast<f1::c_pp::c99::line_directive *>( param )->execute( parser, buf );
#endif
}

const status_t f1::c_pp::c99::line_directive::parse( pp::Iparser & parser )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_par = type_cast<f1::c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == c_par )
		return STATUS_ERROR_INVALID_TYPE;

	// Start capturing preprocessed line and exit
	return c_par->capture_line( line_capture_callback, this );
}

const status_t f1::c_pp::c99::line_directive::execute( pp::Iparser & parser, const pp::string & buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	const char * s = (const char *) buf.data();
	const char * const end_s = (const char *) buf.data_end();

	// Fetch line number
	pp::variant line_no;
	bool parsed = false;
	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	parsed = c_pp_par->parse_integer( line_no, s, end_s );
	if( !parsed )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_INTEGER );
	if( 0 >= line_no.as_int() )
		return parser.message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );

	// Parse file name if it present
	pp::string filename;
	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	if( s < end_s )
	{
		if( '\"' == *s )
		{
			++ s;
			if( c_pp_par->skip_string( s, end_s, '\"', &filename, c_pp::parser::string_parse_flags_t::fl_parse_string_no_escapes | c_pp::parser::string_parse_flags_t::fl_parse_string_ignore_unterm ).failed_status( status ) )
				return status;
		}
		else
			filename.add_part( s, end_s - s );
		if( filename.empty() )
			return c_pp_par->message( syntax::IMSG_ERROR_EXPECTED_FILENAME );
	}

	// Set current line
	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	f1::parser::lineno_t line = f1::parser::lineno_t(line_no.as_int()) - 1;
	if( !filename.empty() )
	{
#if 1
		par->set_current_file_name( filename, line );
#else
		f1::parser::Isource * src = par->get_current_source();
		if( nullptr != src )
			src->set_filename( filename.c_str() );
#endif
	}
	else
		par->set_line( line );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF line_directive.cxx*/
