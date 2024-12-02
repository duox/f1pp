/*macro_has_include.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::c_pp::cpp17::macro_has_include::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_self_parse_arglist | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined ).failed_status( status ) )
		return status;

	// Initialize object

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::c_pp::cpp17::macro_has_include::get_name() const
{
	return "__has_include";
}

const status_t f1::c_pp::cpp17::macro_has_include::parse( pp::Iparser & parser, pp::string & result ) const
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	result.add_part( "0" );

	// Fetch macro identifier
#if 1
	if( !par->skip_char( '(' ) )
	{
		c_pp_par->skip_to_newline();
		return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "')'" );
	}
	c_pp_par->skip_real_spaces_and_comments();

	pp::string filename;
	c_pp::parser::include_flags_t include_flags = null;
	status = c_pp_par->parse_include_multi_filename( filename, include_flags );
	if( filename.is_empty() )
	{
		c_pp_par->skip_to_newline();
		return c_pp_par->message( f1::c_pp::syntax::IMSG_ERROR_EXPECTED_INCLUDE_FILENAME );
	}

	c_pp_par->skip_real_spaces_and_comments();
	if( !par->skip_char( ')' ) )
		parser.message( syntax::IMSG_ERROR_EXPECTED_S, "')'" );
#else
	par->skip_char( '(' );
	par->skip_real_spaces_and_comments();

	pp::string str;
	c_pp_par->parse_delimited_string( str, ')' );
	if( !par->skip_char( ')' ) )
		return par.message( syntax::IMSG_ERROR_EXPECTED_S, ")" );

	const char * s = str.data();
	const char * end_s = str.data_end();

	pp::string filename;
	c_pp::parser::include_flags_t include_flags = null;
	status = c_pp_par->parse_include_multi_filename( filename, include_path_flags_t, s, end_s );
	if( filename.is_empty() )
		return status;
#endif

	// Find the include file
	result.clear();
	if( c_pp_par->get_fs_context().exists( filename.c_str(), filename.size() ) )
		result.add_part( "1" );
	else
		result.add_part( "0" );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::cpp17::macro_has_include::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	DBG_FAIL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( buf );
	__unreferenced_parameter( args );
	return STATUS_ERROR_NOT_SUPPORTED;
}

/*END OF macro_has_include.cxx*/
