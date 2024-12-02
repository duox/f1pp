/*endexec_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endexec_directive::endexec_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endexec_directive::get_name() const
{
	return "endexec";
}
// {8BE21ED3-EB59-49FE-AB5D-C103614A6500}
const f1::guid_t f1::f1pp::endexec_directive::m_guid =
{ 0x8be21ed3, 0xeb59, 0x49fe, { 0xab, 0x5d, 0xc1, 0x03, 0x61, 0x4a, 0x65, 0x00 } };
const f1::guid_t & f1::f1pp::endexec_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::endexec_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_c_extension | cap_keep_text_eol) & mask;
}

// Operations
const status_t f1::f1pp::endexec_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	exec_directive::frame * frm = type_cast<exec_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endexec" );
	if( frm->get_closing_directive_guid() != endexec_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endexec" );

	frm->check_different_sources();	// warn if sources differ

	c_pp_par->skip_real_spaces_and_comments();
	if( !parser.get_parser()->is_nl() )
	{
		parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );
		c_pp_par->skip_to_newline();
	}

	frm->m_exec_directive->execute( parser, frm->m_storage.get_string() );

	// Close the directive
	c_pp_par->modify_c_flags( frm->m_old_c_flags, ~frm->m_old_c_flags );
	parser.set_output( frm->m_old_output );
	parser.set_storage( frm->m_old_storage );
	parser.leave_directive();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endexec_directive.cxx*/
