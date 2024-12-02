/*macro.cxx*/
#include "pch.h"
#include "main.h"

f1::f1pp::macro::macro( const core::Iidentifier * ident ):
	super( ident ),
	m_ex_flags( null )
{
	m_multiline_patch_callback.initialize( *this );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const status_t f1::f1pp::macro::modify_ex_flags( ex_flags_t flags_to_set, ex_flags_t flags_to_clear, ex_flags_t * flags_prev )
{
	if( nullptr != flags_prev )
		*flags_prev = m_ex_flags;

	m_ex_flags = (m_ex_flags | flags_to_set) & ~flags_to_clear;

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::macro::modify_ex_flags( ex_flags_t flags, bool set_or_clear, ex_flags_t * flags_prev )
{
	if( nullptr != flags_prev )
		*flags_prev = m_ex_flags;

	if( set_or_clear )
		m_ex_flags |= flags;
	else
		m_ex_flags &= ~flags;

	return STATUS_SUCCESS;
}
f1::f1pp::macro::ex_flags_t f1::f1pp::macro::get_ex_flags( ex_flags_t mask ) const
{
	return m_ex_flags & mask;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

bool f1::f1pp::macro::pre_change_value( f1::pp::string & string_value, f1::pp::variant * variant_value )
{
	while( !string_value.empty() && isspace( *string_value.data() ) )
		string_value.inc_ptr();

//	if( !get_ex_flags( ex_flags_t::fl_multiline | ex_flags_t::fl_function ) )
		return super::pre_change_value( string_value, variant_value );

//	pp::service::trim_spaces( string_value );
//	string_value.add_part( "\n" );

//	return true;
}

bool f1::f1pp::macro::is_stringizing_operator( f1::c_pp::parser & par, const char *& s, const char * end_s ) const
{
	if( !get_ex_flags( ex_flags_t::fl_multiline ) )
		return super::is_stringizing_operator( par, s, end_s );

	if( s >= end_s )
		return false;

	if( '^' != *s )
		return false;

	++ s;
	__unreferenced_parameter( par );
	return true;
}

const status_t f1::f1pp::macro::parse( pp::Iparser & parser, pp::string & result ) const
{
	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	const_cast<self*>(this)->m_multiline_patch_callback.m_parser = f1pp_par;

	// Skip open bracket if exists
	bool parenthesis = false;
	if( nullptr != get_param_list() )
	{
		parenthesis = par->skip_char( '(' );
		if( parenthesis != !!get_ex_flags( ex_flags_t::fl_parenthesis ) )
		{
			f1pp_par->message( get_ex_flags( ex_flags_t::fl_parenthesis ) ?
				syntax::IMSG_WARNING_PARAM_LIST_DEFINED_WITH_PARENTHESIS :
				syntax::IMSG_WARNING_PARAM_LIST_DEFINED_WITHOUT_PARENTHESIS );
		}
	}

	// Fetch argument list
	core::Tconst_ref<pp::arg_list> args;
	core::Tconst_ref<pp::param_list> params = get_param_list();
	if( nullptr != params )
	{
		par->skip_real_spaces_and_comments();
		f1pp_par->parse_arg_list( args, params, parenthesis ? ')' : '\n' );

		// Spread arguments to the call namespace
		if( nullptr != args )
		{
			// Get parameters information
			size_t param_index = 0, param_count = params->get_param_list().size();
			const bool vararg_present =
				param_count > 0 &&
				(*(params->get_param_list().rbegin())).m_flags & pp::param_list::fl_vararg;
			const size_t named_param_count = vararg_present ? param_count - 1 : param_count;

			// Get arguments information
			size_t arg_index = 0, arg_count = args->get_arg_list().size();
			if( arg_count > param_count && !vararg_present )
			{
				const pp::string * arg = args->get_arg_expanded_value( *params, arg_count - 1 );
				if( nullptr != params && 0 == param_count && (nullptr == arg || arg->empty()) )	// just ()
				{
				}
				else
					f1pp_par->message( syntax::IMSG_WARNING_TOO_MANY_ARGUMENTS_S, get_name() );
				arg_count = param_count;
			}
			if( arg_count < named_param_count )
				f1pp_par->message( syntax::IMSG_ERROR_INSUFFICIENT_MACRO_ARGUMENTS );

			// Set local namespace as current
			core::Tconst_ref<f1::ns::name_space> old_ns = parser.get_ns_context().get_current_namespace();
			parser.get_ns_context().set_current_namespace( &const_cast<self*>(this)->m_multiline_patch_callback.m_locals_ns );

			// Add all named arguments
			for( ; param_index < named_param_count; ++ param_index, ++ arg_index )
			{
				const pp::param_list::param_desc_t * param_desc = params->get_param_desc( param_index );
				const pp::string * arg_value = args->get_arg_expanded_value( *params, arg_index );

				// Create macro for each parameter/argument pair
				core::Tref<pp::Imacro> mac;
				parser.create_macro( mac, param_desc->m_name,
					nullptr == arg_value ? pp::string() : *arg_value,
					pp::Imacro::macro_flags_t::fl_immutable );
				__assert( nullptr != mac );
			}

			// Add varargs
			if( vararg_present )
			{
				pp::string vararg;

				// Gather all arguments
				if( arg_count > named_param_count )
				{
					for( ; ; )
					{
						const pp::string * arg_value = args->get_arg_expanded_value( *params, arg_index );
						vararg.add_part( *arg_value );
						++ arg_index;
						if( arg_index >= arg_count )
							break;
						vararg.add_part( ", " );
					}
				}

				// Create vararg macro
				core::Tref<pp::Imacro> mac;
				parser.create_macro( mac, pp::string( "__VA_ARGS__" ), vararg, pp::Imacro::macro_flags_t::fl_immutable );
				__assert( nullptr != mac );
			}

			// Restore previous namespace
			parser.get_ns_context().set_current_namespace( old_ns );
		}
	}

	// Expand the macro
	expand( parser, result, args );

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::f1pp::macro::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	core::Tref<f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_INVALID_TYPE;

	if( m_ex_flags & ex_flags_t::fl_function )
	{
#if __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__
		f1par->begin_subsection( *this );
#endif // __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__
		parser.get_parser()->insert_patch( get_value(), &const_cast<self*>(this)->m_multiline_patch_callback );
		f1par->set_newline();
	}
	else if( m_ex_flags & ex_flags_t::fl_multiline )
	{
#if __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__
		f1par->begin_subsection( *this );
#endif // __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__
		parser.get_parser()->insert_patch( get_value(), &const_cast<self*>(this)->m_multiline_patch_callback );
		f1par->set_newline();
	}
	else
		return super::expand( parser, buf, args );

	parser.on_macro_expanded( *const_cast<self *>(this) );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF macro.cxx*/
