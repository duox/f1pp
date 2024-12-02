/*Istorage.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::Istorage::Istorage( const core::Iidentifier * ident ) __noexcept:
	super( ident ),
	m_storage_flags( storage_flags_t::null ), m_was_nl( true )
{
}
f1::pp::Istorage::~Istorage() __noexcept
{
}

const status_t f1::pp::Istorage::initialize( const pp::context & context, storage_flags_t storage_flags )
{
	status_t status;

	if( super::initialize().failed_status( status ) )
		return status;

	m_context = &context;
	m_storage_flags = storage_flags;

	return STATUS_SUCCESS;
}
const status_t f1::pp::Istorage::initialize( const pp::context & context, const core::string & output_filename, storage_flags_t storage_flags )
{
	status_t status;

	if( super::initialize().failed_status( status ) )
		return status;

	m_context = &context;
	m_output_filename = output_filename;
	m_storage_flags = storage_flags;

	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::Istorage::move( core::object * obj ) noexcept
{
	return move( type_cast<self *>(obj) );
}
const status_t f1::pp::Istorage::move( self * obj )
{
	status_t status;

	if( super::move( obj ).failed_status( status ) )
		return status;

	m_storage_flags = obj->m_storage_flags;
	obj->m_storage_flags = storage_flags_t::null;

	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::Istorage::modify_flags( storage_flags_t flags_to_set, storage_flags_t flags_to_clear, storage_flags_t * prev_flags )
{
	if( nullptr != prev_flags )
		*prev_flags = m_storage_flags;
	m_storage_flags = (m_storage_flags | flags_to_set) & ~flags_to_clear;
	return STATUS_SUCCESS;
}
f1::pp::Istorage::storage_flags_t f1::pp::Istorage::get_flags( storage_flags_t mask ) const
{
	return m_storage_flags & mask;
}

const f1::pp::context * f1::pp::Istorage::get_context() const __noexcept
{
	return m_context;
}

bool f1::pp::Istorage::get_newline_flag() const
{
	return m_was_nl;
}
const status_t f1::pp::Istorage::set_newline_flag( bool on_newline )
{
	m_was_nl = on_newline;
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::Istorage::append_hook( Ihook & hk )
{
	status_t status = m_hook_list.append_unique( &hk );
	if( status.succeeded() )
	{
		if( hk.on_attach( *this ).failed_status( status ) )
			m_hook_list.remove( &hk );
	}
	return status;
}
const status_t f1::pp::Istorage::remove_hook( Ihook & hk )
{
	status_t status = m_hook_list.remove( &hk );
	if( status.succeeded() )
		__ignore_retval hk.on_detach();
	return status;
}

f1::pp::Istorage::Ihook::~Ihook() noexcept
{
	if( nullptr != m_storage )
		m_storage->remove_hook( *this );
}
const status_t f1::pp::Istorage::Ihook::on_attach( Istorage & stg )
{
	m_storage = &stg;
	return STATUS_SUCCESS;
}
const status_t f1::pp::Istorage::Ihook::on_detach()
{
	m_storage = nullptr;
	return STATUS_SUCCESS;
}
const status_t f1::pp::Istorage::Ihook::on_before_write( const pp::string & str )
{
	__unreferenced_parameter( str );
	return STATUS_INFORMATION_NOT_PROCESSED;
}
const status_t f1::pp::Istorage::Ihook::on_after_write( const pp::string & str )
{
	__unreferenced_parameter( str );
	return STATUS_INFORMATION_NOT_PROCESSED;
}
const status_t f1::pp::Istorage::Ihook::on_flush()
{
	return STATUS_INFORMATION_NOT_PROCESSED;
}
const status_t f1::pp::Istorage::Ihook::on_open()
{
	return STATUS_INFORMATION_NOT_PROCESSED;
}
const status_t f1::pp::Istorage::Ihook::on_close()
{
	return STATUS_INFORMATION_NOT_PROCESSED;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::Istorage::write( const char * string, size_t len )
{
	return write( pp::string( string, len ) );
}
const status_t f1::pp::Istorage::write( const pp::string & str )
{
	array_foreach( core::Tarray<Ihook *>::iterator, it, m_hook_list )
		(*it)->on_before_write( str );

	status_t status = on_write( str );

	array_foreach( core::Tarray<Ihook *>::iterator, it, m_hook_list )
		(*it)->on_after_write( str );

	return status;
}
const status_t f1::pp::Istorage::on_write( const pp::string & str )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( str );
	return STATUS_ERROR_PURE_CALL;
}

const status_t f1::pp::Istorage::flush()
{
	array_foreach( core::Tarray<Ihook *>::const_iterator, it, m_hook_list )
		(*it)->on_flush();
	return on_flush();
}
const status_t f1::pp::Istorage::on_flush()
{
	return STATUS_SUCCESS;
}

const status_t f1::pp::Istorage::open()
{
	// Call all hooks
	array_foreach( core::Tarray<Ihook *>::const_iterator, it, m_hook_list )
		(*it)->on_open();

	// Exit
	return on_open();
}
const status_t f1::pp::Istorage::on_open()
{
	// Check current state
	__debugbreak_if( nullptr == m_context )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Write BOM if needed
	if( m_context->get_output_flags( context::output_flags_t::fl_write_bom ) )
	{
		core::Tconst_ref<nls::Iencoding> enc = m_context->get_character_encoding();
		if( nullptr == enc )
			enc = m_context->get_default_output_encoding();

		if( nullptr != enc )
		{
			size_t size = 0;
			const uint8_t * data = enc->get_stream_marker( size );
			if( nullptr != data && 0 != size )
				write( (const char *) data, size );	// TODO: check for UTF-16 output encoding
		}
	}

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::pp::Istorage::close()
{
	array_foreach( core::Tarray<Ihook *>::const_iterator, it, m_hook_list )
	{
		Ihook * hk = *it;
		hk->on_close();
	}
	return on_close();
}
const status_t f1::pp::Istorage::on_close()
{
	return STATUS_SUCCESS;
}

/*const status_t f1::pp::Istorage::set_pos( f1::io::abs_offset_t pos )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( pos );
	return STATUS_ERROR_PURE_CALL;
}
f1::io::abs_offset_t f1::pp::Istorage::get_pos()
{
	DBG_FAIL_ON_PURE_CALL();
	return 0;
}*/

/*END OF Istorage.cxx*/
