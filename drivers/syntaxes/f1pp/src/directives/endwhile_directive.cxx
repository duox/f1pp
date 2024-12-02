/*endwhile_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endwhile_directive::endwhile_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endwhile_directive::get_name() const
{
	return "endwhile";
}
// {544BF85B-9C2E-4ca9-816F-B61A6FC00A75}
const f1::guid_t f1::f1pp::endwhile_directive::m_guid =
{ 0x544bf85b, 0x9c2e, 0x4ca9, { 0x81, 0x6f, 0xb6, 0x1a, 0x6f, 0xc0, 0xa, 0x75 } };
const f1::guid_t & f1::f1pp::endwhile_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::endwhile_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_keep_text_eol | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::endwhile_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> cpp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == cpp_par )
		return STATUS_ERROR_INVALID_TYPE;

	while_directive::frame * frm = type_cast<while_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endwhile" );
	if( frm->get_closing_directive_guid() != endwhile_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endwhile" );

	frm->check_different_sources();	// warn if sources differ

	// Check the condition
	if( frm->get_flags( pp::Iframe::fl_initially_disabled | pp::Iframe::fl_abort_statement ) )
	{
		if( nullptr != frm->m_control_macro )
			frm->m_control_macro->set_value_variant( pp::variant( true ) );

		parser.leave_directive();		// complete directive
	}
	else
	{
		frm->m_end_position = cpp_par->get_directive_start();//cpp_par->get_parser()->get_pos( frm->m_end_position );
		frm->m_while_directive->_update_condition( *cpp_par, *frm );
		frm->undefine_locals();
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endwhile_directive.cxx*/
