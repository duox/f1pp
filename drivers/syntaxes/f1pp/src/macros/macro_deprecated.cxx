/*macro_deprecated.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::f1pp::macro_deprecated::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_self_parse_arglist | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined | macro_flags_t::fl_expression_only ).failed_status( status ) )
		return status;

	// Initialize object

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::f1pp::macro_deprecated::get_name() const
{
	return "deprecated";
}

const status_t f1::f1pp::macro_deprecated::parse( pp::Iparser & parser, pp::string & result ) const
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
		parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		return STATUS_SUCCESS;
	}
	else
	{
		// Find the macro and test whether it's deprecated
		bool deprecated = false;
		core::Tref<pp::Imacro> mac;
		if( c_pp_par->find_macro( mac, ident ).succeeded() && nullptr != mac )
		{
			deprecated = !!mac->get_flags( Imacro::macro_flags_t::fl_deprecated );
			mac->on_macro_referenced();
		}
		result.add_part( deprecated ? "1" : "0" );
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::macro_deprecated::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	DBG_FAIL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( buf );
	__unreferenced_parameter( args );
	return STATUS_ERROR_NOT_SUPPORTED;
}

/*END OF macro_deprecated.cxx*/
