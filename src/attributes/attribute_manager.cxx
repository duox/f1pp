/*attribute_manager.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::attribute_manager::attribute_manager( const core::Iidentifier * ident ):
	super( ident )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Attribute classes

const status_t f1::pp::attribute_manager::append_attribute_class( const pp::Iattribute_class & attr_class )
{
	return m_attribute_class_list.append( &attr_class );
}
const status_t f1::pp::attribute_manager::remove_attribute_class( const pp::Iattribute_class & attr_class )
{
	return m_attribute_class_list.remove( &attr_class );
}
const status_t f1::pp::attribute_manager::remove_all_attribute_classes()
{
	return m_attribute_class_list.remove_all();
}

const f1::pp::const_attribute_class_list_t & f1::pp::attribute_manager::get_attribute_class_list() const
{
	return m_attribute_class_list;
}

const status_t f1::pp::attribute_manager::find_attribute_class( core::Tconst_ref<pp::Iattribute_class> & res, const f1::guid_t &  guid, const pp::arg_list * args ) const
{
	array_foreach( const_attribute_class_list_t::const_iterator, it, m_attribute_class_list )
	{
		core::Tconst_ref<Iattribute_class> attr_class = *it;
		__debugbreak_if( nullptr == attr_class )
			continue;
		if( attr_class->get_guid() == guid && pp::param_list::match_arg_list( attr_class->get_param_list(), args ) )
		{
			res = attr_class;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}
const status_t f1::pp::attribute_manager::find_attribute_class( core::Tconst_ref<pp::Iattribute_class> & res, const pp::string & ident, const pp::arg_list * args ) const
{
	array_foreach( const_attribute_class_list_t::const_iterator, it, m_attribute_class_list )
	{
		core::Tconst_ref<Iattribute_class> attr_class = *it;
		__debugbreak_if( nullptr == attr_class )
			continue;
		if( attr_class->get_name() == ident && pp::param_list::match_arg_list( attr_class->get_param_list(), args ) )
		{
			res = attr_class;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Attributes

const status_t f1::pp::attribute_manager::create_attribute( f1::core::Tref<pp::Iattribute> & res,
	const f1::guid_t & guid, unsigned flags, f1::pp::arg_list * args, const core::Iidentifier * ident ) noexcept
{
	status_t status;
	
	core::Tconst_ref<pp::Iattribute_class> attr_cls;
	if( find_attribute_class( attr_cls, guid, args ).failed_status( status ) )
		return status;
	return create_attribute( res, *attr_cls.ref_const_cast(), flags, args, ident );
}
const status_t f1::pp::attribute_manager::create_attribute( f1::core::Tref<pp::Iattribute> & res,
	const pp::string & name, unsigned flags, f1::pp::arg_list * args, const core::Iidentifier * ident ) noexcept
{
	status_t status;
	
	core::Tconst_ref<pp::Iattribute_class> attr_cls;
	if( find_attribute_class( attr_cls, name, args ).failed_status( status ) )
		return status;
	return create_attribute( res, *attr_cls.ref_const_cast(), flags, args, ident );
}
const status_t f1::pp::attribute_manager::create_attribute( f1::core::Tref<pp::Iattribute> & res,
	Iattribute_class & attr_cls, unsigned flags, f1::pp::arg_list * args, const core::Iidentifier * ident ) noexcept
{
	status_t status;

	f1::core::Tref<pp::Iattribute> obj;
	if( attr_cls.new_attribute( res, ident ).failed_status( status ) )
		return status;
	if( obj->initialize( attr_cls, flags, args ).failed_status( status ) )
		return status;

	res = obj;
	return STATUS_SUCCESS;
}

/*END OF attribute_manager.cxx*/
