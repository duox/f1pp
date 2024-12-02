/*ifndef_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::ifndef_directive::ifndef_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

unsigned f1::f1pp::ifndef_directive::get_caps( unsigned mask ) const __noexcept
{
	return ((cap_c_extension | cap_override) & mask) | super::get_caps( mask );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::ifndef_directive::parse( pp::Iparser & parser )
{
	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch macro expression
	bool res = false;
	status_t status = f1pp_par->parse_macro_expression( res );

	// Begin directive even if there were some errors
	parser.enter_directive( *new(std::nothrow) c_pp::c99::if_directive::frame( *f1pp_par, !res ) );

	// Set abort flag if there was a problem
	if( status.failed() )
	{
		c_pp::c99::if_directive::frame * frm = type_cast<c_pp::c99::if_directive::frame *>( parser.get_directive_frame() );
		__debugbreak_ifnot( nullptr != frm )
			frm->modify_flags( pp::Iframe::fl_abort_statement );
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF ifndef_directive.cxx*/
