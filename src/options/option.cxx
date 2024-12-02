/*option.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::option::option() __noexcept:
	m_expected_type( pp::variant::ty_none )
{
}

const status_t f1::pp::option::initialize( pp::variant::type_t ty,
	const f1::guid_t & guid, const char * name, size_t len )
{
	status_t status;

	if( super::initialize().failed_status( status ) )
		return status;

	m_guid = guid;
	m_name.add_part( name, len );
	set_expected_type( ty );

	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const status_t f1::pp::option::set_name( const char * name, size_t len )
{
	m_name.add_part( name, len );
	return STATUS_SUCCESS;
}
const status_t f1::pp::option::set_name( const pp::string & name )
{
	m_name.add_part( name.get_string() );
	return STATUS_SUCCESS;
}
const status_t f1::pp::option::set_name( const core::string & name )
{
	m_name.add_part( name );
	return STATUS_SUCCESS;
}
const status_t f1::pp::option::set_name_move( pp::string & name )
{
	m_name.move( name );
	return STATUS_SUCCESS;
}
const f1::core::stringslice & f1::pp::option::get_name() const
{
	return m_name.get_stringslice();
}

const status_t f1::pp::option::set_guid( const f1::guid_t & guid )
{
	m_guid = guid;
	return STATUS_SUCCESS;
}
const f1::guid_t & f1::pp::option::get_guid() const
{
	return m_guid;
}

const status_t f1::pp::option::set_expected_type( f1::pp::variant::type_t ty )
{
	m_expected_type = ty;
	if( pp::variant::ty_none != ty )
		m_value.set_type( ty );
	return STATUS_SUCCESS;
}
const f1::pp::variant::type_t f1::pp::option::get_expected_type() const
{
	return m_expected_type;
}

const f1::pp::variant & f1::pp::option::get_value() const
{
	if( !m_value.is_valid() )
		return get_default_value();
	return m_value;
}
const status_t f1::pp::option::set_value( const pp::variant & val )
{
	m_value = val;
	return STATUS_SUCCESS;
}

/*END OF option.cxx*/
