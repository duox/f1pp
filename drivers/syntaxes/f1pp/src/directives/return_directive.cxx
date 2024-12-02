/*return_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::return_directive::return_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::return_directive::get_name() const
{
	return "return";
}
// {2533699C-19D2-47f2-8DFA-C026845FE9EC}
const f1::guid_t f1::f1pp::return_directive::m_guid =
{ 0x2533699c, 0x19d2, 0x47f2, { 0x8d, 0xfa, 0xc0, 0x26, 0x84, 0x5f, 0xe9, 0xec } };
const f1::guid_t & f1::f1pp::return_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::return_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

// Operations
const status_t f1::f1pp::return_directive::parse( pp::Iparser & parser )
{
	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Skip spaces in the fron of the expression
	f1par->skip_real_spaces_and_comments();

	// Go to expanding macros
	f1par->modify_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode );
	f1par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::return_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> f1par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_INVALID_TYPE;

	// Parse buffer
	if( 0 == out_buf.get_data_size() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_EXPRESSION );
		return STATUS_SUCCESS;
	}

	// Parse expression
	// Using direct parsing from buffer since the string is read and glued, all macros are substituted
	const char *	 start = (const char *) out_buf.get_ptr();
	const char * const end = (const char *) out_buf.get_end_ptr();
	pp::variant result;
	if( f1par->parse_expression( result, start, end, 0 ).failed_status( status ) )
		return status;

	// Check for unprocessed characters after the directive
	f1par->skip_real_spaces_and_comments( start, end );
	if( start < end )
		parser.message( c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "return" );

	// Find execution context of required type
	function_frame * frm = (function_frame *) parser.get_directive_frame();
	if( nullptr == frm )
		return parser.message( c_pp::syntax::IMSG_ERROR_UNEXPECTED_S, "#return" );
	frm->m_returned_value.move( result );
	parser.get_parser()->abort_patch();

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::return_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
	parser.modify_parser_flags( pp::Iparser::parser_flags_t::null, pp::Iparser::parser_flags_t::fl_expression_mode );
#ifdef _DEBUG
	core::Tref<f1::f1pp::return_directive> drc = type_cast<f1::f1pp::return_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::f1pp::return_directive *>( param )->execute( parser, buf );
#endif
}

/*END OF return_directive.cxx*/
