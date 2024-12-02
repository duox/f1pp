/*env_macro.cxx*/
#include "pch.h"
#include "main.h"

f1::f1pp::env_macro::env_macro( const core::Iidentifier * ident ):
	super( ident )
{
}

const status_t f1::f1pp::env_macro::initialize( f1::f1pp::parser & parser, const pp::string & name, const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	if( super::initialize( name, attrs, params, value, flags ).failed_status( status ) )
		return status;

	m_parser = &parser;
	m_env_var_variant.set_type( pp::variant::ty_string );

	return status;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const f1::f1pp::parser * f1::f1pp::env_macro::get_parser() const
{
	return m_parser;
}
f1::f1pp::parser * f1::f1pp::env_macro::get_parser()
{
	return m_parser;
}

const status_t f1::f1pp::env_macro::set_env_var( const pp::string & env_var_name )
{
	m_env_var_name = env_var_name;
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::env_macro::set_env_var_move( pp::string & env_var_name )
{
	m_env_var_name.move( env_var_name );
	return STATUS_SUCCESS;
}
const f1::pp::string & f1::f1pp::env_macro::get_env_var_name() const
{
	return m_env_var_name;
}

const status_t f1::f1pp::env_macro::_read_env_value() const
{
	status_t status;

	if( _read_env_variant().failed_status( status ) )
		return status;

	if( pp::variant::ty_string == m_env_var_variant.get_type() )
		status = m_parser->stringify_chars( m_env_var_value, pp::string( m_env_var_variant.as_string() ), '\"' );
	else
		status = m_env_var_variant.format( m_env_var_value );

	return status;
}
const status_t f1::f1pp::env_macro::_read_env_variant() const
{
	status_t status;
	const char * ep = "";

	m_env_var_value.clear();

	const char * env = getenv( m_env_var_name.c_str() );
	if( nullcstr == env )
		return STATUS_SUCCESS;

	switch( m_env_var_variant.get_type() )
	{
	case pp::variant::ty_bool:
		if( core::string::equal_nocase( env, "true" ) || core::string::equal_nocase( env, "yes" ) || core::string::equal_nocase( env, "1" ) )
			status = m_env_var_variant.set_bool( true );
		else if( core::string::equal_nocase( env, "false" ) || core::string::equal_nocase( env, "no" ) || core::string::equal_nocase( env, "0" ) )
			status = m_env_var_variant.set_bool( false );
		else
			status = STATUS_ERROR_INVALID_VALUE;
		break;
	case pp::variant::ty_int:
		status = m_env_var_variant.set_int( strtoll( env, (char **) &ep, 10 ) );
		break;
	case pp::variant::ty_uint:
		status = m_env_var_variant.set_uint( strtoull( env, (char **) &ep, 10 ) );
		break;
	case pp::variant::ty_float:
		status = m_env_var_variant.set_float( strtod( env, (char **) &ep ) );
		break;
	case pp::variant::ty_text:
		status = m_env_var_variant.set_text( env );
		break;
	case pp::variant::ty_string:
		status = m_env_var_variant.set_string( env );
		break;
	default:
		__assert( !"invalid variant type" );
		status = STATUS_ERROR_INVALID_TYPE;
		break;
	}

	__debugbreak_if( nullchar != *ep )
	{
		m_parser->message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
		if( status.succeeded() )
			status = STATUS_WARNING_SYNTAX_ISSUES;
	}

	return status;
}
const status_t f1::f1pp::env_macro::_write_env_value( const core::stringslice & value ) const
{
	core::string buf;
	buf.format( "%s=%s", m_env_var_name.c_str(), core::string(value).c_str() );
	_putenv( buf.c_str() );
	return STATUS_SUCCESS;
}

const status_t f1::f1pp::env_macro::set_env_var_type( pp::variant::type_t type )
{
	m_env_var_variant.set_type( type );
	return STATUS_SUCCESS;
}
f1::pp::variant::type_t f1::f1pp::env_macro::get_env_var_type() const
{
	return m_env_var_variant.get_type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::env_macro::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	_read_env_value();
	buf.move( m_env_var_value );

	// Exit
	__unreferenced_parameter( parser );
	__unreferenced_parameter( args );
	return STATUS_SUCCESS;
}

const f1::pp::string & f1::f1pp::env_macro::get_value() const
{
	_read_env_value();
	return m_env_var_value;
}
const f1::pp::variant & f1::f1pp::env_macro::get_value_variant() const
{
	_read_env_variant();
	return m_env_var_variant;
}
bool f1::f1pp::env_macro::pre_change_value( f1::pp::string & value_string, f1::pp::variant * value_variant )
{
	_write_env_value( core::stringslice( value_string ) );
	__unreferenced_parameter( value_variant );
	return super::pre_change_value( value_string, value_variant );
}

/*END OF env_macro.cxx*/
