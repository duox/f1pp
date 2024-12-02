/*context.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::frontend::context::context( const core::Iidentifier * ident ):
	super( ident )
{
	//set_mt_context( nullptr );
//	m_thread_pool.createT<f1::mt::default_threadpool::thread_pool>();	// TODO: memory corruption (critical error reported by _heapchk)
//	m_thread_pool_factory.m_frontend_context = this;
//	m_thread_pool->set_factory( m_thread_pool_factory );
}
f1::pp::frontend::context::~context()
{
	m_thread_pool = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*f1::mt::context * f1::pp::frontend::context::get_mt_context()
{
	return m_mt_context;
}
const f1::mt::context * f1::pp::frontend::context::get_mt_context() const
{
	return m_mt_context;
}
const status_t f1::pp::frontend::context::set_mt_context( mt::context * ctx )
{
	if( nullptr != ctx )
		m_mt_context = ctx;
	else
	{
		status_t status = m_mt_context.createT<mt::context>();
		if( status.failed() || m_mt_context->initialize().failed_status( status ) )
			return status;
	}
	return STATUS_SUCCESS;
}*/

const status_t f1::pp::frontend::context::set_thread_count( unsigned count )
{
	return m_thread_pool->set_thread_count( count );
}
unsigned f1::pp::frontend::context::get_thread_count() const
{
	return m_thread_pool->get_thread_count();
}

const f1::guid_t & f1::pp::frontend::context::get_syntax_guid() const
{
	return m_syntax_guid;
}
const status_t f1::pp::frontend::context::set_syntax_guid( const f1::guid_t & guid )
{
	m_syntax_guid = guid;
	return STATUS_SUCCESS;
}

const f1::guid_t & f1::pp::frontend::context::get_architecture_guid() const
{
	return m_arch_guid;
}
const status_t f1::pp::frontend::context::set_architecture_guid( const f1::guid_t & guid )
{
	m_arch_guid = guid;
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::frontend::context::parse_file( const char * filename, size_t len )
{
	status_t status;

	// Create new task
	core::Tref<pp_file_task> tsk;
	__debugbreak_if( tsk.createT<pp_file_task>( m_syntax_guid, m_arch_guid, filename, len ).failed_status( status ) )
		return status;

	// Queue the task
	if( m_thread_pool->enqueue_task( *tsk ).failed_status( status ) )
		return status;

	m_task_list.append( tsk );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::pp::frontend::context::parse_data( const void * data, size_t size, const char * filename )
{
	status_t status;

	// Create new task
	core::Tref<pp_mem_task> tsk;
	__debugbreak_if( tsk.createT<pp_mem_task>( m_syntax_guid, m_arch_guid, data, size, filename ).failed_status( status ) )
		return status;

	// Queue the task
	if( m_thread_pool->enqueue_task( *tsk ).failed_status( status ) )
		return status;

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::pp::frontend::context::parse( f1::core::buffer & buf, const char * filename )
{
	status_t status;

	// Create new task
	core::Tref<pp_buffer_task> tsk;
	__debugbreak_if( tsk.createT<pp_buffer_task>( m_syntax_guid, m_arch_guid, buf, filename ).failed_status( status ) )
		return status;

	// Queue the task
	if( m_thread_pool->enqueue_task( *tsk ).failed_status( status ) )
		return status;

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct thread_context : public f1::mt::Ithread_context {
		explicit thread_context( f1::pp::frontend::context & ctx ):m_frontend_context( &ctx ){}
		status_t const on_detach_from_thread( f1::mt::Ithread & ) override { return STATUS_SUCCESS; }
		status_t const on_attach_to_thread( f1::mt::Ithread & ) override { return STATUS_SUCCESS; }
		f1::pp::context	m_pp_context;
		f1::core::Tweak_ref<f1::pp::frontend::context>	m_frontend_context;
	};

const status_t f1::pp::frontend::context::thread_context_factory::new_thread_context( f1::core::Tref<mt::Ithread_context> & res )
{
	return res.createT<thread_context>( *m_frontend_context );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void f1::pp::frontend::context::pp_task::query_stop( mt::Ithread_context * tc )
{
	__unreferenced_parameter( tc );
}
const status_t f1::pp::frontend::context::pp_file_task::run( void * param, mt::Ithread_context * tc )
{
	status_t status;

	// Get context
	core::Tref<thread_context> atc = type_cast<thread_context *>(tc);
	__debugbreak_if( nullptr == atc )
		return STATUS_ERROR_INVALID_STATE;
	if( !m_syntax_guid.is_null() && atc->m_pp_context.set_syntax( m_syntax_guid ).failed_status( status ) )
		return status;

	// Start assembler
	if( atc->m_pp_context.append_source( m_filename.c_str() ).failed_status( status ) )
		return status;

	status = atc->m_pp_context.process();
	if( STATUS_ERROR_CANNOT_OPEN == status )
		error( status, "cannot open '%s'", m_filename.data() );

	++ m_files_processed;

	// Exit
	__unreferenced_parameter( param );
	return STATUS_SUCCESS;
}
const status_t f1::pp::frontend::context::pp_buffer_task::run( void * param, mt::Ithread_context * tc )
{
	status_t status;

	// Get context
	core::Tref<thread_context> atc = type_cast<thread_context *>(tc);
	__debugbreak_if( nullptr == atc )
		return STATUS_ERROR_INVALID_STATE;
	if( !m_syntax_guid.is_null() && atc->m_pp_context.set_syntax( m_syntax_guid ).failed_status( status ) )
		return status;

	// Start assembler
	core::Tref<parser::Isource> src;
	if( atc->m_pp_context.get_parser_context().create_memory_source( src, m_buffer, false, m_filename.c_str() ).failed_status( status ) )
		return status;
	if( atc->m_pp_context.append_source( *src ).failed_status( status ) )
		return status;

	status = atc->m_pp_context.process();
	if( STATUS_ERROR_CANNOT_OPEN == status )
		error( status, "cannot open '%s'", m_filename.data() );

	++ m_files_processed;

	// Exit
	__unreferenced_parameter( param );
	return STATUS_SUCCESS;
}
const status_t f1::pp::frontend::context::pp_mem_task::run( void * param, mt::Ithread_context * tc )
{
	status_t status;

	// Get context
	core::Tref<thread_context> atc = type_cast<thread_context *>(tc);
	__debugbreak_if( nullptr == atc )
		return STATUS_ERROR_INVALID_STATE;
	if( !m_syntax_guid.is_null() && atc->m_pp_context.set_syntax( m_syntax_guid ).failed_status( status ) )
		return status;

	// Start assembler
	core::Tref<parser::Isource> src;
	if( atc->m_pp_context.get_parser_context().create_memory_source( src, m_ptr, m_size, false, m_filename.c_str() ).failed_status( status ) )
		return status;
	if( atc->m_pp_context.append_source( *src ).failed_status( status ) )
		return status;

	status = atc->m_pp_context.process();
	if( STATUS_ERROR_CANNOT_OPEN == status )
		error( status, "cannot open '%s'", m_filename.data() );

	++ m_files_processed;

	// Exit
	__unreferenced_parameter( param );
	return STATUS_SUCCESS;
}

/*END OF context.cxx*/
