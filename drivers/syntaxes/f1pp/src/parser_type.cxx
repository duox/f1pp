/*parser_type.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::f1pp::parser::parse_type( pp::variant::type_t & type_id, pp::string * type_name_ptr )
{
	// Parse type name
	pp::string type_name;
	parse_directive_identifier( type_name );
	if( type_name.empty() )
		return message( f1pp::syntax::IMSG_ERROR_EXPECTED_TYPE_NAME );

	// Return type id
	type_id = get_type_id( type_name );
	if( nullptr != type_name_ptr )
		type_name_ptr->move( type_name );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::parse_type( pp::variant::type_t & type_id, pp::string * type_name_ptr, const char * & s, const char * end_s )
{
	// Parse type name
	pp::string type_name;
	parse_directive_identifier( type_name, s, end_s );
	if( type_name.empty() )
		return message( f1pp::syntax::IMSG_ERROR_EXPECTED_TYPE_NAME );

	// Return type id
	type_id = get_type_id( type_name );
	if( nullptr != type_name_ptr )
		type_name_ptr->move( type_name );

	// Exit
	return STATUS_SUCCESS;
}

static const struct {
	const char *	m_name;
	size_t			m_length;
} type_name_array[] = {
	{ STR_LEN_PAIR("bool") },
	{ STR_LEN_PAIR("int") }, 
	{ STR_LEN_PAIR("uint") },
	{ STR_LEN_PAIR("float") },
	{ STR_LEN_PAIR("string") },
	{ STR_LEN_PAIR("text") },
};
static const f1::pp::variant::type_t type_id_array[_countof(type_name_array)] = {
	f1::pp::variant::ty_bool,
	f1::pp::variant::ty_int,
	f1::pp::variant::ty_uint,
	f1::pp::variant::ty_float,
	f1::pp::variant::ty_string,
	f1::pp::variant::ty_text,
};

f1::pp::variant::type_t f1::f1pp::parser::get_type_id( const char * name, size_t len ) const
{
	if( size_t(-1) == len )
		len = core::string::length( name );
	return get_type_id( pp::string( name, len ) );
}
f1::pp::variant::type_t f1::f1pp::parser::get_type_id( const pp::string & type_name ) const
{
	pp::variant::type_t type_id = f1::pp::variant::ty_none;
	for( size_t i = 0; i < _countof(type_name_array); ++ i )
	{
		if( type_name.length() != type_name_array[i].m_length )
			continue;
		if( !__small_strncmp( type_name.data(), type_name_array[i].m_name, type_name_array[i].m_length ) )
		{
			type_id = type_id_array[i];
			break;
		}
	}
	return type_id;
}
const char * f1::f1pp::parser::get_type_name( pp::variant::type_t type_id, size_t * len ) const
{
	if( unsigned(type_id) > pp::variant::ty_count )
		return nullcstr;
	if( nullptr != len )
		*len = type_name_array[type_id - 1].m_length;
	return type_name_array[type_id - 1].m_name;
}

/*END OF parser_type.cxx*/
