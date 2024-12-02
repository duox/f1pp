/*redefine_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::redefine_directive::redefine_directive( unsigned flags ):
	super( flags )
{
}

const status_t f1::f1pp::redefine_directive::dup( f1::core::Tref<f1::core::object> & obj, bool deep_copy ) const noexcept
{
	__unreferenced_parameter( deep_copy );
	return obj.createT<self>( get_flags() );
}

// Properties
const char * f1::f1pp::redefine_directive::get_name() const
{
	return "redefine";
}
// {AA192A9D-67A5-4e7b-8EE6-1F028FB4F780}
const f1::guid_t f1::f1pp::redefine_directive::m_guid =
{ 0xaa192a9d, 0x67a5, 0x4e7b, { 0x8e, 0xe6, 0x1f, 0x2, 0x8f, 0xb4, 0xf7, 0x80 } };
const f1::guid_t & f1::f1pp::redefine_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::redefine_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

// Operations
const status_t f1::f1pp::redefine_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch macro identifier
	pp::string ident;
	f1par->skip_real_spaces_and_comments();	// skip spaces and comments between directive name and macro name
	if( f1par->parse_definition_identifier( ident ).failed_status( status ) )
		return status;
	if( ident.is_empty() )
	{
		par->skip_to_nl();
		return parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
	}

	if( !parser.check_limit_value_min( pp::limit_t::li_maximum_identifier_length, ident.length() ) )
		parser.message( syntax::IMSG_WARNING_TOO_LONG_IDENTIFIER );

	// Fetch parameter list
	core::Tref<pp::param_list> params = nullptr;
	if( par->skip_char( '(' ) )
	{
		if( f1par->parse_param_list( params, ')' ).failed_status( status ) )
		{
			f1par->skip_to_newline();
			return STATUS_SUCCESS;	// argument parsing failed, just leave directive definition
		}
	}

	// Check that there're some spaces between macro header and its value (C++11 behaviour)
	bool skipped_spaces = true;
	f1par->skip_real_spaces_and_comments( &skipped_spaces );
	if( !skipped_spaces && !par->is_nl() )
		parser.message( syntax::IMSG_WARNING_DEFINE_WHITESPACE_REQUIRED );

	// Check potential problem with detached param list
	if( nullptr == params && par->peek_char( '(' ) )
		parser.message( syntax::IMSG_WARNING_POSSIBLY_DETACHED_PARAM_LIST );

	// Fetch macro value
	pp::string value;
	if( f1par->parse_to_newline( value ).failed_status( status ) )
		return status;
	f1par->trim_spaces_and_comments( value );

	// Check whether the macro exists
	core::Tref<pp::Imacro> mac = nullptr;
	status = nullptr == params ?
		parser.find_macro( mac, ident,			pp::Iparser::find_macro_flags_t::fl_macro_search_current_namespace_only | pp::Iparser::find_macro_flags_t::fl_macro_search_force ):
		parser.find_macro( mac, ident, *params, pp::Iparser::find_macro_flags_t::fl_macro_search_current_namespace_only | pp::Iparser::find_macro_flags_t::fl_macro_search_force );
	if( status.succeeded() && nullptr != mac )
	{
		if( mac->get_flags( pp::Imacro::macro_flags_t::fl_immutable ) )
		{
			parser.message( syntax::IMSG_ERROR_MACRO_IS_IMMUTABLE );
			f1par->display_macro_location( *mac, syntax::IMSG_INFO_DEFINED_HERE );
			return STATUS_SUCCESS;
		}
		else if( mac->get_flags( pp::Imacro::macro_flags_t::fl_disabled ) )
		{
			parser.message( syntax::IMSG_ERROR_CANNOT_DEFINE_MACRO_S, ident.c_str() );
			return STATUS_SUCCESS;
		}
		else
		{
			core::Tconst_weak_ref<pp::param_list> old_params = mac->get_param_list();
			if( (nullptr != old_params) != (nullptr != params) ||
				( nullptr != old_params && nullptr != params && !old_params->equal( *params ) ) )
			{
				parser.message( syntax::IMSG_WARNING_PARAM_LISTS_DIFFER, ident.c_str() );
				f1par->display_macro_location( *mac, syntax::IMSG_INFO_DEFINED_HERE );
			}

			mac->modify_flags( null, pp::Imacro::macro_flags_t::fl_undefined );
			mac->set_param_list( params );
			mac->set_value( value );
			mac->set_definition_location( *par->get_current_source(), par->get_line() );
		}
	}
	else
	{
		// Check that the name does not exist
		core::Tref<f1::ns::name> nm;
		if( parser.get_ns_context().get_current_namespace()->find_subname( nm, ident.data(), ident.length() ).succeeded() )
		{
			parser.message( syntax::IMSG_ERROR_NAME_IS_ALREADY_OCCUPIED_S, ident.c_str() );
		}
		else
		{
			// Create macro
			if( parser.create_macro( mac, ident, nullptr, params, value ).failed_status( status ) )
				return status;
			mac->set_definition_location( *par->get_current_source(), par->get_line() );
		}
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF redefine_directive.cxx*/
