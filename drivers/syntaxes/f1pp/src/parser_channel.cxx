/*parser_channel.cxx*/3
#include "pch.h"
#include "main.h"

const status_t f1::f1pp::parser::_initialize_channels()
{
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::_deinitialize_channels()
{
	remove_all_channels();
	return STATUS_SUCCESS;
}

const status_t f1::f1pp::parser::create_channel( core::Tref<f1pp::channel> & res,
	const pp::string & name, const pp::string & output_name, const f1::guid_t * output_class,
	const core::Iidentifier * ident )
{
	status_t status;

	core::Tref<f1pp::channel> obj;
	if( obj.createT<f1pp::channel>( ident ).failed_status( status ) )
		return status;
	if( obj->initialize( *this, name, output_name, output_class ).failed_status( status ) )
		return status;

	m_channel_list.append( obj );

	res = obj;
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::remove_channel( channel & chan )
{
	__debugbreak_if( m_active_channel == &chan )
		return STATUS_ERROR_INVALID_STATE;
	chan.get_storage()->close();
	return m_channel_list.remove( &chan );
}
const status_t f1::f1pp::parser::remove_all_channels()
{
	if( nullptr != m_active_channel )
		set_active_channel( nullptr );

	array_foreach( channel_list_t::iterator, it, m_channel_list )
		it->get_storage()->close();
	return m_channel_list.remove_all();
}

const status_t f1::f1pp::parser::find_channel( core::Tconst_ref<channel> & res, const pp::string & name ) const
{
	array_foreach( channel_list_t::const_iterator, it, m_channel_list )
	{
		if( it->get_name().equal( name ) )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	return STATUS_ERROR_NOT_FOUND;
}

const status_t f1::f1pp::parser::set_active_channel( channel * sec )
{
	if( nullptr != m_active_channel )
		m_active_channel->deactivate();
	m_active_channel = sec;
	if( nullptr != sec )
		sec->activate();
	else
	{
		set_storage( m_storage );
		m_storage->set_newline_flag( true );
	}
	set_newline();

	return STATUS_SUCCESS;
}
const f1::f1pp::channel * f1::f1pp::parser::get_active_channel() const
{
	return m_active_channel;
}
f1::f1pp::channel * f1::f1pp::parser::get_active_channel()
{
	return m_active_channel;
}

const status_t f1::f1pp::parser::push_active_channel()
{
	if( nullptr == m_active_channel )
		return STATUS_ERROR_INVALID_STATE;

	m_channel_stack.append( m_active_channel );

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::pop_active_channel()
{
	if( m_channel_stack.empty() )
		return STATUS_ERROR_INVALID_STATE;

	m_active_channel = *m_channel_stack.rbegin();
	m_channel_stack.pop_back();

	return STATUS_SUCCESS;
}

/*END OF parser_channel.cxx*/
