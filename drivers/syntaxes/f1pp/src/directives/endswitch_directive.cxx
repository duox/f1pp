/*endswitch_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endswitch_directive::endswitch_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endswitch_directive::get_name() const
{
	return "endswitch";
}
// {AB155B6A-5F86-43cd-8C14-2E12D8F7EB53}
const f1::guid_t f1::f1pp::endswitch_directive::m_guid =
{ 0xab155b6a, 0x5f86, 0x43cd, { 0x8c, 0x14, 0x2e, 0x12, 0xd8, 0xf7, 0xeb, 0x53 } };
const f1::guid_t & f1::f1pp::endswitch_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::endswitch_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::endswitch_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	switch_directive::frame * frm = type_cast<switch_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, get_name() );
	if( frm->get_closing_directive_guid() != get_guid() )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, get_name() );
	frm->check_different_sources();

	// Check the status of the #switch directive
	if( 0 == frm->m_endswitch_position.m_line )
	{	// this is the first time we encountered #endswitch

		if( 0 != frm->m_case_position.m_line )
		{	// a #case was hit, finish #switch
//			if( 0 != frm->m_fallthrough_position.m_line )
//				parser.message( frm->m_fallthrough_position, syntax::IMSG_WARNING_UNUSED_FALLTHROUGH );
			goto done;
		}
		else if( 0 != frm->m_default_position.m_line )
		{	// no #case was hit, but #default statement was used; move to #default
			frm->allow_exec( true );
			parser.get_parser()->get_pos( frm->m_endswitch_position );
			parser.get_parser()->set_pos( frm->m_default_position );
		}
		else
		{	// no #case was hit, no #default directive; do nothing but leave the directive
			goto done;
		}
	}
	else
	{	// we has already visited this #endswitch, returned back from #default and a chain
		// of #case's via #fallthrough directives.
	done:
//		if( 0 != frm->m_fallthrough_position.m_line )
//			parser.message( frm->m_fallthrough_position, syntax::IMSG_WARNING_UNUSED_FALLTHROUGH );
		parser.leave_directive();	// just pop the last nesting level
		frm = nullptr;
	}
	if( nullptr != frm )
	{
		frm->allow_exec( true );
		frm->m_fallthrough_position.reset();	// reset #fallthrough status
	}
//	else
//		c_pp_par->set_allow_exec( true );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endswitch_directive.cxx*/
