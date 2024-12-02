/*string_storage.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::string_storage_class::string_storage_class( const core::Iidentifier * ident ) noexcept:
	super( ident )
{
}

// {6DF01777-0775-4B6A-98B9-220CF031A287}
const f1::guid_t f1::pp::string_storage_class::m_guid = 
{ 0x6df01777, 0x0775, 0x4b6a, { 0x98, 0xb9, 0x22, 0x0c, 0xf0, 0x31, 0xa2, 0x87 } };
const f1::guid_t & f1::pp::string_storage_class::get_guid() const noexcept
{
	return m_guid;
}
const char * f1::pp::string_storage_class::get_name() const noexcept
{
	return "string";
}

const status_t f1::pp::string_storage_class::new_storage( core::Tref<pp::Istorage> & res, const core::Iidentifier * ident ) noexcept
{
	return res.createT<string_storage>( ident );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

f1::pp::string_storage::string_storage( const core::Iidentifier * ident ) noexcept:
	super( ident )/*,
	m_pos( io::abs_offset_t(-1) )*/
{
}

const status_t f1::pp::string_storage::initialize( const pp::context & context, storage_flags_t storage_flags )
{
	status_t status;

	if( super::initialize( context, storage_flags ).failed_status( status ) )
		return status;

	return STATUS_SUCCESS;
}

const status_t f1::pp::string_storage::move( core::object * obj ) noexcept
{
	return move( type_cast<self *>( obj ) );
}
const status_t f1::pp::string_storage::move( self * obj )
{
	status_t status;

	if( super::move( obj ).failed_status( status ) )
		return status;

	m_string.move( obj->m_string );
//	m_pos = obj->m_pos;

	return STATUS_SUCCESS;
}

const f1::pp::string & f1::pp::string_storage::get_string() const
{
	return m_string;
}
f1::pp::string & f1::pp::string_storage::get_string()
{
	return m_string;
}

const status_t f1::pp::string_storage::reset()
{
	return m_string.clear();
}
const status_t f1::pp::string_storage::on_write( const pp::string & str ) noexcept
{
	__assert( str.length() < 100 );

//	if( m_pos != io::abs_offset_t(-1) )
//		return m_string.insert( size_t(m_pos), string, len );
	return m_string.add_part( str );
}
/*const status_t f1::pp::string_storage::set_pos( f1::io::abs_offset_t pos ) noexcept
{
	__debugbreak_if( pos >= m_string.size() )
		return STATUS_ERROR_OUT_OF_RANGE;
	m_pos = pos;

	return STATUS_SUCCESS;
}
f1::io::abs_offset_t f1::pp::string_storage::get_pos() const noexcept
{
	return m_pos;
}*/

/*END OF string_storage.cxx*/
