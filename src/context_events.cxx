/*context_events.cxx*/
// Events

#include "pch.h"
#include "main.h"


bool f1::pp::context::is_event_enabled( event_id_t id ) const
{
	__debugbreak_if( size_t(id) >= _countof(m_event) )
		return false;
	return !!m_event_mask.get_bit( id );
}

const status_t f1::pp::context::append_event_handler( event_id_t id, Ievent_handler & handler )
{
	__debugbreak_if( size_t(id) >= _countof(m_event) )
		return STATUS_ERROR_INVALID_PARAMETER;
	event_desc_t * desc = m_event + id;

	Ievent_handler * curr_handler = desc->m_handler_first;
	for( ; nullptr != curr_handler; curr_handler = curr_handler->m_handler_next )
	{
		if( curr_handler == &handler )
			return STATUS_ERROR_ALREADY_EXISTS;
	}

	if( nullptr == desc->m_handler_last )
		desc->m_handler_first = desc->m_handler_last = &handler;
	else
		desc->m_handler_last = desc->m_handler_last->m_handler_next = &handler;

	m_event_mask.set_bit( id );
	handler.on_attach( *this );

	return STATUS_SUCCESS;
}
const status_t f1::pp::context::remove_event_handler( event_id_t id, Ievent_handler & handler )
{
	__debugbreak_if( size_t(id) >= _countof(m_event) )
		return STATUS_ERROR_INVALID_PARAMETER;
	event_desc_t * desc = m_event + id;

	core::Tref<Ievent_handler> curr_handler = desc->m_handler_first;
	core::Tref<Ievent_handler> prev_handler;
	for( ; nullptr != curr_handler; curr_handler = curr_handler->m_handler_next )
	{
		if( curr_handler == &handler )
		{
			if( nullptr == prev_handler )
				desc->m_handler_first = curr_handler->m_handler_next;
			else
				prev_handler->m_handler_next = curr_handler->m_handler_next;

			if( nullptr == curr_handler->m_handler_next )
				desc->m_handler_last = prev_handler;

			__assert( (nullptr == desc->m_handler_first && nullptr == desc->m_handler_last) || 
					  (nullptr != desc->m_handler_first && nullptr != desc->m_handler_last) 
				);

			if( nullptr == desc->m_handler_first )
				m_event_mask.clear_bit( id );

			handler.on_detach( *this );

			return STATUS_SUCCESS;
		}
		prev_handler = curr_handler;
	}

	DBG_FAIL();
	return STATUS_ERROR_NOT_FOUND;
}
const status_t f1::pp::context::remove_all_event_handlers( Ievent_handler & handler )
{
	for( int i = 0; i < _countof(m_event); ++ i )
	{
		if( m_event_mask.get_bit( i ) )
			remove_event_handler( event_id_t(i), handler );
	}
	return STATUS_SUCCESS;
}
const status_t f1::pp::context::remove_all_event_handlers()
{
	event_desc_t * desc = m_event;
	for( int i = 0; i < _countof(m_event); ++ i, ++ desc )
	{
		core::Tref<Ievent_handler> handler = desc->m_handler_first;
		core::Tref<Ievent_handler> next_handler;
		for( ; nullptr != handler; handler = next_handler )
		{
			next_handler = handler->m_handler_next;
			handler->m_handler_next = nullptr;
			handler->on_detach( *this );
		}
		desc->m_handler_first = desc->m_handler_last = nullptr;
	}
	m_event_mask.clear();
	return STATUS_SUCCESS;
}

const status_t f1::pp::context::invoke_event( event_id_t id, void * param )
{
	__debugbreak_if( size_t(id) >= _countof(m_event) )
		return STATUS_ERROR_INVALID_PARAMETER;
	if( !m_event_mask.get_bit( id ) )
		return STATUS_SUCCESS;

	event_desc_t * desc = m_event + id;

	core::Tref<Ievent_handler> handler = desc->m_handler_first;
	core::Tref<Ievent_handler> next_handler;
	for( ; nullptr != handler; handler = next_handler )
	{
		next_handler = handler->m_handler_next;
		handler->invoke( id, param );
	}
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

f1::pp::context::Ievent_handler::~Ievent_handler()
{
}
const status_t f1::pp::context::Ievent_handler::on_attach( pp::context & ctx )
{
	__unreferenced_parameter( ctx );
	return STATUS_SUCCESS;
}
void f1::pp::context::Ievent_handler::on_detach( pp::context & ctx )
{
	__unreferenced_parameter( ctx );
	return;
}
void f1::pp::context::Ievent_handler::invoke( event_id_t event_id, void * param )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( event_id );
	__unreferenced_parameter( param );
	return;
}

/*END OF context.cxx*/
