/*attribute_list.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::attribute_list::attribute_list( const core::Iidentifier * ident ):
	super( ident )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::attribute_list::append_attribute( const pp::attribute & attr )
{
	return m_attribute_list.append( &attr );
}
const status_t f1::pp::attribute_list::append_attributes( const pp::attribute_list & attrs )
{
	return m_attribute_list.append( attrs.m_attribute_list );
}
const status_t f1::pp::attribute_list::remove_attribute( const pp::attribute & attr )
{
	return m_attribute_list.remove( &attr );
}
const status_t f1::pp::attribute_list::remove_attributes( const pp::attribute_list & attrs )
{
	return m_attribute_list.remove( attrs.m_attribute_list );
}
const status_t f1::pp::attribute_list::remove_all_attributes()
{
	return m_attribute_list.remove_all();
}

const f1::pp::const_attribute_list_t & f1::pp::attribute_list::get_attribute_list() const
{
	return m_attribute_list;
}

bool f1::pp::attribute_list::exists_attribute( const f1::guid_t & guid ) const
{
//	core::Tconst_ref<pp::attribute> attr;
//	return find_attribute( attr, guid ).succeeded();
	return false;
}
bool f1::pp::attribute_list::exists_attribute( const pp::string & name ) const
{
	core::Tconst_ref<pp::attribute> attr;
	return find_attribute( attr, name ).succeeded();
}
bool f1::pp::attribute_list::exists_attribute( const char * name, size_t len ) const
{
	core::Tconst_ref<pp::attribute> attr;
	return find_attribute( attr, pp::string(name, len) ).succeeded();
}

/*const status_t f1::pp::attribute_list::find_attribute( core::Tconst_ref<pp::attribute> & res, const f1::guid_t & guid, const pp::arg_list * args ) const
{
	array_foreach( const_attribute_list_t::const_iterator, it, m_attribute_list )
	{
		core::Tconst_ref<attribute> attr = *it;
		core::Tconst_ref<attribute_class> attr_class = attr->get_attribute_class();
		__debugbreak_if( nullptr == attr_class )
			continue;
		if( attr->get_class_guid() == guid && _check_attribute( *attr, *attr_class, args ) )
		{
			res = attr;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}*/
const status_t f1::pp::attribute_list::find_attribute( core::Tconst_ref<pp::attribute> & res, const pp::string & ident, const pp::arg_list * args ) const
{
	array_foreach( const_attribute_list_t::const_iterator, it, m_attribute_list )
	{
		core::Tconst_ref<attribute> attr = *it;
		if( attr->get_name() == ident )
		{
			res = attr;
			return STATUS_SUCCESS;
		}
		/*core::Tconst_ref<attribute_class> attr_class = attr->get_attribute_class();
		__debugbreak_if( nullptr == attr_class )
			continue;
		if( attr_class->get_name() == ident && _check_attribute( *attr, *attr_class, args ) )
		{
			res = attr;
			return STATUS_SUCCESS;
		}*/
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}

/*bool f1::pp::attribute_list::_check_attribute( const pp::attribute & attr, const attribute_class & attr_class, const pp::arg_list * args ) const
{
#if 1
	__unreferenced_parameter( attr );
	return pp::param_list::match_arg_list( attr_class.get_param_list(), args );
#else
	core::Tconst_ref<pp::param_list> params = attr_class.get_param_list();

	// No params - should be no args
	if( nullptr == params )
		return nullptr == args;
	if( nullptr == args )
		return false;

	// Empty param list - should be empty arg list
	if( params->get_param_list().empty() )
		return args->get_arg_list().empty();
	if( args->get_arg_list().empty() )
		return false;

	// Check argument count, considering presence of default arguments
	// TODO: default arguments
	if( params->get_param_list().size() != args->get_arg_list().size() )
		return false;

	// Exit
	__unreferenced_parameter( attr );
	return true;
#endif
}
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::pp::attribute_list::output( pp::Iparser & parser ) const
{
	parser.output( "[", 1 );
	array_foreach( pp::const_attribute_list_t::const_iterator, it, get_attribute_list() )
	{
		it->output( parser );
	}
	parser.output( "]", 1 );

	return STATUS_SUCCESS;
}

/*END OF attribute_list.cxx*/
