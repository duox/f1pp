/*macro_option.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::f1pp::macro_option::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Check current state
	core::Tref<pp::param_list> pars;
	if( nullptr == params )
	{
		pars.createT<pp::param_list>();
		pars->append_param( 0, "opt" );
		params = pars;
	}

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined | macro_flags_t::fl_expression_only ).failed_status( status ) )
		return status;

	// Initialize object

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::f1pp::macro_option::get_name() const
{
	return "option";
}

const status_t f1::f1pp::macro_option::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	status_t status;

	// Check current state
	__debugbreak_if( nullptr == args || 1 != args->get_arg_list().size() )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "option( name )" );

	core::Tref<f1::c_pp::parser> cpp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == cpp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Get option name
	pp::string ident;
	const pp::arg_list::arg_desc_t & arg = *args->get_arg_list().begin();
	if( cpp_par->unstringify_chars( ident, arg.m_expanded_value.is_empty() ? arg.m_value : arg.m_expanded_value ).failed_status( status ) )
		return status;

	// Find option
	core::Tref<pp::Ioption> opt;
	if( parser.get_context()->find_option( opt, ident.c_str(), ident.length() ).failed() )
	{
		buf.add_part_alloc( "0" );
		return STATUS_SUCCESS;
	}

	// Store option value
	pp::string value;
	opt->get_value().format( value );
	buf.add_part_alloc( value );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF macro_option.cxx*/
