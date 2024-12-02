/*fallthrough_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::fallthrough_directive::fallthrough_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::fallthrough_directive::get_name() const
{
	return "fallthrough";
}

// {6CE3D127-92DD-491e-A70C-926D3C956851}
const f1::guid_t f1::f1pp::fallthrough_directive::m_guid =
{ 0x6ce3d127, 0x92dd, 0x491e, { 0xa7, 0xc, 0x92, 0x6d, 0x3c, 0x95, 0x68, 0x51 } };
const f1::guid_t & f1::f1pp::fallthrough_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::fallthrough_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::fallthrough_directive::parse( pp::Iparser & parser )
{
	// Check current state
	switch_directive::frame * frm = type_cast<switch_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#fallthrough" );

	// Set fallthrough flag
	if( 0 != frm->m_fallthrough_position.m_line )
	{
		parser.message( syntax::IMSG_WARNING_ALREADY_USED_S, "#fallthrough" );
		parser.message( frm->m_fallthrough_position.m_source->get_filename(), frm->m_fallthrough_position.m_line, syntax::IMSG_INFO_USED_HERE );
	}
	if( 0 != frm->m_case_position.m_line || ( 0 != frm->m_endswitch_position.m_line && 0 != frm->m_default_position.m_line ) )
		parser.get_parser()->get_pos( frm->m_fallthrough_position );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF fallthrough_directive.cxx*/
