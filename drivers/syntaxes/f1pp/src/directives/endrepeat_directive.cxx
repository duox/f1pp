/*endrepeat_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endrepeat_directive::endrepeat_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endrepeat_directive::get_name() const
{
	return "endrepeat";
}
// {599C2437-860A-496a-93D4-34BA5BAC931C}
const f1::guid_t f1::f1pp::endrepeat_directive::m_guid =
{ 0x599c2437, 0x860a, 0x496a, { 0x93, 0xd4, 0x34, 0xba, 0x5b, 0xac, 0x93, 0x1c } };
const f1::guid_t & f1::f1pp::endrepeat_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::endrepeat_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::endrepeat_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	f1::pp::Iframe * i_ctx = parser.get_directive_frame();
	if( nullptr == i_ctx )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endrepeat" );

	repeat_directive::frame * frm = type_cast<repeat_directive::frame *>( i_ctx );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endrepeat" );
	if( frm->get_closing_directive_guid() != endrepeat_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endrepeat" );

	frm->check_different_sources();	// warn if sources differ

	// Check for disabled statement (fast path)
	if( frm->get_flags( pp::Iframe::fl_initially_disabled | pp::Iframe::fl_abort_statement ) )
	{
		f1pp_par->skip_to_newline();
		parser.leave_directive();	// complete directive
		return STATUS_SUCCESS;
	}

	// Check the counter
	if( !f1pp_par->check_limit_value_min( pp::limit_t::li_maximum_iteration_count, frm->m_iteration_count + 1 ) )
	{
		f1pp_par->message( syntax::IMSG_FATALERROR_TOO_MANY_ITERATIONS );
	}
	else if( 0 == frm->m_repeat_count )
	{
		if( nullptr != frm->m_control_macro )
			frm->m_control_macro->set_value_variant( pp::variant( intmax_t(frm->m_repeat_count) ) );

//		f1pp_par->allow_exec( true );
	}
	else if( 1 < frm->m_repeat_count )
	{
		// Move to the part to execute
		parser.get_parser()->set_pos( frm->m_position );
		parser.get_parser()->set_location( frm->get_initial_location() );
		-- frm->m_repeat_count;
		++ frm->m_iteration_count;
		frm->allow_exec( true );

		// Update counter macro
		if( nullptr != frm->m_control_macro )
			frm->m_control_macro->set_value_variant( pp::variant( frm->m_backward ? intmax_t(frm->m_repeat_count) : intmax_t(frm->m_iteration_count) ) );

		// Exit
		frm->undefine_locals();
		return STATUS_SUCCESS;
	}

	// Pop the last nesting level
	parser.leave_directive();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endrepeat_directive.cxx*/
