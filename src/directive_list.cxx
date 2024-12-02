/*directive_list.cxx*/
#include "pch.h"
#include "main.h"

// directive search deep-first

/*f1::pp::directive_list::f1::pp::directive_list( pp_context & context ):
	m_context( &context ), m_file( nullptr ), m_parent( nullptr )
{
}
f1::pp::directive_list::f1::pp::directive_list( pp_file & file ):
	m_context( file.get_context() ), m_file( &file ), m_parent( nullptr )
{
}*/
f1::pp::directive_list::directive_list() __noexcept:
	m_parent( nullptr )
{
}
f1::pp::directive_list::~directive_list() __noexcept
{
	remove_all_directives();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*pp_context * f1::pp::directive_list::get_context() const
{
	return m_context;
}

pp_file * f1::pp::directive_list::get_file() const
{
	return m_file;
}*/

const f1::guid_t & f1::pp::directive_list::get_guid() const __noexcept
{
	return f1::guid_t::null;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::directive_list::append_directive( const pp::Idirective & directive ) __noexcept
{
	if( service::get().get_flags( service::fl_debug_output ) )
	{
		DBG_PRINTF(( "INFO: adding directive: %s\n", directive.get_name() ));
	}

	core::Tref<pp::Idirective> existing_directive = nullptr;
	if( 0 == directive.get_caps( Idirective::cap_override ) )
	{
		if( find_directive( existing_directive, directive.get_guid() ).succeeded() )
		{
			if( service::get().get_flags( service::fl_debug_output ) )
			{
				DBG_PRINTF(( "WARNING: directive has already been added: %s\n", directive.get_name() ));
			}
			return STATUS_SUCCESS;
		}

		std::string name( directive.get_name() );
		name_map_t::iterator it = m_name_list.find( name );
		__debugbreak_if( m_name_list.end() != it )
			return STATUS_ERROR_ALREADY_OWNED;
	}

	if( nullptr == existing_directive || 0 != directive.get_caps( Idirective::cap_override ) )
	{
		m_name_list.insert( std::pair<std::string, core::Tconst_ref<pp::Idirective> >( directive.get_name(), &directive ) );
		m_guid_list.insert( std::pair<		guid_t,core::Tconst_ref<pp::Idirective> >( directive.get_guid(), &directive ) );
	}
	m_directive_list.append( &directive );

	return STATUS_SUCCESS;
}
const status_t f1::pp::directive_list::append_directives( const pp::directive_list & list ) __noexcept
{
	array_foreach( const_directive_list_t::const_iterator, it, list.m_directive_list )
		append_directive( **it );
	return STATUS_SUCCESS;
}
const status_t f1::pp::directive_list::append_directives( const pp::directive_list_t & list ) __noexcept
{
	array_foreach( directive_list_t::const_iterator, it, list )
		append_directive( **it );
	return STATUS_SUCCESS;
}
const status_t f1::pp::directive_list::remove_directive( const pp::Idirective & directive ) __noexcept
{
	if( service::get().get_flags( service::fl_debug_output ) )
	{
		DBG_PRINTF(( "INFO: removing directive: %s\n", directive.get_name() ));
	}

	std::string name( directive.get_name() );
	name_map_t::iterator it = m_name_list.find( name );
	__debugbreak_if( m_name_list.end() == it )
		return STATUS_ERROR_NOT_FOUND;

	m_name_list.erase( it );
	m_guid_list.erase( m_guid_list.find( directive.get_guid() ) );
	m_directive_list.remove( &directive );

	return STATUS_SUCCESS;
}
const status_t f1::pp::directive_list::remove_directives( const pp::directive_list & list ) __noexcept
{
	array_foreach( const_directive_list_t::const_iterator, it, list.m_directive_list )
		remove_directive( **it );
	return STATUS_SUCCESS;
}
const status_t f1::pp::directive_list::remove_directives( const pp::directive_list_t & list ) __noexcept
{
	array_foreach( directive_list_t::const_iterator, it, list )
		remove_directive( **it );
	return STATUS_SUCCESS;
}
const status_t f1::pp::directive_list::remove_all_directives() __noexcept
{
	if( service::get().get_flags( service::fl_debug_output ) )
	{
		DBG_PRINTF(( "INFO: removing all directives\n" ));
	}

	for( name_map_t::iterator it = m_name_list.begin(); it != m_name_list.end(); ++ it )
		it->second.clear();
	m_name_list.clear();
	m_guid_list.clear();
	m_directive_list.clear();

	return STATUS_SUCCESS;
}

const status_t f1::pp::directive_list::find_directive( core::Tconst_ref<Idirective> & res, unsigned flags, const f1::pp::string & name ) const __noexcept
{
	return find_directive( res, flags, name.data(), name.length() );
}
const status_t f1::pp::directive_list::find_directive( core::Tconst_ref<Idirective> & res, unsigned flags, const f1::core::stringslice & name ) const __noexcept
{
	return find_directive( res, flags, name.data(), name.length() );
}
const status_t f1::pp::directive_list::find_directive( core::Tconst_ref<Idirective> & res, unsigned flags, const char * str, size_t len ) const __noexcept
{
	core::string name( str, len );
#if 1
	static bool (core::string::*const equal_fn1)( const char * s ) const noexcept = &core::string::equal;
	static bool (core::string::*const equal_fn2)( const char * s ) const noexcept = &core::string::equal_nocase;
	bool (core::string::*const equal_fn)( const char * s ) const noexcept = (flags & fl_compare_no_case) ?
		equal_fn2:
		equal_fn1;
	array_foreach( const_directive_list_t::const_reverse_iterator, it, m_directive_list )
	{
		if( (name.*equal_fn)( it->get_name() ) )
		{
			if( it->get_flags( Idirective::bf_disabled ) )
			{
				res = nullptr;
				return STATUS_ERROR_DISABLED;
			}
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
#else
	name_map_t::const_iterator it = m_name_list.find( name.c_str() );
	if( m_name_list.end() == it )
	{
		res = nullptr;
		return STATUS_ERROR_NOT_FOUND;
	}
	if( it->second->get_flags( Idirective::bf_disabled ) )
	{
		res = nullptr;
		return STATUS_ERROR_DISABLED;
	}

	res = it->second;
	return STATUS_SUCCESS;
#endif
}
const status_t f1::pp::directive_list::find_directive( core::Tconst_ref<pp::Idirective> & res, const guid_t & guid ) const
{
#if 1	// TODO: two objects with the same GUID, found the first one, how to find the last one instead?
	array_foreach( const_directive_list_t::const_reverse_iterator, it, m_directive_list )
	{
		if( guid == it->get_guid() )
		{
			if( it->get_flags( Idirective::bf_disabled ) )
			{
				res = nullptr;
				return STATUS_ERROR_DISABLED;
			}
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
#else
	guid_map_t::const_iterator it = m_guid_list.find( guid );
	if( m_guid_list.end() == it )
	{
		res = nullptr;
		return STATUS_ERROR_NOT_FOUND;
	}
	if( it->second->get_flags( Idirective::bf_disabled ) )
	{
		res = nullptr;
		return STATUS_ERROR_DISABLED;
	}

	res = it->second;
	return STATUS_SUCCESS;
#endif
}

const f1::pp::const_directive_list_t & f1::pp::directive_list::get_directive_list() const
{
	return m_directive_list;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::directive_list::append_extension( f1::pp::directive_list & dlist )
{
	if( nullptr != dlist.m_parent )
		return STATUS_ERROR_ALREADY_OWNED;
#ifdef _DEBUG
	array_foreach( core::Tobject_array<f1::pp::directive_list>::reverse_iterator, it, m_children )
	{
		if( &dlist == (*it) )
			return STATUS_ERROR_ALREADY_EXISTS;
	}
#endif // _DEBUG

	m_children.push_back( &dlist );
	dlist.m_parent = this;

	return STATUS_SUCCESS;
}
const status_t f1::pp::directive_list::remove_extension( f1::pp::directive_list & dlist )
{
	if( this != dlist.m_parent )
		return STATUS_ERROR_NOT_OWNED;
	m_children.remove( &dlist );
	dlist.m_parent = nullptr;

	return STATUS_SUCCESS;
}
const status_t f1::pp::directive_list::remove_all_extensions()
{
	array_foreach( core::Tobject_array<f1::pp::directive_list>::reverse_iterator, it, m_children )
		(*it)->m_parent = nullptr;
	m_children.clear();

	return STATUS_SUCCESS;
}

f1::pp::directive_list * f1::pp::directive_list::get_parent()
{
	return m_parent;
}
const f1::pp::directive_list * f1::pp::directive_list::get_parent() const
{
	return m_parent;
}

const status_t f1::pp::directive_list::find_extension( core::Tref<f1::pp::directive_list> & res, const guid_t & guid ) const
{
	array_foreach( core::Tobject_array<f1::pp::directive_list>::const_reverse_iterator, it, m_children )
	{
		if( (*it)->get_guid() == guid )
		{
			res = (*it);
			return STATUS_SUCCESS;
		}
	}

	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}

/*END OF directive_list.cxx*/
