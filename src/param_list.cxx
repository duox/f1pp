/*param_list.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::param_list::~param_list() noexcept
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::param_list::append_param( unsigned flags, const char * name, size_t len )
{
	if( size_t(-1) == len )
		len = core::string::length( name );

	if( 0 != len )
	{
		pp::string nm(name, len);
		array_foreach( param_desc_list_t::const_iterator, it, m_params ) {
			__debugbreak_if( (*it).m_name.equal_nocase( nm ) )
				return STATUS_ERROR_ALREADY_EXISTS;
		}
	}

	param_desc_t * desc = m_params.append_new();
	__debugbreak_if( nullptr == desc )
		return STATUS_ERROR_INSUFFICIENT_MEMORY;
	if( nullptr != name )
		desc->m_name.add_part_alloc( name, len );
	desc->m_flags = flags;

	return STATUS_SUCCESS;
}
const status_t f1::pp::param_list::append_param( unsigned flags, const core::string & name )
{
	return append_param( flags, name.c_str(), name.size() );
}
const status_t f1::pp::param_list::append_param( unsigned flags, const core::stringslice & name )
{
	return append_param( flags, name.data(), name.length() );
}
const status_t f1::pp::param_list::append_param( const param_desc_t & desc, size_t count )
{
	status_t status;

	const param_desc_t * d = &desc;
	for( size_t i = 0; i < count; ++ i, ++ d ) {
		if( append_param( d->m_flags, d->m_name ).failed_status( status ) )
			return status;
	}

	return STATUS_SUCCESS;
}
const status_t f1::pp::param_list::remove_all_params()
{
	m_params.clear();
	return STATUS_SUCCESS;
}

const f1::pp::param_list::param_desc_list_t & f1::pp::param_list::get_param_list() const
{
	return m_params;
}
const f1::pp::param_list::param_desc_t * f1::pp::param_list::get_param_desc( size_t index ) const
{
	__debugbreak_if( index >= m_params.size() )
		return nullptr;
	return &m_params[index];
}

signed f1::pp::param_list::find_param( const pp::string & name ) const
{
	int index = 0;
	array_foreach( param_desc_list_t::const_iterator, it, m_params ) {
		const param_desc_t & desc = *it;
		if( desc.m_flags & fl_vararg )
			break;
		if( desc.m_name.equal( name ) )
			return index;
		++ index;
	}
	return -1;
}

bool __nodiscard f1::pp::param_list::equal( const core::object * obj ) const noexcept
{
	if( nullptr == obj )
		return false;

	core::Tconst_weak_ref<param_list> params = type_cast<const param_list *>( obj );
	__debugbreak_if( nullptr == params )
		return false;	//STATUS_ERROR_INVALID_TYPE;

	return equal( *params );
}
bool __nodiscard f1::pp::param_list::equal( const param_list & params ) const
{
	return m_params.size() == params.m_params.size();
	/*for( param_desc_list_t::const_iterator it1(m_params), it2(params.m_params); it1.valid() && it2.valid(); it1.next(), it2.next() )
	{
		const param_desc_t & param1 = *it1;
		const param_desc_t & param2 = *it2;
		if( !param1.m_name.equal( param2.m_name ) )
			return false;
	}
	return true;*/
}

bool f1::pp::param_list::match_param_list( const param_list & params ) const
{
	// Check both list for equal size
	if( m_params.size() != params.m_params.size() )
		return false;

	// Check for both lists to have or not have vararg parameter
	const param_desc_t * param1 = &*m_params.rbegin();
	const param_desc_t * param2 = &*params.m_params.rbegin();
	if( (param1->m_flags & fl_vararg) != (param2->m_flags & fl_vararg) )
		return false;

	// Check for default values equality
	array_foreach2( param_desc_list_t::const_iterator, it1, it2, m_params, params.m_params )
	{
		param1 = &*it1;
		param2 = &*it2;

		if( (param1->m_flags & fl_default_value) != (param2->m_flags & fl_default_value) )
			return false;
		if( param1->m_flags & fl_default_value )
		{
			if( param1->m_default_value != param2->m_default_value )
				return false;
		}
	}

	// Exit
	return true;
}
bool f1::pp::param_list::match_arg_list( const arg_list & args ) const
{
	const arg_list::arg_desc_list_t &	al = args.get_arg_list();

	if( al.size() == m_params.size() )
		return true;
	if( m_params.empty() )
	{
		if( args.get_arg_list().size() > 1 )
			return false;

		//const arg_list::arg_desc_t & last_arg = *args.get_arg_list().rbegin();
		//return last_arg.m_expanded_value.empty();
		return true;	// even empty argument is the argument for a one-parameter macro
	}
	if( al.size() < m_params.size() )		// insufficient number of arguments
	{
		for( size_t i = al.size(), n = m_params.size(); i < n; ++ i )
		{
			const param_desc_t & param = m_params[i];
			if( 0 != (param.m_flags & fl_vararg) )			// vararg parameter, so all furhter arguments are parts of it
				return true;
			if( 0 == (param.m_flags & fl_default_value) )	// parameter is required
				return false;
		}
	}

	return true;
}
bool f1::pp::param_list::match_arg_list( const pp::param_list * params, const pp::arg_list * args )
{
	// No params - should be no args
	if( nullptr == params )
		return nullptr == args;
	if( nullptr == args )
		return false;

	// Check argument count, considering presence of default arguments
	return params->match_arg_list( *args );
}

const status_t f1::pp::param_list::format( core::string & buf ) const noexcept
{
	bool first = true;
	array_foreach( param_desc_list_t::const_iterator, it, m_params )
	{
		const param_desc_t & param = *it;

		if( !first )
			buf.append( ", " );
		else
			first = false;

		if( param.m_flags & fl_vararg )
		{
			buf.append( "... " );
			if( !param.m_name.empty() )
				buf.append( param.m_name.c_str() );
		}
		else
		{
			if( !param.m_name.empty() )
				buf.append( param.m_name.c_str() );
			else
				buf.append( "?" );
		}

		if( param.m_flags & fl_default_value )
		{
			const char * default_value = param.m_default_value.c_str();
			if( nullptr == default_value )
				default_value = "?";
			buf.append( "= " );
			buf.append( default_value );
		}
	}
	return STATUS_SUCCESS;
}

/*END OF param_list.cxx*/
