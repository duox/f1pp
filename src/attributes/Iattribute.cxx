/*Iattribute.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::Iattribute::Iattribute( const core::Iidentifier * ident ):
	super( ident )
{
}
const status_t f1::pp::Iattribute::initialize( Iattribute_class & cls, unsigned flags, pp::arg_list * args )
{
	status_t status;

	if( super::initialize().failed_status( status ) )
		return status;

	m_attribute_class = &cls;
	m_arg_list = args;
	m_flags = flags;

	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const f1::guid_t & f1::pp::Iattribute::get_class_guid() const
{
	if( nullptr == m_attribute_class )
		return f1::guid_t::invalid;
	return m_attribute_class->get_guid();
}
bool f1::pp::Iattribute::supports_guid( const f1::guid_t & guid ) const noexcept
{
	return get_class_guid() == guid;
}

const f1::pp::Iattribute_class * f1::pp::Iattribute::get_attribute_class() const
{
	return m_attribute_class;
}
f1::pp::Iattribute_class * f1::pp::Iattribute::get_attribute_class()
{
	return m_attribute_class;
}

const status_t f1::pp::Iattribute::modify_flags( unsigned flags_to_set, unsigned flags_to_clear, unsigned * flags_prev ) noexcept
{
	m_flags.modify( flags_to_set, flags_to_clear, flags_prev );
	return STATUS_SUCCESS;
}
unsigned __nodiscard f1::pp::Iattribute::get_flags( unsigned mask ) const noexcept
{
	return m_flags & mask;
}

const f1::pp::arg_list * f1::pp::Iattribute::get_arg_list() const noexcept
{
	return m_arg_list;
}

/*END OF Iattribute.cxx*/
