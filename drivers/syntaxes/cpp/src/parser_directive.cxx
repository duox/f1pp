/*parser_directive.cxx*/
/**
 * @brief Directives-related stuff.
 */
#include "pch.h"
#include "main.h"


const status_t f1::c_pp::parser::root_parser_hooks::on_esc_char( pp::Iparser & parser )
{
	core::Tref<c_pp::parser> pp_parser = type_cast<c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == pp_parser )
		return STATUS_ERROR_INVALID_STATE;
	return pp_parser->parse_directive();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Parsing

const status_t f1::c_pp::parser::parse_directive()
{
	status_t	status;

//	__debugbreak_if( nullptr == m_directive_list )
//		return STATUS_ERROR_NOT_INITIALIZED;

	// Skip spaces and comments between escape character and directive name
	skip_real_spaces_and_comments();
	if( m_parser->is_nl() )
		return STATUS_SUCCESS;	// empty directive
	m_directive_name_start = m_parser->get_ptr();

	// Parse #line directive
	if( isdigit( m_parser->peek_char() ) )
	{
#if 1
		// Fetch line number
		pp::variant line_no;
		bool parsed = false;
		skip_real_spaces_and_comments();
		parsed = parse_integer( line_no );
		if( !parsed )
			return message( syntax::IMSG_ERROR_EXPECTED_INTEGER );
		if( 0 >= line_no.as_int() )
			return message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );

		// Parse file name if it present
		pp::string filename;
		skip_real_spaces_and_comments();
		if( !m_parser->peek_char( ':' ) )
		{
			skip_real_spaces_and_comments();
			if( m_parser->skip_char( '\"' ) )
				status = parse_string( filename, '\"', c_pp::parser::string_parse_flags_t::fl_parse_string_ignore_unterm );
			else
				status = parse_delimited_string( filename, ':' );

			if( status.failed() )
				return status;
			if( filename.empty() )
				return message( syntax::IMSG_ERROR_EXPECTED_FILENAME );
		}

		// Set current line and file name
		if( !filename.empty() )
			m_parser->set_current_file_name( filename, f1::parser::lineno_t(line_no.as_int()) );
		else
			m_parser->set_line( f1::parser::lineno_t(line_no.as_int()) );
#else
		// Find #line directive parser
		if( nullptr == m_line_directive )
		{
			core::Tconst_ref<pp::Idirective> drct;
			get_directive_list()->find_directive( drct, f1::c_pp::c99::line_directive::m_guid );
			m_line_directive = drct;
		}

		// Parse #line directive arguments
		if( nullptr != m_line_directive )
			m_line_directive->parse( *this );
#endif

		// Check for extension version # nnn file : text
		skip_real_spaces_and_comments();
		if( m_parser->skip_char( ':' ) )
			set_newline();	// simulate beginning of the line

		// Exit
		return STATUS_SUCCESS;
	}

	// Fetch directive name
	pp::string ident;
	if( parse_directive_identifier( ident ).failed() || ident.is_empty() )
	{
		message( syntax::IMSG_ERROR_EXPECTED_DIRECTIVE_NAME );
		return STATUS_SUCCESS;
	}

	// Find directive
	core::Tref<pp::Idirective> drective = nullptr;
	if( m_directive_list.find_directive( drective, (m_c_flags & c_flags_t::fl_directive_no_case) ? pp::directive_list::fl_compare_no_case : 0, ident ).failed_status( status ) )
	{
		if( m_c_flags & c_flags_t::fl_execute_exec_top_only )
		{
			output( pp::string( m_directive_start, m_parser->get_ptr() - m_directive_start ) );
			return STATUS_SUCCESS;
		}

		if( STATUS_ERROR_NOT_FOUND == status )
			status = on_unknown_directive( ident );
		return status;
	}

	// Check whether this directive for particular flags
	if( drective->get_flags( pp::Idirective::bf_deprecated ) )
		message( syntax::IMSG_WARNING_DEPRECATED_DIRECTIVE_S, ident.c_str() );
	if( drective->get_flags( pp::Idirective::bf_ignore ) )
	{
		skip_to_newline();
		return STATUS_SUCCESS;
	}

	// Skip directive if execution is disabled
	if( !is_exec_allowed() )
	{
		if( !drective->get_caps( pp::Idirective::cap_invariant ) )
		{
			pp::Iframe * frm = drective->alloc_idle_frame( *this );
			if( nullptr != frm )
			{
				frm->modify_flags( Iframe::fl_idle );
				enter_directive( *frm );
			}
			skip_to_newline();
			return STATUS_SUCCESS;
		}
		/*m_pre_directive_can_exec = */m_can_exec = true;//set_allow_exec( true );	// for the time of execution of the directive, allow its execution
	}

	// Check whether this directive for particular flags
	if( m_c_flags & c_flags_t::fl_execute_exec_top_only )
	{
		if( drective->get_caps( pp::Idirective::cap_structured ) )
		{
			pp::Iframe * frm = drective->alloc_idle_frame( *this );
			__debugbreak_if( nullptr == frm )
				;
			else
			{
				frm->modify_flags( Iframe::fl_idle );
				enter_directive( *frm );
			}
			skip_to_newline();
			output( pp::string( m_directive_start, m_parser->get_ptr() - m_directive_start ) );
			return STATUS_SUCCESS;
		}

		pp::Iframe * frm = get_directive_frame();
		if(	nullptr != frm )
		{
			// Check for disabled statement (fast path)
			if( frm->get_flags( pp::Iframe::fl_initially_disabled | pp::Iframe::fl_abort_statement ) )
			{
				skip_to_newline();
				leave_directive();	// complete directive
				return STATUS_SUCCESS;
			}
			// Idle frame -- place entire directive to the output
			if( frm->get_flags( pp::Iframe::fl_idle ) && frm->get_closing_directive_guid() == drective->get_guid() )
			{
				skip_to_newline();
				leave_directive();
				output( pp::string( m_directive_start, get_parser()->get_ptr() - m_directive_start ) );
				return STATUS_SUCCESS;
			}
			if(	!drective->get_caps( pp::Idirective::cap_forced ) )
			{
#if 1
				if( drective->get_guid() == frm->get_openning_directive_guid() )
				{
					frm->inc_nesting();
					output( pp::string( m_directive_start, m_parser->get_ptr() - m_directive_start ) );
					return STATUS_SUCCESS;
				}
				else if( drective->get_guid() == frm->get_closing_directive_guid() )
				{
					if( 0 != frm->get_nesting() )
					{
						frm->dec_nesting();
						output( pp::string( m_directive_start, m_parser->get_ptr() - m_directive_start ) );
						return STATUS_SUCCESS;
					}
				}
				else
#else
				if( drective->get_guid() != frm->get_openning_directive_guid() && drective->get_guid() != frm->get_closing_directive_guid() )
#endif
				{
					skip_to_newline();

					pp::string buf;
					buf.add_part(
						m_directive_start,
						m_parser->get_ptr() - m_directive_start
						);
					output( buf );

					return STATUS_SUCCESS;
				}
			}
		}
	}
	if( drective->get_caps( pp::Idirective::cap_unimplemented ) || drective->get_flags( pp::Idirective::bf_pass_through ) )
	{
		if( m_c_flags & c_flags_t::fl_drop_pass_through_directives )
			skip_to_newline();
		else
			output_directive( *drective );
		return STATUS_SUCCESS;
	}

	// Parse directive and exit
	return on_execute_directive( *drective );
}
const status_t f1::c_pp::parser::output_directive( pp::Idirective & drective )
{
	pp::string buf;
	parse_to_newline( buf );

	output( "#", 1 );
	output( drective.get_name() );
	output( buf );

	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::on_execute_directive( pp::Idirective & obj )
{
	// Execute directive
	status_t status = super::on_execute_directive( obj );

	// Check end of directive is reached
	if( 0 == obj.get_caps( pp::Idirective::cap_keep_text_eol ) )
	{
		skip_real_spaces_and_comments();
		if( !m_parser->is_nl() )
		{
			if( m_pre_directive_can_exec || m_post_directive_can_exec )
				message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, (core::string( "#" ) + obj.get_name()).c_str() );
			skip_to_newline();
		}
	}

	// Exit
	return status;
}
const status_t f1::c_pp::parser::on_unknown_directive( const pp::string & ident )
{
	skip_to_newline();

	// Display standard for which directive is defined
	const pp::Istandard * standard = pp::service::get().find_item_standard( pp::Idirective::m_item_class_guid, ident );
	if( nullptr != standard )
		message( syntax::IMSG_WARNING_NOT_AVAILABLE_STANDARD_SS, ident.c_str(), standard->get_description_string() );
	else
		// Show error message
		message( syntax::IMSG_ERROR_UNKNOWN_DIRECTIVE, ident.c_str() );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF parser_directive.cxx*/
