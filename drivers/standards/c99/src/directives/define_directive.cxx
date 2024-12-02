/*define_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::c99::define_directive::define_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::c99::define_directive::get_name() const
{
	return "define";
}
// {FB008221-B998-42ed-B8FE-0C3F462D6F95}
const f1::guid_t f1::c_pp::c99::define_directive::m_guid =
{ 0xfb008221, 0xb998, 0x42ed, { 0xb8, 0xfe, 0xc, 0x3f, 0x46, 0x2d, 0x6f, 0x95 } };
const f1::guid_t & f1::c_pp::c99::define_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::c99::define_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::c99::define_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch macro identifier
	pp::string ident;
	c_pp_par->skip_real_spaces_and_comments();	// skip spaces and comments between directive name and macro name
	if( c_pp_par->parse_definition_identifier( ident ).failed_status( status ) )
		return status;
	if( ident.is_empty() )
	{
		parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		par->skip_to_nl();
		return STATUS_SUCCESS;
	}

	if( !parser.check_limit_value_min( pp::limit_t::li_maximum_identifier_length, ident.length() ) )
		parser.message( syntax::IMSG_WARNING_TOO_LONG_IDENTIFIER );

	// Fetch parameter list
	core::Tref<pp::param_list> params = nullptr;
	if( par->skip_char( '(' ) )
	{
		if( c_pp_par->parse_param_list( params, ')' ).failed_status( status ) )
		{
			c_pp_par->skip_to_newline();	// TODO: print error
			return STATUS_SUCCESS;	// argument parsing failed, just leave directive definition
		}
	}

	// Check that there're some spaces between macro header and its value (C++11 behaviour)
	bool skipped_spaces = true;
	c_pp_par->skip_real_spaces_and_comments( &skipped_spaces );
	if( !skipped_spaces && !par->is_nl() )
		parser.message( syntax::IMSG_WARNING_DEFINE_WHITESPACE_REQUIRED );

	// Check potential problem with detached param list
	if( nullptr == params && par->peek_char( '(' ) )
	{
		if( parser.get_msgout_context().is_message_enabled( syntax::IMSG_WARNING_POSSIBLY_DETACHED_PARAM_LIST ) )
		{
			core::Tref<pp::param_list> temp_params;

			par->push_pos();
			c_pp_par->parse_param_list( temp_params, 0, c_pp::parser::fl_parse_param_list_silent );
			if( nullptr != temp_params )
				parser.message( syntax::IMSG_WARNING_POSSIBLY_DETACHED_PARAM_LIST );
			par->pop_pos();
		}
	}

	// Fetch macro value
	pp::string value;
	if( c_pp_par->parse_to_newline( value ).failed_status( status ) )
		return status;
	c_pp_par->trim_spaces_and_comments( value );

	// Get macro flags
	pp::Imacro::macro_flags_t macro_flags = null;
	if( fetch_macro_flags( *c_pp_par, macro_flags, ident, params, value ).failed_status( status ) )
		return status;

	// Get attributes
	core::Tconst_ref<pp::attribute_list> attrs = c_pp_par->get_directive_attribute_list();

	// Create macro
	bool result;
	if( can_create_macro( result, *c_pp_par, ident, attrs, params, &value, macro_flags ).failed_status( status ) )
		return status;

	if( result )
	{
		core::Tref<pp::Imacro> mac;
		if( create_macro( mac, *c_pp_par, ident, attrs, params, value, macro_flags ).failed_status( status ) )
			return status;
		if( nullptr != mac )
			mac->set_definition_location( *par->get_current_source(), par->get_line() );
	}

	// Parse macro value
//	if( mac->parse_value( *c_pp_par ).failed_status( status ) )
//		return status;

	// Exit
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hooks

const status_t f1::c_pp::c99::define_directive::fetch_macro_flags(
	c_pp::parser & parser, pp::Imacro::macro_flags_t & flags,
	const pp::string & ident, const pp::param_list * params, const pp::string & value )
{
	flags = null;

	__unreferenced_parameter( parser );
	__unreferenced_parameter( ident );
	__unreferenced_parameter( params );
	__unreferenced_parameter( value );
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::c99::define_directive::find_macro( core::Tref<pp::Imacro> & res,
	c_pp::parser & parser,
	const core::stringslice & ident, const pp::param_list * params,
	pp::Iparser::find_macro_flags_t flags, core::Tref<f1::ns::name> * nm )
{
	return nullptr == params ?
		parser.find_macro( res, ident,			flags, nm ):
		parser.find_macro( res, ident, *params, flags, nm );
}

const status_t f1::c_pp::c99::define_directive::can_create_macro(
	bool & result,
	c_pp::parser & parser,
	const f1::pp::string & ident, const f1::pp::attribute_list * attrs,
	const f1::pp::param_list * params, const pp::string * value, pp::Imacro::macro_flags_t flags
	)
{
	status_t status;

	result = false;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Check whether the macro exists
	core::Tref<pp::Imacro> mac = nullptr;
	find_macro( mac, parser, ident, params, pp::Iparser::find_macro_flags_t::fl_macro_search_current_namespace_only | pp::Iparser::find_macro_flags_t::fl_macro_search_force );
	if( nullptr != mac && 0 != (flags & pp::Imacro::macro_flags_t::fl_weak) )
		return STATUS_SUCCESS;
	if( nullptr != mac && 0 == mac->get_flags( pp::Imacro::macro_flags_t::fl_weak ) )
	{
		if( mac->get_flags( pp::Imacro::macro_flags_t::fl_undefined ) )
		{
			mac->modify_flags( null, pp::Imacro::macro_flags_t::fl_undefined );
			mac->set_param_list( params );
			if( nullptr != value )
				mac->set_value( *value );
			mac->set_definition_location( *par->get_current_source(), par->get_line() );
		}
		else if( mac->get_flags( pp::Imacro::macro_flags_t::fl_immutable ) )
		{
			parser.message( syntax::IMSG_ERROR_MACRO_IS_IMMUTABLE );
			parser.display_macro_location( *mac, syntax::IMSG_INFO_DEFINED_HERE );
			return STATUS_SUCCESS;
		}
		else if( mac->get_flags( pp::Imacro::macro_flags_t::fl_disabled ) )
		{
			parser.message( syntax::IMSG_ERROR_CANNOT_DEFINE_MACRO_S, ident.c_str() );
			return STATUS_SUCCESS;
		}
		else
		{
			bool equal;
			if( nullptr == value )
			{
				equal = mac->get_value().is_empty();
			}
			else
			{
				core::md5hash::digest_t value_digest;
				parser.calc_value_hash( value_digest, *value, params );

				equal = !memcmp( &value_digest, mac->get_value_hash(), sizeof(value_digest) );
				if( equal )
				{
					bool values_equal = parser.compare_values( mac->get_value(), mac->get_param_list(), *value, params );
					__assert( equal == values_equal );
					equal = values_equal;
				}
			}

			/*bool equal = nullptr == value ?
				mac->get_value().is_empty() :
				parser.compare_values( mac->get_value(), mac->get_param_list(), *value, params );

			core::md5hash::digest_t digest1;
			core::md5hash::digest_t digest2;
			parser.calc_value_hash( digest1, mac->get_value(), mac->get_param_list() );
			parser.calc_value_hash( digest2, *value, params );
			bool equal2 = !memcmp( &digest1, &digest2, sizeof(digest1) );
			__assert( equal == equal2 );*/

			/*bool equal = value->equal( mac->get_value() );
			if( equal )
			{
				core::Tconst_ref<pp::param_list> pl = mac->get_param_list();
				if( nullptr != params && nullptr != pl )
					equal = params->equal( *pl );
				else
					equal = nullptr == params && nullptr == pl;
			}*/

			if( !equal )
			{
				parser.message( syntax::IMSG_ERROR_MACRO_ALREADY_DEFINED_S, ident.c_str() );
				parser.display_macro_location( *mac, syntax::IMSG_INFO_DEFINED_HERE );
				return STATUS_SUCCESS;
			}
			else
			{
				status = parser.message( syntax::IMSG_WARNING_REDUNANT_DEFINE );
				if( status.succeeded() && STATUS_WARNING_OPERATION_DISABLED != status )
				{
					core::Tref<f1::parser::Isource> src = mac->get_definition_source();
					if( nullptr != src )
						parser.display_macro_location( *mac, syntax::IMSG_INFO_DEFINED_HERE );
				}
			}
		}
	}
	else
	{
		// Check that the name does not exist
		core::Tref<f1::ns::name> nm;
		parser.get_ns_context().get_current_namespace()->find_subname( nm, ident.data(), ident.length() );
		if( nullptr == mac && nullptr != nm )
		{
			parser.message( syntax::IMSG_ERROR_NAME_IS_ALREADY_OCCUPIED_S, ident.c_str() );
		}
		else
		{
			// Undefine weak macro
			if( nullptr != nm )
			{
				__assert( nullptr != mac );
				//parser.get_ns_context().get_current_namespace()->remove_subname( *nm );
				parser.remove_macro( *mac );
			}

			result = true;
		}
	}

	// Exit
	__unreferenced_parameter( attrs );
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::c99::define_directive::create_macro( core::Tref<pp::Imacro> & res,
	c_pp::parser & parser,
	const pp::string & ident, const pp::attribute_list * attrs, const pp::param_list * params, const pp::string & value, pp::Imacro::macro_flags_t flags )
{
	return parser.create_macro( res, ident, attrs, params, value, flags );
}

/*END OF define_directive.cxx*/
