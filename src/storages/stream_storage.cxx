/*stream_storage.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::stream_storage_class::stream_storage_class( const core::Iidentifier * ident ) __noexcept:
	super( ident )
{
}

// {62A054D2-A6DC-4964-8D05-5CAA73A48AEA}
const f1::guid_t f1::pp::stream_storage_class::m_guid = 
{ 0x62a054d2, 0xa6dc, 0x4964, { 0x8d, 0x5, 0x5c, 0xaa, 0x73, 0xa4, 0x8a, 0xea } };
const f1::guid_t & f1::pp::stream_storage_class::get_guid() const __noexcept
{
	return m_guid;
}
const char * f1::pp::stream_storage_class::get_name() const noexcept
{
	return "stream";
}

const status_t f1::pp::stream_storage_class::new_storage( core::Tref<pp::Istorage> & res, const core::Iidentifier * ident ) __noexcept
{
	return res.createT<stream_storage>( ident );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

f1::pp::stream_storage::stream_storage( const core::Iidentifier * ident ) __noexcept:
	super( ident )/*,
	m_pos( io::abs_offset_t(-1) )*/
{
}

const status_t f1::pp::stream_storage::initialize( const pp::context & context, storage_flags_t storage_flags )
{
	status_t status;

	if( super::initialize( context, storage_flags ).failed_status( status ) )
		return status;

	return STATUS_SUCCESS;
}
const status_t f1::pp::stream_storage::initialize( const pp::context & context, const core::string & output_filename, storage_flags_t storage_flags )
{
	status_t status;

	if( super::initialize( context, storage_flags ).failed_status( status ) )
		return status;

	if( m_stream.createT<io::file_ostream>( std::cref(output_filename) ).failed_status( status ) )
		return status;

	return STATUS_SUCCESS;
}
const status_t f1::pp::stream_storage::initialize( const pp::context & context, io::Iostream & str, storage_flags_t storage_flags )
{
	status_t status;

	if( super::initialize( context, storage_flags ).failed_status( status ) )
		return status;
	m_stream = &str;

	return STATUS_SUCCESS;
}

const status_t f1::pp::stream_storage::move( core::object * obj ) noexcept
{
	return move( type_cast<self *>( obj ) );
}
const status_t f1::pp::stream_storage::move( self * obj )
{
	status_t status;

	if( super::move( obj ).failed_status( status ) )
		return status;

	m_stream.move( obj->m_stream );
//	m_pos = obj->m_pos;

	return STATUS_SUCCESS;
}

const f1::io::Iostream * f1::pp::stream_storage::get_stream() const
{
	return m_stream;
}
f1::io::Iostream * f1::pp::stream_storage::get_stream()
{
	return m_stream;
}
const status_t f1::pp::stream_storage::set_stream( f1::io::Iostream & str )
{
	m_stream = &str;
	return STATUS_SUCCESS;
}

const status_t f1::pp::stream_storage::on_write( const pp::string & str ) __noexcept
{
	__assert( str.length() < 500 );

//	if( m_pos == io::abs_offset_t(-1) )
//		return m_stream.insert( size_t(m_pos), string, len );
	return m_stream->write( str.data(), str.length() );
}
/*const status_t f1::pp::stream_storage::set_pos( f1::io::abs_offset_t pos )
{
	__debugbreak_if( pos >= m_stream.get_data_size() )
		return STATUS_ERROR_INVALID_PARAMETER;

	m_pos = pos;

	return STATUS_SUCCESS;
}
f1::io::abs_offset_t f1::pp::stream_storage::get_pos() const
{
	return m_pos;
}*/

/*END OF stream_storage.cxx*/
