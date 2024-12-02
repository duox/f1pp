/*buffer_storage.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::buffer_storage_class::buffer_storage_class( const core::Iidentifier * ident ) __noexcept:
	super( ident )
{
}

// {77E3F059-6991-4d32-BFFD-13B1569A51FB}
const f1::guid_t f1::pp::buffer_storage_class::m_guid = 
{ 0x77e3f059, 0x6991, 0x4d32, { 0xbf, 0xfd, 0x13, 0xb1, 0x56, 0x9a, 0x51, 0xfb } };
const f1::guid_t & f1::pp::buffer_storage_class::get_guid() const __noexcept
{
	return m_guid;
}
const char * f1::pp::buffer_storage_class::get_name() const noexcept
{
	return "buffer";
}

const status_t f1::pp::buffer_storage_class::new_storage( core::Tref<pp::Istorage> & res, const core::Iidentifier * ident ) __noexcept
{
	return res.createT<buffer_storage>( ident );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

f1::pp::buffer_storage::buffer_storage( const core::Iidentifier * ident ) __noexcept:
	super( ident )/*,
	m_pos( io::abs_offset_t(-1) )*/
{
}

const status_t f1::pp::buffer_storage::initialize( const pp::context & context, storage_flags_t storage_flags )
{
	status_t status;

	if( m_buffer.initialize().failed_status( status ) )
		return status;

	if( super::initialize( context, storage_flags ).failed_status( status ) )
		return status;

	return STATUS_SUCCESS;
}

const status_t f1::pp::buffer_storage::move( core::object * obj ) noexcept
{
	return move( type_cast<self *>( obj ) );
}
const status_t f1::pp::buffer_storage::move( self * obj )
{
	status_t status;

	if( super::move( obj ).failed_status( status ) )
		return status;

	if( m_buffer.move( &obj->m_buffer ).failed_status( status ) )
		return status;
//	m_pos = obj->m_pos;

	return STATUS_SUCCESS;
}

const f1::core::buffer & f1::pp::buffer_storage::get_buffer() const noexcept
{
	return m_buffer;
}
f1::core::buffer & f1::pp::buffer_storage::get_buffer() noexcept
{
	return m_buffer;
}

const status_t f1::pp::buffer_storage::reset()
{
	return m_buffer.set_data_size( 0 );
}
const status_t f1::pp::buffer_storage::on_write( const pp::string & str ) __noexcept
{
	__assert( str.length() < 16384 );

//	if( m_pos == io::abs_offset_t(-1) )
//		return m_buffer.insert( size_t(m_pos), string, len );
	return m_buffer.append( str.data(), str.length() );
}
/*const status_t f1::pp::buffer_storage::set_pos( f1::io::abs_offset_t pos )
{
	__debugbreak_if( pos >= m_buffer.get_data_size() )
		return STATUS_ERROR_INVALID_PARAMETER;

	m_pos = pos;

	return STATUS_SUCCESS;
}
f1::io::abs_offset_t f1::pp::buffer_storage::get_pos() const
{
	return m_pos;
}*/

/*END OF buffer_storage.cxx*/
