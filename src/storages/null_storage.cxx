/*null_storage.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::null_storage_class::null_storage_class( const core::Iidentifier * ident ) noexcept:
	super( ident )
{
}

// {F7F7ACA0-96BA-4bf1-B9C0-3726A7829B8B}
const f1::guid_t f1::pp::null_storage_class::m_guid = 
{ 0xf7f7aca0, 0x96ba, 0x4bf1, { 0xb9, 0xc0, 0x37, 0x26, 0xa7, 0x82, 0x9b, 0x8b } };
const f1::guid_t & f1::pp::null_storage_class::get_guid() const noexcept
{
	return m_guid;
}
const char * f1::pp::null_storage_class::get_name() const noexcept
{
	return "null";
}

const status_t f1::pp::null_storage_class::new_storage( core::Tref<pp::Istorage> & res, const core::Iidentifier * ident ) noexcept
{
	return res.createT<null_storage>( ident );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

f1::pp::null_storage::null_storage( const core::Iidentifier * ident ) noexcept:
	super( ident )
{
}

const status_t f1::pp::null_storage::initialize( const pp::context & context, storage_flags_t flags )
{
	return super::initialize( context, flags );
}

const status_t f1::pp::null_storage::on_write( const pp::string & str ) noexcept
{
	__unreferenced_parameter( str );
	return STATUS_SUCCESS;
}
/*const status_t f1::pp::null_storage::set_pos( f1::io::abs_offset_t pos )
{
	__unreferenced_parameter( pos );
	return STATUS_SUCCESS;
}
f1::io::abs_offset_t f1::pp::null_storage::get_pos() const
{
	return 0;
}*/

/*END OF null_storage.cxx*/
