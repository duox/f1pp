/*Idirective.cxx*/
#include "pch.h"
#include "main.h"

// {EA5D52D4-148A-4934-B61B-B139937C6E50}
const f1::guid_t f1::pp::Idirective::m_item_class_guid =
{ 0xea5d52d4, 0x148a, 0x4934, { 0xb6, 0x1b, 0xb1, 0x39, 0x93, 0x7c, 0x6e, 0x50 } };

f1::pp::Idirective::Idirective( unsigned flags ) __noexcept:
	m_flags( flags )
{
}
f1::pp::Idirective::~Idirective() __noexcept
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::pp::Idirective::get_name() const
{
	DBG_FAIL_ON_PURE_CALL();
	return nullptr;
}
const f1::guid_t & f1::pp::Idirective::get_guid() const
{
	DBG_FAIL_ON_PURE_CALL();
	return f1::guid_t::invalid;
}

unsigned f1::pp::Idirective::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

const status_t f1::pp::Idirective::modify_flags( unsigned flags_to_set, unsigned flags_to_clear, unsigned * flags_prev ) __noexcept
{
	m_flags.modify( flags_to_set, flags_to_clear, flags_prev );
	return STATUS_SUCCESS;
}
unsigned f1::pp::Idirective::get_flags( unsigned mask ) const __noexcept
{
	return m_flags & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Supported attributes
/*
const f1::pp::attribute_manager * f1::pp::Idirective::get_attribute_manager() const
{
	return m_attribute_manager;
}
f1::pp::attribute_manager * f1::pp::Idirective::get_attribute_manager()
{
	return m_attribute_manager;
}

const status_t f1::pp::Idirective::append_attribute_class( const f1::guid_t & guid, const char * name, size_t len )
{
	return append_attribute_class( guid, pp::string(name, len) );
}
const status_t f1::pp::Idirective::append_attribute_class( const f1::guid_t & guid, const pp::string & name )
{
	status_t status;

	core::Tref<pp::user_attribute_class> cls;
	if( cls.createT<pp::user_attribute_class>().failed_status( status ) )
		return status;
	if( cls->initialize( guid, name ).failed_status( status ) )
		return status;

	return append_attribute_class( *cls );
}
const status_t f1::pp::Idirective::append_attribute_class( const Iattribute_class & cls )
{
	if( nullptr == m_attribute_manager )
	{
		status_t status = m_attribute_manager.createT<pp::attribute_manager>();
		if( status.failed() )
			return status;
		__debugbreak_if( nullptr == m_attribute_manager )
			return STATUS_ERROR_INTERNAL;
	}

	return m_attribute_manager->append_attribute_class( cls );
}
const status_t f1::pp::Idirective::remove_attribute_class( const Iattribute_class & cls )
{
	__debugbreak_if( nullptr == m_attribute_manager )
		return STATUS_ERROR_NOT_FOUND;
	return m_attribute_manager->remove_attribute_class( cls );
}
const status_t f1::pp::Idirective::remove_all_attribute_classes()
{
	if( nullptr == m_attribute_manager )
		return STATUS_SUCCESS;
	return m_attribute_manager->remove_all_attribute_classes();
}

const status_t f1::pp::Idirective::find_attribute_class( core::Tconst_ref<pp::Iattribute_class> & res,
	const f1::guid_t & guid, const pp::arg_list * args ) const
{
	__debugbreak_if( nullptr == m_attribute_manager )
		return STATUS_ERROR_NOT_FOUND;
	return m_attribute_manager->find_attribute_class( res, guid, args );
}
const status_t f1::pp::Idirective::find_attribute_class( core::Tconst_ref<pp::Iattribute_class> & res,
	const pp::string & name, const pp::arg_list * args ) const
{
	__debugbreak_if( nullptr == m_attribute_manager )
		return STATUS_ERROR_NOT_FOUND;
	return m_attribute_manager->find_attribute_class( res, name, args );
}
const status_t f1::pp::Idirective::find_attribute_class( core::Tconst_ref<pp::Iattribute_class> & res,
	const char * name, size_t len, const pp::arg_list * args ) const
{
	__debugbreak_if( nullptr == m_attribute_manager )
		return STATUS_ERROR_NOT_FOUND;
	return m_attribute_manager->find_attribute_class( res, pp::string(name, len), args );
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::pp::Idirective::parse( pp::Iparser & parser )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	return STATUS_ERROR_PURE_CALL;
}
f1::pp::Iframe * f1::pp::Idirective::alloc_idle_frame( f1::pp::Iparser & parser )
{
	__unreferenced_parameter( parser );
	return nullptr;
}

/*END OF Idirective.cxx*/
