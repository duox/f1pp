/*macro_defined.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::c_pp::c99::macro_defined::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_self_parse_arglist | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined | macro_flags_t::fl_expression_only | macro_flags_t::fl_operator ).failed_status( status ) )
		return status;

	// Initialize object
//	core::Tref<ns::name> nm;
//	core::Tref<ns::name_space> ns = &parser.get_ns_context()->get_root_namespace();
//	if( create_name( nm, *ns, 0, "defined" ).failed_status( status ) )
//		return status;


	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::c_pp::c99::macro_defined::get_name() const
{
	return "defined";
}

const status_t f1::c_pp::c99::macro_defined::parse( pp::Iparser & parser, pp::string & result ) const
{
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

	// Fetch macro identifier
	par->skip_real_spaces_and_comments();

	pp::string ident;
	c_pp_par->parse_macro_identifier( ident );

	// Skip close bracket if needed
	par->skip_real_spaces_and_comments();
	if( parenthesis && !par->skip_char( ')' ) )
		parser.message( syntax::IMSG_ERROR_EXPECTED_S, "')'" );

	// Find the macro
	if( ident.is_empty() )
	{
		result.add_part_alloc( "0" );
		parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		return STATUS_SUCCESS;
	}
	else
	{
		core::Tref<pp::Imacro> mac = nullptr;
		parser.find_macro( mac, ident.get_stringslice() );

		if( nullptr != mac && mac->get_flags( pp::Imacro::macro_flags_t::fl_expression_only ) )
			mac = nullptr;
		result.add_part_alloc( nullptr != mac ? "1" : "0" );
		if( nullptr != mac )
			parser.on_macro_expanded( *mac );
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c99::macro_defined::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	DBG_FAIL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( buf );
	__unreferenced_parameter( args );
	return STATUS_ERROR_NOT_SUPPORTED;
}

/*END OF macro_defined.cxx*/
