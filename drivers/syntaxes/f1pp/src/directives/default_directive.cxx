/*default_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::default_directive::default_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::default_directive::get_name() const
{
	return "default";
}
// {CA3A7A50-2572-4229-862A-5B36816BDE2F}
const f1::guid_t f1::f1pp::default_directive::m_guid =
{ 0xca3a7a50, 0x2572, 0x4229, { 0x86, 0x2a, 0x5b, 0x36, 0x81, 0x6b, 0xde, 0x2f } };
const f1::guid_t & f1::f1pp::default_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::default_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::default_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	switch_directive::frame * frm = type_cast<switch_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#default" );

	// Parse colon
	c_pp_par->skip_real_spaces_and_comments();
	if( c_pp_par->get_parser()->skip_char( ':' ) )	// skip optional ':'
		c_pp_par->skip_real_spaces_and_comments();

	if( !c_pp_par->get_parser()->is_nl() )
		parser.message( c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "default" );

	// Process #default directive logics
#if 1
	if( 0 == frm->m_endswitch_position.m_line )
	{	// first execution of #default (before #endswitch is reached)
		if( 0 != frm->m_default_position.m_line )
		{
			parser.message( syntax::IMSG_WARNING_ALREADY_USED_S, "#default" );
			parser.message( frm->m_default_position.m_source->get_filename(), frm->m_default_position.m_line, syntax::IMSG_INFO_USED_HERE );
		}
		parser.get_parser()->get_pos( frm->m_default_position );

		if( 0 == frm->m_fallthrough_position.m_line )
		{	// there wasn't a #fallthrough directive, so disable processing
			frm->allow_exec( false );	// continue searching for the case labels
		}
		else
		{	// there was a #fallthrough, so do not change execution status
			frm->m_fallthrough_position.reset();	// reset #fallthrough status
		}
	}
	else
	{	// second execution of #default (after #endswitch is reached)
		frm->m_default_position.reset();
	}
#else
	if( 0 == frm->m_case_position.m_line )
	{
		c_pp_par->get_parser()->get_pos( frm->m_case_position );
		c_pp_par->allow_exec( true );
	}
#endif

	// Exit
	return STATUS_SUCCESS;
}

/*END OF default_directive.cxx*/
