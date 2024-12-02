/*endnamespace_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endnamespace_directive::endnamespace_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endnamespace_directive::get_name() const
{
	return "endnamespace";
}
// {4452E09F-907D-4964-9CC4-32055A0E0363}
const f1::guid_t f1::f1pp::endnamespace_directive::m_guid =
{ 0x4452e09f, 0x907d, 0x4964, { 0x9c, 0xc4, 0x32, 0x5, 0x5a, 0xe, 0x3, 0x63 } };
const f1::guid_t & f1::f1pp::endnamespace_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::endnamespace_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::endnamespace_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	namespace_directive::frame * frm = type_cast<namespace_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endnamespace" );
	if( frm->get_closing_directive_guid() != endnamespace_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endnamespace" );

	frm->check_different_sources();	// warn if sources differ

	// Close namespace
	parser.get_ns_context().set_current_namespace( frm->m_parent_namespace );

	// Pop the last nesting level
	parser.leave_directive();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endnamespace_directive.cxx*/
