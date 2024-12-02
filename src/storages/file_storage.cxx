/*file_storage.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::file_storage_class::file_storage_class( const core::Iidentifier * ident ) __noexcept:
	super( ident )
{
}

// {2B83A87F-682A-4214-8099-93D782D23148}
const f1::guid_t f1::pp::file_storage_class::m_guid = 
{ 0x2b83a87f, 0x682a, 0x4214, { 0x80, 0x99, 0x93, 0xd7, 0x82, 0xd2, 0x31, 0x48 } };
const f1::guid_t & f1::pp::file_storage_class::get_guid() const __noexcept
{
	return m_guid;
}
const char * f1::pp::file_storage_class::get_name() const noexcept
{
	return "file";
}

const status_t f1::pp::file_storage_class::new_storage( core::Tref<pp::Istorage> & res, const core::Iidentifier * ident ) __noexcept
{
	return res.createT<file_storage>( ident );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

f1::pp::file_storage::file_storage( const core::Iidentifier * ident ) __noexcept:
	super( ident )
{
}

const status_t f1::pp::file_storage::initialize( const pp::context & context, const core::string & filename, storage_flags_t storage_flags )
{
	status_t status;

	core::Tref<io::file_ostream> ostr = nullptr;
	if( ostr.createT<io::file_ostream>( filename ).failed_status( status ) )
		return status;
	if( ostr->bad() )
		return STATUS_ERROR_CANNOT_OPEN;

	if( super::initialize( context, *ostr, storage_flags ).failed_status( status ) )
		return status;
	m_output_filename = filename;

	return STATUS_SUCCESS;
}

/*END OF file_storage.cxx*/
