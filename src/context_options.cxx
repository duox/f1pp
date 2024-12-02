/*context_options.cxx*/
/** @file
 * @brief Options management.
 *
 *	Options can override existing options.
 */
#include "pch.h"
#include "main.h"

const status_t f1::pp::context::create_option( core::Tref<Ioption> & res,
	pp::variant::type_t ty,
	const f1::guid_t & guid,
	const char * name, size_t len )
{
	status_t status;

	// Create and initialize new option
	core::Tref<option> obj;
	if( obj.createT<option>().failed_status( status ) )
		return status;
	if( obj->initialize( ty, guid, name, len ).failed_status( status ) )
		return status;

	if( append_option( *obj ).failed_status( status ) )
		return status;

	// Exit
	res = obj;
	return STATUS_SUCCESS;
}

const status_t f1::pp::context::append_option(
	pp::variant::type_t ty,
	const f1::guid_t & guid,
	const char * name, size_t len )
{
	core::Tref<Ioption> obj;
	return create_option( obj, ty, guid, name, len );
}
const status_t f1::pp::context::append_option( Ioption & opt )
{
	return m_option_list.append_unique( &opt );
}
const status_t f1::pp::context::remove_option( const Ioption & opt )
{
	return m_option_list.remove( &opt );
}
const status_t f1::pp::context::remove_options( const const_option_list_t & option_list )
{
	array_foreach( const_option_list_t::const_reverse_iterator, it, option_list )
		remove_option( **it );
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::remove_options( const core::Tarray<pp::string> & names )
{
	array_foreach( core::Tarray<pp::string>::const_iterator, it, names )
	{
		const pp::string name = *it;
		core::Tref<pp::Ioption> opt;
		__debugbreak_ifnot( find_option( opt, name.data(), name.length() ).succeeded() )
			remove_option( *opt );
	}
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::remove_options( const core::Tarray<f1::guid_t> & guids )
{
	array_foreach( core::Tarray<f1::guid_t>::const_iterator, it, guids )
	{
		core::Tref<pp::Ioption> opt;
		__debugbreak_ifnot( find_option( opt, *it ).succeeded() )
			remove_option( *opt );
	}
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::remove_all_options()
{
	return m_option_list.remove_all();
}

bool f1::pp::context::exists_option( const f1::guid_t & guid ) const
{
	core::Tconst_ref<Ioption> obj;
	return find_option( obj, guid ).succeeded();
}
bool f1::pp::context::exists_option( const char * name, size_t len ) const
{
	core::Tconst_ref<Ioption> obj;
	return find_option( obj, name, len ).succeeded();
}

const status_t f1::pp::context::find_option( core::Tref<Ioption> & res, const f1::guid_t & guid )
{
	status_t status;
	core::Tconst_ref<Ioption> opt;
	if( const_cast<const self *>(this)->find_option( opt, guid ).failed_status( status ) )
		return status;
	res = opt;
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::find_option( core::Tconst_ref<Ioption> & res, const f1::guid_t & guid ) const
{
	array_foreach( option_list_t::const_reverse_iterator, it, m_option_list )
	{
		if( it->get_guid() == guid )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	return STATUS_ERROR_NOT_FOUND;
}
const status_t f1::pp::context::find_option( core::Tref<Ioption> & res, const char * name, size_t len )
{
	status_t status;
	core::Tconst_ref<Ioption> opt;
	if( const_cast<const self *>(this)->find_option( opt, name, len ).failed_status( status ) )
		return status;
	res = opt;
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::find_option( core::Tconst_ref<Ioption> & res, const char * name, size_t len ) const
{
	if( size_t(-1) == len )
		len = core::string::length( name );
	array_foreach( option_list_t::const_reverse_iterator, it, m_option_list )
	{
		if( it->get_name().equal( name, len ) )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	return STATUS_ERROR_NOT_FOUND;
}
const status_t f1::pp::context::find_option( core::Tref<Ioption> & res, const pp::string & name )
{
	status_t status;
	core::Tconst_ref<Ioption> opt;
	if( const_cast<const self *>(this)->find_option( opt, name ).failed_status( status ) )
		return status;
	res = opt;
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::find_option( core::Tconst_ref<Ioption> & res, const pp::string & name ) const
{
	array_foreach( option_list_t::const_reverse_iterator, it, m_option_list )
	{
		if( it->get_name().equal( name.data(), name.length() ) )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	return STATUS_ERROR_NOT_FOUND;
}
const status_t f1::pp::context::find_options( const_option_list_t & res, const core::Tarray<pp::string> & names ) const
{
	array_foreach( core::Tarray<pp::string>::const_reverse_iterator, name_it, names )
	{
		const pp::string & ident = *name_it;

		option_list_t::const_reverse_iterator option_it;
		array_foreach_i( option_list_t::const_reverse_iterator, option_it, m_option_list )
		{
			if( option_it->get_name().equal( ident.data(), ident.length() ) )
			{
				res.append( *option_it );
				break;
			}
		}
		if( !option_it.valid() )
			return STATUS_ERROR_NOT_FOUND;
	}
	return STATUS_SUCCESS;
}

const f1::pp::variant & f1::pp::context::get_option_value( const f1::guid_t & guid )
{
	core::Tref<Ioption> opt;
	if( find_option( opt, guid ).failed() )
		return pp::variant::null;

	return opt->get_value();
}
const f1::pp::variant & f1::pp::context::get_option_value( const char * name, size_t len ) const
{
	core::Tref<Ioption> opt;
	if( find_option( opt, name, len ).failed() )
		return pp::variant::null;

	return opt->get_value();
}
const status_t f1::pp::context::set_option_value( const pp::variant & value, const f1::guid_t & guid )
{
	status_t status;

	core::Tref<Ioption> opt;
	if( find_option( opt, guid ).failed_status( status ) )
		return status;

	return opt->set_value( value );
}
const status_t f1::pp::context::set_option_value( const pp::variant & value, const char * name, size_t len ) const
{
	status_t status;

	core::Tref<Ioption> opt;
	if( find_option( opt, name, len ).failed_status( status ) )
		return status;

	return opt->set_value( value );
}
const status_t f1::pp::context::set_option_value( const pp::variant & value, const pp::string & name ) const
{
	status_t status;

	core::Tref<Ioption> opt;
	if( find_option( opt, name ).failed_status( status ) )
		return status;

	return opt->set_value( value );
}

const status_t f1::pp::context::push_all_options()
{
	return push_options( m_option_list );
}
const status_t f1::pp::context::push_options( const core::Tarray<f1::guid_t> & guids )
{
	status_t status;

	option_list_t options;
	array_foreach( core::Tarray<f1::guid_t>::const_iterator, it, guids )
	{
		const f1::guid_t guid = *it;

		core::Tref<pp::Ioption> opt;
		if( find_option( opt, guid ).failed_status( status ) )
			return status;

		options.append( opt );
	}
	return push_options( options );
}
const status_t f1::pp::context::push_options( const core::Tarray<pp::string> & names )
{
	status_t status;

	option_list_t options;
	array_foreach( core::Tarray<pp::string>::const_iterator, it, names )
	{
		const pp::string name = *it;

		core::Tref<pp::Ioption> opt;
		if( find_option( opt, name.data(), name.length() ).failed_status( status ) )
			return status;

		options.append( opt );
	}
	return push_options( options );
}
const status_t f1::pp::context::push_option( const f1::guid_t & guid )
{
	status_t status;

	core::Tref<pp::Ioption> opt;
	if( find_option( opt, guid ).failed_status( status ) )
		return status;
	return push_option( *opt );
}
const status_t f1::pp::context::push_option( const pp::string & name )
{
	status_t status;

	core::Tref<pp::Ioption> opt;
	if( find_option( opt, name.data(), name.length() ).failed_status( status ) )
		return status;
	return push_option( *opt );
}
const status_t f1::pp::context::push_option( pp::Ioption & opt )
{
	option_list_t options;
	options.append( &opt );

	return push_options( options );
}
const status_t f1::pp::context::push_options( const option_list_t & lst )
{
	option_stack_entry * entry = m_option_stack.append_new();

	array_foreach( option_list_t::const_iterator, it, lst )
	{
		core::Tconst_ref<pp::Ioption> opt = *it;

		option_desc * desc = entry->m_option_desc_list.append_new();
		desc->m_guid = opt->get_guid();
		desc->m_name = opt->get_name();
		desc->m_value = opt->get_value();
	}
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::pop_options()
{
	__debugbreak_if( m_option_stack.empty() )
		return STATUS_ERROR_STACK_IS_EMPTY;
	option_stack_entry * entry = m_option_stack.get_last();
	__debugbreak_if( nullptr == entry )
		return STATUS_ERROR_INTERNAL;

	array_foreach( core::Tarray<option_desc>::const_iterator, it, entry->m_option_desc_list )
	{
		const option_desc & desc = *it;

		// Try to find option
		core::Tref<pp::Ioption> opt;
		if( find_option( opt, desc.m_name.data(), desc.m_name.length() ).succeeded() )
		{
			// If name and guid match, just set the value and move to the next option
			if( find_option( opt, desc.m_guid ).succeeded() )
			{
				opt->set_value( desc.m_value );
				continue;
			}
			// Otherwise, option exists but has different GUID
		}
		else
		{
			find_option( opt, desc.m_guid );
			// If opt is not nullptr, option exists but has different name
		}

		// Undefine existing option and create the new one
		if( nullptr != opt )
			remove_option( *opt );

		create_option( opt, desc.m_value.get_type(), desc.m_guid, desc.m_name.data(), desc.m_name.length() );
		__debugbreak_if( nullptr == opt )
			continue;
		opt->set_value( desc.m_value );
	}

	// Exit
	return STATUS_SUCCESS;
}

const f1::pp::option_list_t & f1::pp::context::get_option_list() const
{
	return m_option_list;
}
f1::pp::option_list_t & f1::pp::context::get_option_list()
{
	return m_option_list;
}

/*END OF context_options.cxx*/
