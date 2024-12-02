/*macro_has_embed.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::c_pp::c23::macro_has_embed::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_self_parse_arglist | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined | macro_flags_t::fl_expression_only ).failed_status( status ) )
		return status;

	// Initialize object
//	core::Tref<ns::name> nm;
//	core::Tref<ns::name_space> ns = &parser.get_ns_context()->get_root_namespace();
//	if( create_name( nm, *ns, 0, "__has_embed" ).failed_status( status ) )
//		return status;


	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::c_pp::c23::macro_has_embed::get_name() const
{
	return "__has_embed";
}

const status_t f1::c_pp::c23::macro_has_embed::parse( pp::Iparser & parser, pp::string & result ) const
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Skip open bracket if exists
	par->skip_real_spaces_and_comments();
	bool parenthesis = par->skip_char( '(' );

	// Fetch file name
	embed_directive::resource_state	state;
	if( c_pp_par->parse_include_multi_filename( state.m_file_name, state.m_include_flags ).failed_status( status ) )
		return status;

	// Process parameters
	__debugbreak_ifnot( nullptr != c_pp::c23::embed_directive::m_instance )
	{
		if( c_pp::c23::embed_directive::m_instance->parse_parameter_list( state, *c_pp_par ).failed_status( status ) )
			return status;
	}

	// Skip closing bracket if exists
	par->skip_real_spaces_and_comments();
	if( parenthesis && !par->skip_char( ')' ) )
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, ")" );

	// Now perform resource check
	if( execute( *c_pp_par, state, result ).failed_status( status ) )
		return status;

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c23::macro_has_embed::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	DBG_FAIL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( buf );
	__unreferenced_parameter( args );
	return STATUS_ERROR_NOT_SUPPORTED;
}
const status_t f1::c_pp::c23::macro_has_embed::execute( f1::c_pp::parser & parser, const embed_directive::resource_state & state, pp::string & result ) const
{
	// Find the resource
	f1::io::file_istream is( state.m_file_name.get_string() );
	if( is.bad() )
	{
		result.add_part( "__STDC_EMBED_NOT_FOUND__" );
		return STATUS_SUCCESS;
	}

	// Check the resource size
	if( 0 == state.m_length || 0 == is.get_size() )
		result.add_part( "__STDC_EMBED_EMPTY__" );
	else
		result.add_part( "__STDC_EMBED_FOUND__" );

	// Exit
	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}

/*END OF macro_has_embed.cxx*/
