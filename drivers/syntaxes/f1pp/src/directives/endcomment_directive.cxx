/*endcomment_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endcomment_directive::endcomment_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endcomment_directive::get_name() const
{
	return "endcomment";
}
// {AFBF69FF-DB45-47C1-84B8-7914A7A8D4D8}
const f1::guid_t f1::f1pp::endcomment_directive::m_guid =
{ 0xafbf69ff, 0xdb45, 0x47c1, { 0x84, 0xb8, 0x79, 0x14, 0xa7, 0xa8, 0xd4, 0xd8 } };
const f1::guid_t & f1::f1pp::endcomment_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::endcomment_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_c_extension | cap_forced) & mask;
}

// Operations
const status_t f1::f1pp::endcomment_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	comment_directive::frame * frm = type_cast<comment_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endcomment" );
	if( frm->get_closing_directive_guid() != endcomment_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endcomment" );

	frm->check_different_sources();	// warn if sources differ

	// Parse identifier, if it exists
	pp::string ident;
	c_pp_par->skip_real_spaces_and_comments();
	c_pp_par->parse_definition_identifier( ident );

	if( !ident.empty() || !frm->m_ident.empty() )
	{
		if( ident != frm->m_ident )
			return STATUS_SUCCESS;
	}
	__assert( ident.length() == frm->m_ident.length() );

	// Close the directive
	c_pp_par->modify_c_flags( frm->m_old_c_flags, ~frm->m_old_c_flags );
	parser.set_output( frm->m_old_output );
	parser.set_storage( frm->m_old_storage );
	parser.leave_directive();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endcomment_directive.cxx*/
