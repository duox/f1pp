/*macro_has_option.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::f1pp::macro_has_option::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Check current state
	core::Tref<pp::param_list> pars;
	if( nullptr == params )
	{
		pars.createT<pp::param_list>();
		pars->append_param( 0, "name" );
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

const char * f1::f1pp::macro_has_option::get_name() const
{
	return "__has_option";
}

const status_t f1::f1pp::macro_has_option::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	status_t status;

	// Check current state
	__debugbreak_if( 1 != args->get_arg_list().size() )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "__has_include( filename )" );

	core::Tref<f1::c_pp::parser> cpp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == cpp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Get macro value
	pp::string val;
	const pp::arg_list::arg_desc_t & arg = *args->get_arg_list().begin();
	if( cpp_par->unstringify_chars( val, arg.m_expanded_value.is_empty() ? arg.m_value : arg.m_expanded_value ).failed_status( status ) )
		return status;

	const char * s = val.data();
	const char * end = val.data_end();

	// Skip open bracket if exists
	cpp_par->skip_real_spaces_and_comments( s, end );
	bool parenthesis = '(' == *s;
	if( parenthesis )
		++ s;

	// Fetch directive name
	cpp_par->skip_real_spaces_and_comments( s, end );

	pp::string ident;
	cpp_par->parse_macro_identifier( ident, s, end );	// try to parse simple identifier first
	if( ident.empty() )	// try to parse strings
	{
		cpp_par->parse_string( ident, s, end );
		if( ident.empty() )
			parser.message( syntax::IMSG_ERROR_EXPECTED_DIRECTIVE_NAME );
		else if( parenthesis )
		{
			for(;;)
			{
				cpp_par->skip_real_spaces_and_comments( s, end );
				if( ')' == *s )
					break;
				pp::string part;
				cpp_par->parse_string( part );
				ident.add_part_alloc( part );
			}
		}
	}

	// Skip close bracket if needed
	cpp_par->skip_real_spaces_and_comments( s, end );
	if( parenthesis )
	{
		if( ')' != *s )
			parser.message( syntax::IMSG_ERROR_EXPECTED_S, "')'" );
		else
			++ s;
	}

	// Find the directive
	if( !ident.is_empty() )
	{
		core::Tref<pp::Ioption> opt = nullptr;
		parser.get_context()->find_option( opt, ident );
		buf.add_part_alloc( nullptr != opt ? "1" : "0" );
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF macro_has_option.cxx*/
