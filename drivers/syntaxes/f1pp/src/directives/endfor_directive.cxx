/*endfor_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endfor_directive::endfor_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endfor_directive::get_name() const
{
	return "endfor";
}
// {D336F2B7-4322-4360-9359-4D599C91FD90}
const f1::guid_t f1::f1pp::endfor_directive::m_guid =
{ 0xd336f2b7, 0x4322, 0x4360, { 0x93, 0x59, 0x4d, 0x59, 0x9c, 0x91, 0xfd, 0x90 } };
const f1::guid_t & f1::f1pp::endfor_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::endfor_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_keep_text_eol | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::endfor_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_INVALID_TYPE;
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	for_directive::frame * frm = type_cast<for_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endfor" );
	if( frm->get_closing_directive_guid() != endfor_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endfor" );

	frm->check_different_sources();	// warn if sources differ

	// Check for disabled statement (fast path)
	if( frm->get_flags( pp::Iframe::fl_initially_disabled | pp::Iframe::fl_abort_statement ) || nullptr == frm->m_step_start )
	{
		f1par->skip_to_newline();
		parser.leave_directive();	// complete directive
		return STATUS_SUCCESS;
	}

	// Check the position
	frm->undefine_locals();
	frm->m_end_position = f1par->get_directive_start();	// store #endfor position (for fast exit from the #for parser)
	//par->get_pos( frm->m_end_position );	// store #endfor position (for fast exit from the #for parser)
	par->set_ptr( frm->m_step_start );		// set pointer to the set expression
	par->set_location( frm->get_initial_location() );
	if( 0 == frm->m_step_length )
	{
		// Step expression is empty; call the callback directly
		frm->m_cond_breakpoint_callback.on_breakpoint_reached();
	}
	else
	{
		// Execute step expression
		frm->m_error_count = f1par->get_common_error_count();
		f1par->get_assignment_directive().set_finish_callback( &frm->m_cond_breakpoint_callback );
		status = f1par->get_assignment_directive().parse( parser );
		UNREFERENCED_VALUE(status);	// ignore invalid status in expression
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endfor_directive.cxx*/
