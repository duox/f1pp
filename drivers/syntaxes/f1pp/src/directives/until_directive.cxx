/*until_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::until_directive::until_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::until_directive::get_name() const
{
	return "until";
}
// {CB3FCB2F-96E4-4af9-98A6-424EBBBC67D5}
const f1::guid_t f1::f1pp::until_directive::m_guid =
{ 0xcb3fcb2f, 0x96e4, 0x4af9, { 0x98, 0xa6, 0x42, 0x4e, 0xbb, 0xbc, 0x67, 0xd5 } };
const f1::guid_t & f1::f1pp::until_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::until_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_hard_edge | cap_invariant | cap_keep_text_eol | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::until_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> cpp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == cpp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Check for #do...#until statement
	do_directive::frame * do_frm = type_cast<do_directive::frame *>( cpp_par->get_directive_frame() );
	if( nullptr == do_frm )
		return cpp_par->message( syntax::IMSG_ERROR_UNEXPECTED_S, "#until" );
	if( do_frm->get_closing_directive_guid() != until_directive::m_guid )
		return cpp_par->message( syntax::IMSG_ERROR_UNEXPECTED_S, "#until" );

	// Fetch expression string and expand macros
	cpp_par->modify_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode );
	cpp_par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::until_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> cpp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == cpp_par )
		return STATUS_ERROR_INVALID_TYPE;

	// Get current execution context
	do_directive::frame * do_frm = type_cast<do_directive::frame *>( cpp_par->get_directive_frame() );
	__debugbreak_if( nullptr == do_frm )
		return STATUS_ERROR_INTERNAL;

	// Check limit
	if( !parser.check_limit_value_min( pp::limit_t::li_maximum_iteration_count, do_frm->m_iteration_count ) )
	{
		parser.leave_directive();
		return parser.message( syntax::IMSG_FATALERROR_TOO_MANY_ITERATIONS );
	}
	++ do_frm->m_iteration_count;

	// Parse buffer
	if( 0 == out_buf.get_data_size() )
	{
		parser.leave_directive();
		return parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_EXPRESSION );
	}

	// Parse expression
	// Using direct parsing from buffer since the string is read and glued, all macros are substituted
	const char *	 start = (const char *) out_buf.get_ptr();
	const char * const end = (const char *) out_buf.get_end_ptr();
	pp::variant result;
	status = cpp_par->parse_expression( result, start, end, 0 );
	UNREFERENCED_VALUE(status);	// ignore invalid status in expression

	// Check for unexpected characters after the statement
	cpp_par->skip_real_spaces_and_comments( start, end );
	if( start < end )
		parser.message( c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "until" );

	// Process result
	if( !result.can_cast_to( pp::variant::ty_bool ) )
		return cpp_par->message( syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );

	if( !result.as_bool() )
	{
		// Set current position of the beginning of the expression
		cpp_par->get_parser()->set_pos( do_frm->m_position );
		cpp_par->get_parser()->set_location( do_frm->get_initial_location() );
		do_frm->undefine_locals();
	}
	else
	{
		if( nullptr != do_frm->m_control_macro )
			do_frm->m_control_macro->set_value_variant( pp::variant( true ) );
		parser.leave_directive();
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::until_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
	parser.modify_parser_flags( pp::Iparser::parser_flags_t::null, pp::Iparser::parser_flags_t::fl_expression_mode );
#ifdef _DEBUG
	core::Tref<f1::f1pp::until_directive> drc = type_cast<f1::f1pp::until_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::f1pp::until_directive *>( param )->execute( parser, buf );
#endif
}

/*END OF until_directive.cxx*/
