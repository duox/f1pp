/*exit_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::exit_directive::exit_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::exit_directive::get_name() const
{
	return "exit";
}

// {43807442-6A94-47B7-AC25-3594523780F7}
const f1::guid_t f1::f1pp::exit_directive::m_guid =
{ 0x43807442, 0x6a94, 0x47b7, { 0xac, 0x25, 0x35, 0x94, 0x52, 0x37, 0x80, 0xf7 } };
const f1::guid_t & f1::f1pp::exit_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::exit_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

// Operations
// TODO: #exit exit-code
const status_t f1::f1pp::exit_directive::parse( pp::Iparser & parser )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Get current source
	core::Tconst_ref<f1::parser::Isource> src = par->get_current_source();
	if( nullptr != src->get_parent() )
		par->leave_source();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF exit_directive.cxx*/
