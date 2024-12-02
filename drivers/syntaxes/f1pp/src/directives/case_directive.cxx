/*case_directive.cxx*/
/** @file
 *
 * @brief The #case directive parser.
 *
 * @todo Macro expressions (like `A &~B | (C & D)`)
 */
#include "../pch.h"
#include "../main.h"

f1::f1pp::case_directive::case_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::case_directive::get_name() const
{
	return "case";
}
// {E3762B29-545C-45d2-A63D-09CA293407DA}
const f1::guid_t f1::f1pp::case_directive::m_guid =
{ 0xe3762b29, 0x545c, 0x45d2, { 0xa6, 0x3d, 0x9, 0xca, 0x29, 0x34, 0x7, 0xda } };
const f1::guid_t & f1::f1pp::case_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::case_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_keep_text_eol | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::case_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	switch_directive::frame * frm = type_cast<switch_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#case" );

	// Check for disabled statement (fast path)
	if( frm->get_flags( pp::Iframe::fl_initially_disabled ) )
	{
		f1pp_par->skip_to_newline();
		return STATUS_SUCCESS;
	}

	// Ignore case if we already have successful variant parsed
	if( 0 != frm->m_case_position.m_line )
	{
		f1pp_par->skip_to_newline();
		finish( *f1pp_par, *frm, false );
		return STATUS_SUCCESS;
	}

	// Fetch expression string and expand macros
	if( switch_directive::frame::Etype_switch == frm->m_switch_type )
	{
		f1pp_par->modify_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode );
		f1pp_par->capture_line( line_capture_callback, this );
		return STATUS_SUCCESS;
	}

	// Process switchdef/switchndef case
	bool case_succeeded = true;
	pp::string ident;
	for(;;)
	{
#if 1
		bool result = false;
		if( f1pp_par->parse_macro_expression( result ).failed_status( status ) )
			return status;
#else
		f1pp_par->skip_real_spaces_and_comments();
		if( f1pp_par->parse_macro_identifier( ident ).failed() )
		{
			f1pp_par->skip_to_newline();
			return parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		}

		core::Tconst_ref<pp::Imacro> mac;
		f1pp_par->find_macro( mac, ident );

		bool result;
		if( nullptr != mac )
		{
			result = true;
			mac.ref_const_cast()->on_macro_referenced();
		}
		else
			result = false;
#endif
		if( switch_directive::frame::Etype_switchdef == frm->m_switch_type )
			case_succeeded = case_succeeded && result;
		else
			case_succeeded = case_succeeded && !result;

		f1pp_par->skip_real_spaces_and_comments();
		if( par->is_nl() )
			break;
		if( !par->skip_char( ',' ) )
		{
			f1pp_par->skip_to_newline();
			return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "','" );
		}
	}

	finish( *f1pp_par, *frm, case_succeeded );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::case_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> f1pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	switch_directive::frame * frm = type_cast<switch_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#case" );

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
	if( ':' == *start )	// skip optional ':'
	{
		++ start;
		f1pp_par->skip_real_spaces_and_comments( start, end );
	}

	if( start < end )
		parser.message( c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "case" );

	// Process directive logics
	finish( *f1pp_par, *frm, result == frm->m_switch_value );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::case_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
	parser.modify_parser_flags( pp::Iparser::parser_flags_t::null, pp::Iparser::parser_flags_t::fl_expression_mode );
#ifdef _DEBUG
	core::Tref<f1::f1pp::case_directive> drc = type_cast<f1::f1pp::case_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::f1pp::case_directive *>( param )->execute( parser, buf );
#endif
}
const status_t f1::f1pp::case_directive::finish( f1::c_pp::parser & f1pp_par, switch_directive::frame & frm, bool allow_exec )
{
	// Process directive logics
	if( 0 == frm.m_endswitch_position.m_line )	// #endswitch hasn't been reached yet
	{
		if( allow_exec )
		{
			if( 0 != frm.m_case_position.m_line )
			{
				f1pp_par.message( f1pp::syntax::IMSG_ERROR_CASE_ALREADY_EXECUTED );
				f1pp_par.message( frm.m_case_position.m_source->get_filename(), frm.m_case_position.m_line, syntax::IMSG_INFO_STARTED_HERE );
			}
			else
			{
				f1pp_par.get_parser()->get_pos( frm.m_case_position );
				frm.allow_exec( true );
			}
		}
		else
		{
			if( 0 == frm.m_fallthrough_position.m_line )
				frm.allow_exec( false, true );
		}
	}
	else
	{
		if( 0 == frm.m_fallthrough_position.m_line )
			frm.allow_exec( false, true );
	}
	frm.m_fallthrough_position.reset();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF case_directive.cxx*/
