/*context.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::dbg::context::context( const core::Iidentifier * ident ) noexcept:
	super( ident ),
	m_state( ws_uninitialized )
{
}
f1::pp::dbg::context::~context()
{
	DBG_HEAP_CHECK();
}

const status_t f1::pp::dbg::context::initialize() noexcept
{
	status_t status;

	// Initialize base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Conntect to the PP context
	m_pp_context.createT<pp_context>( std::ref(*this) );
	m_pp_context->initialize();

	m_pp_context->append_standards( pp::service::get().get_standard_list() );

	m_pp_event_handler.initialize();

	// Exit
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const f1::pp::context * f1::pp::dbg::context::get_pp_context() const noexcept
{
	return m_pp_context;
}
f1::pp::context * f1::pp::dbg::context::get_pp_context() noexcept
{
	return m_pp_context;
}

const f1::pp::Iparser * f1::pp::dbg::context::get_pp_parser() const noexcept
{
	return m_pp_parser;
}
f1::pp::Iparser * f1::pp::dbg::context::get_pp_parser() noexcept
{
	return m_pp_parser;
}

const f1::parser::context * f1::pp::dbg::context::get_parser_context() const
{
	return nullptr == m_pp_parser ? nullptr : &m_pp_parser->get_parser_context();
}
f1::parser::context * f1::pp::dbg::context::get_parser_context()
{
	return nullptr == m_pp_parser ? nullptr : &m_pp_parser->get_parser_context();
}

f1::pp::dbg::context::working_state_t f1::pp::dbg::context::get_state() const noexcept
{
	return m_state;
}
const status_t f1::pp::dbg::context::set_state( working_state_t state ) noexcept
{
	__debugbreak_if( unsigned(state) >= ws_state_count )
		return STATUS_ERROR_INVALID_PARAMETER;
	m_state = state;
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sources

const status_t f1::pp::dbg::context::append_source( const core::string & name )
{
	__debugbreak_if( nullptr != get_source_desc( name ) )
		return STATUS_ERROR_ALREADY_EXISTS;

	source_desc * desc = m_source_desc_list.append_new();
	desc->m_filename = name;

	return STATUS_SUCCESS;
}
const status_t f1::pp::dbg::context::remove_source( const core::string & name )
{
	source_desc_list_t::iterator it = find_source_desc( name );
	__debugbreak_if( !it.valid() )
		return STATUS_ERROR_NOT_FOUND;
	m_source_desc_list.erase( it );
	return STATUS_SUCCESS;
}
const status_t f1::pp::dbg::context::remove_all_sources()
{
	return m_source_desc_list.remove_all();
}

f1::pp::dbg::context::source_desc_list_t::const_iterator f1::pp::dbg::context::find_source_desc( const core::string & name ) const
{
	source_desc_list_t::const_iterator it;
	array_foreach_i( source_desc_list_t::const_iterator, it, m_source_desc_list )
	{
		if( (*it).m_filename == name )
			break;
	}
	return it;
}
f1::pp::dbg::context::source_desc_list_t::iterator f1::pp::dbg::context::find_source_desc( const core::string & name )
{
	source_desc_list_t::iterator it;
	array_foreach_i( source_desc_list_t::iterator, it, m_source_desc_list )
	{
		if( (*it).m_filename == name )
			break;
	}
	return it;
}

const f1::pp::dbg::context::source_desc * f1::pp::dbg::context::get_source_desc( const core::string & name ) const
{
	source_desc_list_t::const_iterator it = find_source_desc( name );
	return it.valid() ? &*it : nullptr;
}
f1::pp::dbg::context::source_desc * f1::pp::dbg::context::get_source_desc( const core::string & name )
{
	source_desc_list_t::iterator it = find_source_desc( name );
	return it.valid() ? &*it : nullptr;
}

const f1::pp::dbg::context::source_desc_list_t & f1::pp::dbg::context::get_source_desc_list() const
{
	return m_source_desc_list;
}
f1::pp::dbg::context::source_desc_list_t & f1::pp::dbg::context::get_source_desc_list()
{
	return m_source_desc_list;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::pp::dbg::context::begin_session( f1::parser::Isource & src )
{
	__debugbreak_if( ws_uninitialized != m_state )
		return STATUS_ERROR_INVALID_STATE;

	// Initialize preprocessor
	m_pp_context->append_source( src );

	m_pp_event_handler.m_dbg_context = this;
	m_pp_context->append_event_handler( pp::context::event_id_t::ev_parse_iteration,
		m_pp_event_handler
		);

	// Create working thread
	m_thread.createT<f1::mt::windows::thread>( std::ref(m_server_task), this, mt::Ithread::fl_create_suspended );
	set_state( ws_launched );
	set_state( ws_paused );
	m_thread->create();

	// Exit	
	return STATUS_SUCCESS;
}
const status_t f1::pp::dbg::context::end_session()
{
	if( ws_uninitialized == m_state )
		return STATUS_SUCCESS;

	if( ws_running == m_state )
		abort_session();
	set_state( ws_finished );
	set_state( ws_uninitialized );

	if( nullptr != m_pp_parser )
		m_pp_parser->abort_parser();

	m_pp_context->remove_all_sources();
	m_pp_context->set_storage( nullptr );

	m_pp_context->remove_event_handler( pp::context::event_id_t::ev_parse_iteration,
		m_pp_event_handler
		);
	m_pp_event_handler.m_dbg_context = nullptr;

//	m_thread = nullptr;
//	m_pp_parser = nullptr;

	return STATUS_SUCCESS;
}

const status_t f1::pp::dbg::context::abort_session()
{
	__debugbreak_if( ws_uninitialized == m_state )
		return STATUS_ERROR_INVALID_STATE;

	if( nullptr != m_thread )
	{
		set_state( ws_abort );
		m_thread->resume();

		//m_thread->destroy();
		m_thread = nullptr;
	}
	set_state( ws_stopped );

	return STATUS_SUCCESS;
}

const status_t f1::pp::dbg::context::resume_session()
{
	__debugbreak_if( ws_single_step != m_state && ws_running != m_state && ws_paused != m_state )
		return STATUS_ERROR_INVALID_STATE;

	set_state( ws_running );
	m_thread->resume();

	return STATUS_SUCCESS;
}

const status_t f1::pp::dbg::context::step_session()
{
	__debugbreak_if( ws_single_step != m_state && ws_uninitialized != m_state && ws_paused != m_state )
		return STATUS_ERROR_INVALID_STATE;

	set_state( ws_single_step );
	m_thread->resume();

	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

f1::pp::dbg::context::pp_context::pp_context( dbg::context & dbg_context ):
	m_dbg_context( &dbg_context )
{
}
const status_t f1::pp::dbg::context::pp_context::on_parser_attached( pp::Iparser & parser )
{
	m_dbg_context->m_pp_parser = &parser;
	return super::on_parser_attached( parser );
}
const status_t f1::pp::dbg::context::pp_context::on_parser_detached( pp::Iparser & parser )
{
	//m_dbg_context->m_pp_parser = nullptr;
	return super::on_parser_detached( parser );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::dbg::context::server_task::run( void * param, mt::Ithread_context * tc )
{
	f1::pp::dbg::context & ctx = *reinterpret_cast<f1::pp::dbg::context *>(param);

	__debugbreak_if( nullptr == ctx.get_pp_context() )
		return STATUS_SUCCESS;

	core::Tref<pp::Ioutput> outp = ctx.get_pp_context()->get_output();
	__debugbreak_if( nullptr == outp )
		return STATUS_ERROR_NOT_INITIALIZED;
	outp->begin_output();

	ctx.get_pp_context()->process();

	if( nullptr != outp )
		outp->end_output();

	if( ws_uninitialized != ctx.m_state )
	{
		ctx.set_state( ws_stopped );
		ctx.end_session();
	}

	__unreferenced_parameter( tc );
	DBG_HEAP_CHECK();
	return STATUS_SUCCESS;
}
void f1::pp::dbg::context::server_task::query_stop( mt::Ithread_context * tc )
{
	__unreferenced_parameter( tc );
	return;
}

void f1::pp::dbg::context::pp_event_handler::invoke( pp::context::event_id_t id, void * param )
{
	core::Tref<pp::Iparser> par = type_cast<pp::Iparser *>(reinterpret_cast<core::object *>(param));
	//OutputDebugString( "*" );

	if( nullptr != m_dbg_context && ws_single_step == m_dbg_context->m_state )
	{
		SuspendThread( GetCurrentThread() );
		if( nullptr != m_dbg_context && ws_abort == m_dbg_context->m_state )
		{
			par->abort_parser();
		}
	}

	__unreferenced_parameter( id );
	__unreferenced_parameter( param );
	return;
}
void f1::pp::dbg::context::pp_event_handler::on_detach( pp::context & ctx )
{
	__unreferenced_parameter( ctx );
}

/*END OF context.cxx*/
