/*elifdef_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::elifdef_directive::elifdef_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

unsigned f1::f1pp::elifdef_directive::get_caps( unsigned mask ) const __noexcept
{
	return ((cap_c_extension | cap_override) & mask) | super::get_caps( mask );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::elifdef_directive::parse( pp::Iparser & parser )
{
	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	c_pp::c99::if_directive::frame * frm = type_cast<c_pp::c99::if_directive::frame *>( parser.get_directive_frame() );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INVALID_STATE;

	// Fetch macro expression
	bool res = false;
	status_t status = f1pp_par->parse_macro_expression( res );
	if( status.failed() )
	{
		parser.message( syntax::IMSG_ERROR_EXPECTED_MACRO_EXPRESSION );
		res = false;
	}

	// Update context
	frm->m_allow_parse = res;
	frm->allow_exec( res );

	// Set abort flag if there was a problem
	if( status.failed() )
		frm->modify_flags( pp::Iframe::fl_abort_statement );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF elifdef_directive.cxx*/
