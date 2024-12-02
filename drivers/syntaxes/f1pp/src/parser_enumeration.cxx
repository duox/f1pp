/*parser_enumeration.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::f1pp::parser::create_enumeration( core::Tref<enumeration> & res, const char * name, size_t len )
{
	status_t status;

	core::Tref<enumeration> obj;
	if( obj.createT<enumeration>().failed_status( status ) )
		return status;
	__debugbreak_if( nullptr == obj )
		return STATUS_ERROR_OBJECT_CREATION;
	if( obj->initialize( *this, pp::string( name, len ) ).failed_status( status ) )
		return status;

	m_enumerations.append_unique( obj );

	res = obj;
	return status;
}

const status_t f1::f1pp::parser::append_enumeration( enumeration & en )
{
	return m_enumerations.append_unique( &en );
}
const status_t f1::f1pp::parser::remove_enumeration( enumeration & en )
{
	return m_enumerations.remove( &en );
}
const status_t f1::f1pp::parser::remove_all_enumerations()
{
	return m_enumerations.remove_all();
}

const status_t f1::f1pp::parser::get_enumeration( core::Tconst_ref<enumeration> & res, const char * name, size_t len )
{
	array_foreach( enumeration_list_t::const_iterator, it, m_enumerations )
	{
		core::Tconst_weak_ref<enumeration> en = *it;
		core::Tref<ns::name> nm;
		if( en->find_name( nm, name, len ).succeeded() )
		{
			core::Tweak_ref<ns::named_object> nobj = nm->get_object();
			if( nullptr != nobj )
			{
				res = nobj.dynamic_pointer_cast<enumeration>();
				if( nullptr == res )
					return STATUS_ERROR_INVALID_TYPE;
				return STATUS_SUCCESS;
			}
			break;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}
const status_t f1::f1pp::parser::get_enumeration( core::Tconst_ref<enumeration> & res, const pp::string & name )
{
	return get_enumeration( res, name.data(), name.length() );
}
const status_t f1::f1pp::parser::find_enumeration( core::Tconst_ref<enumeration> & res, const char * name, size_t len )
{
	status_t status;

	core::Tconst_ref<f1::ns::name> nm = nullptr;
	if( m_ns_context.lookup_name( nm, name, len ).failed_status( status ) )
		return status;
	__debugbreak_if( nullptr == nm )
		return STATUS_ERROR_INTERNAL;

	if( nullptr == nm->get_object() )
		return STATUS_ERROR_NOT_FOUND;
	res = type_cast<const enumeration *>( nm->get_object() );

	return nullptr == res ? STATUS_ERROR_INVALID_TYPE : STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::find_enumeration( core::Tconst_ref<enumeration> & res, const pp::string & name )
{
	return find_enumeration( res, name.data(), name.length() );
}

const f1::f1pp::enumeration_list_t & f1::f1pp::parser::get_enumeration_list() const
{
	return m_enumerations;
}

/*END OF parser_enumeration.cxx*/
