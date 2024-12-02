/*repeat_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::repeat_directive::repeat_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::repeat_directive::get_name() const
{
	return "repeat";
}
// {E942A495-E217-44cc-8973-27617FDFC833}
const f1::guid_t f1::f1pp::repeat_directive::m_guid =
{ 0xe942a495, 0xe217, 0x44cc, { 0x89, 0x73, 0x27, 0x61, 0x7f, 0xdf, 0xc8, 0x33 } };
const f1::guid_t & f1::f1pp::repeat_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::repeat_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_hard_edge | cap_invariant | cap_keep_text_eol) & mask;
}

// Operations
const status_t f1::f1pp::repeat_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;
	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Check whether identifier is specified
	// Macro specified in such a way belongs to the upper namespace in order to be examined
	// after the loop finishes
	f1pp_par->skip_real_spaces_and_comments();
	par->push_pos();

	core::Tref<pp::Imacro> mac = nullptr;
	pp::string ident;
	f1pp_par->parse_macro_identifier( ident );
	if( !ident.is_empty() )
	{
		f1pp_par->skip_real_spaces_and_comments();
		if( par->skip_char( ':' ) )
		{
			if( parser.find_macro( mac, ident, pp::Iparser::find_macro_flags_t::fl_macro_search_current_namespace_only ).failed() &&
				parser.create_macro( mac, ident ).failed_status( status ) )
			{
				parser.message( syntax::IMSG_ERROR_CANNOT_DEFINE_MACRO_S, ident.c_str() );
			}
			else
				mac->set_value_variant( pp::variant( intmax_t(0) ) );
		}
	}

	if( nullptr == mac )
		par->pop_pos();
	else
		par->pop_discard_pos();

	// Create execution context and enter new nested block
	frame * frm = new(std::nothrow) frame( *f1pp_par );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;
	frm->m_control_macro = mac;
	parser.enter_directive( *frm );

	// Check for disabled statement (fast path)
	if( frm->get_flags( pp::Iframe::fl_initially_disabled ) )
	{
		f1pp_par->skip_to_newline();
		return STATUS_SUCCESS;
	}

	// Fetch expression string and expand macros
	f1pp_par->modify_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode );
	f1pp_par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::repeat_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> f1pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	repeat_directive::frame * frm = type_cast<repeat_directive::frame *>( parser.get_directive_frame() );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;
	__debugbreak_if( frm->get_openning_directive_guid() != repeat_directive::m_guid )
		return STATUS_ERROR_INTERNAL;

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
	status = f1pp_par->parse_expression( result, start, end, 0 );
	UNREFERENCED_VALUE(status);	// ignore invalid status in expression

	// Process result
	f1pp_par->skip_real_spaces_and_comments( start, end );
	if( start < end )
		parser.message( c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "repeat" );
	if( result.as_int() < 0 )
		parser.message( f1pp::syntax::IMSG_ERROR_EXPECTED_POSITIVE_INTEGER );

	// Update execution context and enter new nested block
	frm->m_repeat_count = result.as_uint();
	frm->allow_exec( 0 != frm->m_repeat_count );
	parser.get_parser()->get_pos( frm->m_position );

	// Update counter macro
	if( nullptr != frm->m_control_macro )
		frm->m_control_macro->set_value_variant( pp::variant( frm->m_backward ? intmax_t(frm->m_repeat_count) : intmax_t(frm->m_iteration_count) ) );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::repeat_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
	parser.modify_parser_flags( pp::Iparser::parser_flags_t::null, pp::Iparser::parser_flags_t::fl_expression_mode );
#ifdef _DEBUG
	core::Tref<f1::f1pp::repeat_directive> drc = type_cast<f1::f1pp::repeat_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::f1pp::repeat_directive *>( param )->execute( parser, buf );
#endif
}
f1::pp::Iframe * f1::f1pp::repeat_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return nullptr;
	return new(std::nothrow) frame( *f1pp_par );
}

f1::f1pp::repeat_directive::frame::frame( f1pp::parser & par ):
	super( par ),
	m_repeat_count( 0 ), m_iteration_count( 0 ), m_backward( false )
{
}
const char * f1::f1pp::repeat_directive::frame::get_openning_directive_name() const
{
	return "repeat";
}
const char * f1::f1pp::repeat_directive::frame::get_closing_directive_name() const
{
	return "endrepeat";
}
const f1::guid_t & f1::f1pp::repeat_directive::frame::get_openning_directive_guid() const
{
	return repeat_directive::m_guid;
}
const f1::guid_t & f1::f1pp::repeat_directive::frame::get_closing_directive_guid() const
{
	return endrepeat_directive::m_guid;
}

/*END OF repeat_directive.cxx*/
