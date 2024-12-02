/*endraw_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endraw_directive::endraw_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endraw_directive::get_name() const
{
	return "endraw";
}
// {70918618-0AA7-4432-93ED-4CE8BE672E5C}
const f1::guid_t f1::f1pp::endraw_directive::m_guid =
{ 0x70918618, 0xaa7, 0x4432, { 0x93, 0xed, 0x4c, 0xe8, 0xbe, 0x67, 0x2e, 0x5c } };
const f1::guid_t & f1::f1pp::endraw_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::endraw_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::endraw_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	raw_directive::frame * frm = type_cast<raw_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endraw" );
	if( frm->get_closing_directive_guid() != endraw_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endraw" );

	frm->check_different_sources();	// warn if sources differ

	// Close the directive
	c_pp_par->modify_c_flags( frm->m_old_c_flags, ~frm->m_old_c_flags );
	parser.leave_directive();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endraw_directive.cxx*/
