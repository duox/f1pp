/*env_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::env_directive::env_directive( unsigned flags ):
	super( flags ),
	m_type( pp::variant::ty_string )
{
}

// Properties
const char * f1::f1pp::env_directive::get_name() const
{
	return "env";
}
// {01F9970E-66E6-4240-A994-67E120D39A8A}
const f1::guid_t f1::f1pp::env_directive::m_guid =
{ 0x01f9970e, 0x66e6, 0x4240, { 0xa9, 0x94, 0x67, 0xe1, 0x20, 0xd3, 0x9a, 0x8a } };
const f1::guid_t & f1::f1pp::env_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::env_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

// Operations
const status_t f1::f1pp::env_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Gather macro attributes
	m_macro_flags = null;
	if( f1par->exists_directive_attribute( "deprecated" ) )
		m_macro_flags |= pp::Imacro::macro_flags_t::fl_deprecated;
	if( f1par->exists_directive_attribute( "disabled" ) )
		m_macro_flags |= pp::Imacro::macro_flags_t::fl_disabled;
	if( f1par->exists_directive_attribute( "protected" ) )
		m_macro_flags |= pp::Imacro::macro_flags_t::fl_immutable;
	if( f1par->exists_directive_attribute( "weak" ) )
		m_macro_flags |= pp::Imacro::macro_flags_t::fl_weak;

	m_def_flags = 0;
	if( f1par->exists_directive_attribute( "global" ) )
		m_def_flags |= Idefinition_frame::fl_def_global;
	if( f1par->exists_directive_attribute( "local" ) )
		m_def_flags |= Idefinition_frame::fl_def_local;
	if( f1par->exists_directive_attribute( "public" ) )
		m_def_flags |= Idefinition_frame::fl_def_public;

	// Fetch macro identifier
	f1par->skip_real_spaces_and_comments();
	f1par->parse_definition_identifier( m_ident );
	if( m_ident.is_empty() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		return STATUS_SUCCESS;
	}
	if( !parser.check_limit_value_min( pp::limit_t::li_maximum_identifier_length, m_ident.length() ) )
		parser.message( syntax::IMSG_WARNING_TOO_LONG_IDENTIFIER );

	// Parse optional type specification
	f1par->skip_real_spaces_and_comments();
	if( par->skip_char( ':' ) )
	{
		f1par->skip_real_spaces_and_comments();
		m_type = f1::pp::variant::ty_string;
		pp::string type_name;
		f1par->parse_type( m_type, &type_name );
		if( f1::pp::variant::ty_none == m_type )
			return parser.message( f1pp::syntax::IMSG_ERROR_UNKNOWN_TYPE_NAME_S, type_name.c_str() );
	}

	// Skip optional '='
	f1par->skip_real_spaces_and_comments();
	if( par->skip_char( '=' ) )
		f1par->skip_real_spaces_and_comments();

	// Go to expanding macros
	f1par->modify_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode );
	f1par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::env_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_INVALID_TYPE;

	// Parse buffer
	if( 0 == out_buf.get_data_size() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_EXPRESSION );
		return STATUS_SUCCESS;
	}

	// Concatenate all strings
	// Using direct parsing from buffer since the string is read and glued, all macros are substituted
	const char *	 start = (const char *) out_buf.get_ptr();
	const char * const end = (const char *) out_buf.get_end_ptr();
	pp::string result;

	for(;;)
	{
		pp::string value;

		f1par->skip_real_spaces_and_comments( start, end );
		if( start >= end )
			break;

		if( f1par->is_ident_first_char( *start ) )
			f1par->parse_definition_identifier( value, start, end );
		else if( '\'' == *start || '\"' == *start || '`' == *start )
			status = f1par->skip_string( start, end, 0, &value );
		else
			return parser.message( f1pp::syntax::IMSG_ERROR_EXPECTED_STRING_OR_IDENT );
		if( status.failed() )
			return status;

		result.add_part( value );
	}

	// Process result
	f1par->skip_real_spaces_and_comments( start, end );
	if( start < end )
		parser.message( c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "env" );

	// Create macro
	core::Tref<pp::Imacro> existing_macro;
	f1par->find_macro( existing_macro, m_ident, pp::Iparser::find_macro_flags_t::fl_macro_search_current_namespace_only );
	if( nullptr != existing_macro )
	{
		if( 0 == existing_macro->get_flags( pp::Imacro::macro_flags_t::fl_weak ) )
		{
			status = parser.message( c_pp::syntax::IMSG_ERROR_MACRO_ALREADY_DEFINED_S, m_ident.c_str() );
			f1par->display_macro_location( *existing_macro, syntax::IMSG_INFO_DEFINED_HERE );
			return status;
		}

		core::Tref<f1::ns::name> nm;
		parser.get_ns_context().get_current_namespace()->find_subname( nm, m_ident.data(), m_ident.length() );
		if( nullptr != nm )
		{
			__assert( nullptr != existing_macro );
			parser.get_ns_context().get_current_namespace()->remove_subname( *nm );
		}
	}

	core::Tref<env_macro> macro;
	macro.createT<env_macro>();
	macro->initialize( *f1par, m_ident, f1par->get_directive_attribute_list(),
		nullptr, nullptr, m_macro_flags );
	macro->set_env_var_move( result );
	macro->set_env_var_type( m_type );
	f1par->append_macro( *macro );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::env_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
	parser.modify_parser_flags( pp::Iparser::parser_flags_t::null, pp::Iparser::parser_flags_t::fl_expression_mode );
#ifdef _DEBUG
	core::Tref<f1::f1pp::env_directive> drc = type_cast<f1::f1pp::env_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::f1pp::env_directive *>( param )->execute( parser, buf );
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*END OF env_directive.cxx*/
