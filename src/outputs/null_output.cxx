/*null_output.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::null_output_class::null_output_class( const core::Iidentifier * ident ) __noexcept:
	super( ident )
{
}

// {34FCDE03-8307-4410-B3FF-9A75A892AEB2}
const f1::guid_t f1::pp::null_output_class::m_guid = 
{ 0x34fcde03, 0x8307, 0x4410, { 0xb3, 0xff, 0x9a, 0x75, 0xa8, 0x92, 0xae, 0xb2 } };
const f1::guid_t & f1::pp::null_output_class::get_guid() const __noexcept
{
	return m_guid;
}

const status_t f1::pp::null_output_class::new_output( core::Tref<pp::Ioutput> & res, const core::Iidentifier * ident ) __noexcept
{
	return res.createT<null_output>( ident );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

f1::pp::null_output::null_output( const core::Iidentifier * ident ) __noexcept:
	super( ident )
{
}

const status_t f1::pp::null_output::begin_output() noexcept
{
	return STATUS_SUCCESS;
}
const status_t f1::pp::null_output::end_output() noexcept
{
	return STATUS_SUCCESS;
}

const status_t f1::pp::null_output::write( const char * string, size_t len ) __noexcept
{
	__unreferenced_parameter( string );
	__unreferenced_parameter( len );
	return STATUS_SUCCESS;
}

/*END OF null_output.cxx*/
