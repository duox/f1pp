/*arg_list.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::arg_list::~arg_list() noexcept
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::arg_list::append_arg( const char * value, size_t len )
{
	arg_desc_t * desc = m_args.append_new();
	__debugbreak_if( nullptr == desc )
		return STATUS_ERROR_INSUFFICIENT_MEMORY;
	desc->m_value.add_part_alloc( value, len );

	return STATUS_SUCCESS;
}
const status_t f1::pp::arg_list::append_arg( const pp::string & value, size_t count )
{
	status_t status;

	const pp::string * str = &value;
	for( size_t i = 0; i < count; ++ i, ++ str ) {
		if( append_arg( str->data(), str->length() ).failed_status( status ) )
			return status;
	}

	return STATUS_SUCCESS;
}
const status_t f1::pp::arg_list::append_arg( const arg_desc_t & desc, size_t count )
{
	status_t status;

	const arg_desc_t * arg = &desc;
	for( size_t i = 0; i < count; ++ i, ++ arg ) {
		if( append_arg( arg->m_value ).failed_status( status ) )
			return status;
	}

	return STATUS_SUCCESS;
}
const status_t f1::pp::arg_list::append_expanded_arg( const pp::string & value, const pp::string & expanded_value )
{
	arg_desc_t * desc = m_args.append_new();
	__debugbreak_if( nullptr == desc )
		return STATUS_ERROR_INSUFFICIENT_MEMORY;
	desc->m_value.add_part_alloc( value );
	desc->m_expanded_value.add_part_alloc( expanded_value );

	return STATUS_SUCCESS;
}
const status_t f1::pp::arg_list::remove_all_args()
{
	m_args.clear();
	return STATUS_SUCCESS;
}

const f1::pp::arg_list::arg_desc_t * f1::pp::arg_list::get_arg_desc( size_t index ) const
{
	if( index >= m_args.size() )
		return nullptr;
	return &m_args[index];
}

const f1::pp::string * f1::pp::arg_list::get_arg_value( const pp::param_list & params, size_t index ) const
{
	if( index < m_args.size() )
		return &m_args[index].m_value;

	if( index < params.get_param_list().size() )
		return nullptr;
	const param_list::param_desc_t & desc = params.get_param_list()[index];
	if( 0 == (desc.m_flags & param_list::fl_default_value) )
		return nullptr;
	return &desc.m_default_value;
}
const f1::pp::string * f1::pp::arg_list::get_arg_expanded_value( const pp::param_list & params, size_t index ) const
{
	if( index < m_args.size() )
		return &m_args[index].m_expanded_value;

	if( index < params.get_param_list().size() )
		return nullptr;
	const param_list::param_desc_t & desc = params.get_param_list()[index];
	if( 0 == (desc.m_flags & param_list::fl_default_value) )
		return nullptr;
	return &desc.m_default_value;
}

const f1::pp::arg_list::arg_desc_list_t & f1::pp::arg_list::get_arg_list() const
{
	return m_args;
}
f1::pp::arg_list::arg_desc_list_t & f1::pp::arg_list::get_arg_list()
{
	return m_args;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::pp::arg_list::format( core::string & buf ) const noexcept
{
	bool first = true;
	array_foreach( pp::arg_list::arg_desc_list_t::const_iterator, it, m_args )
	{
		const arg_desc_t & arg = *it;

		if( !first )
			buf.append( ", " );
		else
			first = false;

		buf.append( arg.m_expanded_value.empty() ?
			arg.m_value.c_str() :
			arg.m_expanded_value.c_str()
			);
	}

	return STATUS_SUCCESS;
}

const status_t f1::pp::arg_list::output( pp::Iparser & parser ) const
{
	parser.output( "(", 1 );
	array_foreach( pp::arg_list::arg_desc_list_t::const_iterator, it, m_args )
	{
		parser.output( (*it).m_expanded_value.empty() ?
			(*it).m_value :
			(*it).m_expanded_value
			);
	}
	parser.output( ")", 1 );

	return STATUS_SUCCESS;
}

/*END OF arg_list.cxx*/
