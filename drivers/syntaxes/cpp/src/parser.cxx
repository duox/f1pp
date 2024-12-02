/*parser.cxx*/
#include "pch.h"
#include "main.h"

f1::c_pp::parser::parser( const core::Iidentifier * ident ):
	super( ident ),
	m_VA_ARGS_supported( false ), m_VA_OPT_supported( false ),
	m_c_flags( c_flags_t::null ), m_c_message_flags( c_message_flags_t::null ),
	m_esc_char( '#' ), m_charizing_char( '@' ),
	m_can_exec( true ), m_pre_directive_can_exec( true ), m_post_directive_can_exec( true ),//m_exec_disable_count( 0 ),
	m_error_directive_count( 0 ),
	m_seq_start( nullptr ),
	m_disable_line_update( false ), m_disable_next_line_update( false ),
	m_allow_args_separators( false ), m_new_line( false ), m_was_new_line( false ),
	m_macro_expansion_nesting( 0 ),
	m_capturing_line( false ), m_capture_callback_call( 0 )/*, m_pre_capture_args_parse_mode( 0 )*/
{
	memset( m_esc_chars, 0, sizeof(m_esc_chars) );
	m_c_flags |= c_flags_t::fl_standard_digraphs | c_flags_t::fl_standard_trigraphs;

	m_event_handler.initialize();

//	m_capture_line_breakpoint_callback.m_parser = this;
}

const status_t f1::c_pp::parser::_initialize( f1::parser::Isource * source )
{
	status_t status;

	// Check current state
	__debugbreak_if( nullptr == m_context )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Call base class
	if( super::_initialize( nullptr/*source*/ ).failed_status( status ) )
		return status;

	// Initialize object
	load_default_escapes();
	m_custom_esc_seqs = false;

	if( nls::service::get().find_encoding( m_nls_utf8_encoding, nls::eid_mibe_utf8 ).failed_status( status ) )
		return STATUS_ERROR_NOT_INITIALIZED;
	if( nls::service::get().find_encoding( m_nls_utf16_encoding, nls::eid_mibe_utf16le ).failed_status( status ) )
		return STATUS_ERROR_NOT_INITIALIZED;
	if( nls::service::get().find_encoding( m_nls_utf32_encoding, nls::eid_mibe_utf32le ).failed_status( status ) )
		return STATUS_ERROR_NOT_INITIALIZED;

	m_unexpanded_buffer_output.initialize( *m_context );
	//m_capture_output.initialize( *this );
	//m_buffer_output.initialize( *m_context );

	set_parser_hooks( m_root_parser_hooks );

//	append_digraphs( "<::><%%>%:", "[]{}#", 5 );		// TODO: remove (separate step): uncomment if some non-standard digraphs must be added
//	append_trigraphs( "=/'()!<>-", "#\\^[]|{}~", 9 );	// TODO: remove (separate step): uncomment if some non-standard trigraphs must be added

	// Set source processor hook
//	m_parser->append_source_listener( m_source_listener );
	m_parser->register_event_handler(
		f1::parser::event_type_t(f1::parser::etype_patch_started | f1::parser::etype_patch_completed | f1::parser::etype_source_included | f1::parser::etype_source_entered | f1::parser::etype_source_leaving | f1::parser::etype_ptr_changing | f1::parser::etype_all_patches_passed),
		m_parser_event_handler, this );
	if( nullptr != source )
		m_parser->insert_source( *source );
//		on_source_added( *m_parser, source );

	// Define operators
	add_unary_operator( m_prefix_unary_operator_desc_list, unary_lnot, "!", 1 );
	add_unary_operator( m_prefix_unary_operator_desc_list, unary_lnot, "not", 3, fl_uop_deprecated );
	add_unary_operator( m_prefix_unary_operator_desc_list, unary_not,  "~", 1 );
	add_unary_operator( m_prefix_unary_operator_desc_list, unary_not,  "compl", 5, fl_uop_deprecated );
	add_unary_operator( m_prefix_unary_operator_desc_list, unary_minus, "-", 1 );
	add_unary_operator( m_prefix_unary_operator_desc_list, unary_plus, "+", 1 );

	add_binary_operators( m_default_binary_operator_desc, countof(m_default_binary_operator_desc) );

	// Execute parser commands
//	array_foreach( core::Tobject_array<Iparser::Icommand>::const_iterator, it, m_context->get_parser_commands() )
//		it->execute( *this );

	// Add context definitions
	// TODO
	//append_include_paths( m_context->get_include_dirs() );
	//check_include_dirs( get_search_dir_list() );

	array_foreach( core::Tarray<pp::context::include_file_t>::const_iterator, it, m_context->get_include_files() )
	{
		const pp::context::include_file_t & file = *it;
		if( file.m_flags_to_set & pp::context::include_file_t::fl_auto_include )
			include_file( include_flags_t::fl_user, file.m_file_name.data(), file.m_file_name.length() );
		if( file.m_flags_to_set & pp::context::include_file_t::fl_deprecate )
		{
			core::string full_path_name;
//			resolve_include_filename( full_path_name, core::stringslice(file.m_file_name),
//				!!(file.m_flags_to_set & pp::context::include_file_t::fl_system) );
			include_desc * inc_desc = nullptr;
			register_include( inc_desc,
				file.m_file_name.data(), file.m_file_name.length(),
				include_flags_t(file.m_flags_to_set &~ file.m_flags_to_clear) );
			modify_include_flags( *const_cast<include_desc *>(inc_desc), include_flags_t::fl_deprecated );
		}
	}

	array_foreach( core::Tarray<pp::context::macro_def_t>::const_iterator, it, m_context->get_macros_to_define() )
	{
		const pp::context::macro_def_t & macdef = *it;
		core::Tref<pp::Imacro> mac;
		create_macro( mac, pp::string(macdef.m_name), nullptr, nullptr, pp::string(macdef.m_value) );
	}

	array_foreach( core::Tarray<core::string>::const_iterator, it, m_context->get_macros_to_undefine() )
	{
		const core::string & name = *it;

		core::Tref<pp::Imacro> mac;
		if( find_macro( mac, pp::string(name), pp::Iparser::find_macro_flags_t::fl_macro_search_force ).failed() )
			message( syntax::IMSG_WARNING_UNDEFINED_IDENTIFIER_IN_UNDEF, name.c_str() );
		else
			remove_macro( *mac );
	}

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::on_line_changed( f1::parser::lineno_t line )
{
	if( m_disable_line_update )
		return STATUS_SUCCESS;
	if( m_disable_next_line_update )
	{
		//m_disable_next_line_update = false;
		return STATUS_SUCCESS;
	}
	return super::on_line_changed( line );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const status_t f1::c_pp::parser::modify_c_flags( c_flags_t flags_to_set, c_flags_t flags_to_clear, c_flags_t * prev_flags )
{
	if( nullptr != prev_flags )
		*prev_flags = m_c_flags;
	m_c_flags = (m_c_flags | flags_to_set) & ~flags_to_clear;
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::set_c_flags( c_flags_t flags, c_flags_t * prev_flags )
{
	if( nullptr != prev_flags )
		*prev_flags = m_c_flags;
	m_c_flags = flags;
	return STATUS_SUCCESS;
}
f1::c_pp::parser::c_flags_t f1::c_pp::parser::get_c_flags( c_flags_t mask ) const
{
	return m_c_flags & mask;
}

const status_t f1::c_pp::parser::modify_c_message_flags( c_message_flags_t flags_to_set, c_message_flags_t flags_to_clear, c_message_flags_t * prev_flags )
{
	if( nullptr != prev_flags )
		*prev_flags = m_c_message_flags;
	m_c_message_flags = (m_c_message_flags | flags_to_set) & ~flags_to_clear;
	return STATUS_SUCCESS;
}
f1::c_pp::parser::c_message_flags_t f1::c_pp::parser::get_c_message_flags( c_message_flags_t mask ) const
{
	return m_c_message_flags & mask;
}

const status_t f1::c_pp::parser::set_esc_char( int ch )
{
	m_esc_char = ch;
	return STATUS_SUCCESS;
}
int f1::c_pp::parser::get_esc_char() const
{
	return m_esc_char;
}

const status_t f1::c_pp::parser::set_charizing_char( int ch )
{
	m_charizing_char = ch;
	return STATUS_SUCCESS;
}
int f1::c_pp::parser::get_charizing_char() const
{
	return m_charizing_char;
}

const status_t f1::c_pp::parser::set_allow_exec( bool allow, bool force )
{
#if 0
	if( !allow )
	{
		//__assert( m_can_exec != allow );
		m_exec_disable_count ++;
		m_can_exec = false;
	}
	else
	{
		//__assert( m_can_exec != allow );
		/*__debugbreak_ifnot*/if( 0 != m_exec_disable_count )
			m_exec_disable_count --;
		if( 0 == m_exec_disable_count )
			m_can_exec = true;
	}
#else
	m_post_directive_can_exec = allow;
	//if( /*allow || force || */( nullptr == m_directive_start && 0 == m_capture_callback_call ) )	// outside of a directive
	{
		m_can_exec = allow;
	}
#endif
	return STATUS_SUCCESS;
}
bool f1::c_pp::parser::was_exec_allowed() const
{
	return m_post_directive_can_exec;
}
bool f1::c_pp::parser::is_exec_allowed() const
{
	if( m_can_exec )
		return true;

//	const f1::pp::Iframe * frm = get_directive_frame();
//	if( nullptr != frm && 0 == frm->get_flags( pp::Iframe::fl_initially_disabled ) )
//		return true;

	//__assert( m_can_exec == (0 == m_exec_disable_count) );
	return false;
}

bool f1::c_pp::parser::set_ignore_next_newline( bool disable )
{
	const bool old_value = m_disable_next_line_update;
	m_disable_next_line_update = disable;
	return old_value;
}

unsigned f1::c_pp::parser::get_error_directive_count() const
{
	return m_error_directive_count;
}

unsigned f1::c_pp::parser::get_macro_expansion_nesting() const
{
	return m_macro_expansion_nesting;
}

// {9FE937DC-DE00-4aa3-9571-BBC1A21C7516}
const f1::guid_t f1::c_pp::parser::m_guid_clear_c_flags =
{ 0x9fe937dc, 0xde00, 0x4aa3, { 0x95, 0x71, 0xbb, 0xc1, 0xa2, 0x1c, 0x75, 0x16 } };
// {AB389579-54BB-4cc5-A44C-245CBD44B229}
const f1::guid_t f1::c_pp::parser::m_guid_set_c_flags = 
{ 0xab389579, 0x54bb, 0x4cc5, { 0xa4, 0x4c, 0x24, 0x5c, 0xbd, 0x44, 0xb2, 0x29 } };

const status_t f1::c_pp::parser::set_parameter( const f1::guid_t & param_guid, const core::variant & val )
{
	if( param_guid == m_guid_clear_c_flags )
		m_c_flags = c_flags_t::bits(m_c_flags & ~val.as_uint64());
	else if( param_guid == m_guid_set_c_flags )
		m_c_flags = m_c_flags | c_flags_t::bits(val.as_uint64());
	else
		return set_parameter( param_guid, val );
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations


const status_t f1::c_pp::parser::on_merge_operator( const pp::Imacro & mac, const pp::string & buf )
{
	__unreferenced_parameter( mac );
	__unreferenced_parameter( buf );
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::on_source_included( f1::parser::Isource & src ) noexcept
{
	m_new_line = true;
	__unreferenced_parameter( src );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::on_source_enter( f1::parser::Isource & src ) noexcept
{
	__unreferenced_parameter( src );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::on_source_leave( f1::parser::Isource & src ) noexcept
{
	__unreferenced_parameter( src );
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::parser_event_handler::invoke( f1::parser::event_type_t type_mask, void * handler_param, void * event_param, f1::parser::Iparser & parser )
{
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<f1::c_pp::parser *>(reinterpret_cast<core::object *>(handler_param));
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_PARAMETER;

	// When new source is added, we must process multigraphs if required, thus building source's virtual buffer
	if( type_mask & f1::parser::etype_source_included )
	{
		status_t status;

		// Check nesting level
		if( !c_pp_par->check_limit_value_min( pp::limit_t::li_maximum_include_depth, parser.get_nesting_level() ) )
			c_pp_par->message( syntax::IMSG_WARNING_MAXIMUM_NESTING_LEVEL_REACHED );

		// Check current state
		core::Tref<f1::parser::Isource>	source = type_cast<f1::parser::Isource *>(reinterpret_cast<core::object *>(event_param));
		__debugbreak_if( nullptr == source )
			return STATUS_ERROR_INVALID_PARAMETER;

		// Process multigraphs
		if( c_pp_par->get_c_flags( c_flags_t::fl_process_multigraphs ) )
		{
			core::stringslice src_buf;
			if( source->get_data( src_buf ).failed_status( status ) )
				return status;

			core::buffer buf;
			c_pp_par->process_multigraphs( buf, src_buf );	// process digraphs and trigraphs

			source->get_storage()->set_virtual_buffer_move( buf );	// set source virtual buffer
		}

		// Notify parser that new include file is included
		c_pp_par->on_source_included( *source );

		// Check it is a raw source
		// TODO
	}
	if( type_mask & (f1::parser::etype_source_entered) )
	{
		// Check current state
		core::Tref<f1::parser::Isource>	source = type_cast<f1::parser::Isource *>(reinterpret_cast<core::object *>(event_param));
		__debugbreak_if( nullptr == source )
			return STATUS_ERROR_INVALID_PARAMETER;

		// Notify parser that new include file is active
		c_pp_par->on_source_enter( *source );
	}
	if( type_mask & (f1::parser::etype_source_leaving) )
	{
		// Check current state
		core::Tref<f1::parser::Isource>	source = type_cast<f1::parser::Isource *>(reinterpret_cast<core::object *>(event_param));
		__debugbreak_if( nullptr == source )
			return STATUS_ERROR_INVALID_PARAMETER;
		c_pp_par->flush_chars();

		// Notify parser that new include file is getting inactive
		c_pp_par->on_source_leave( *source );

		if( c_pp_par->m_capturing_line && parser.get_root_source() == source )
			c_pp_par->finish_capture_line();
	}
	if( type_mask & (f1::parser::etype_source_leaving|f1::parser::etype_ptr_changing) )
	{
		// Flush internal buffer when pointer is changed
		c_pp_par->flush_chars();
	}
	if( type_mask & f1::parser::etype_patch_started )
	{
//		c_pp_par->m_allow_args_separators = false;
		DBG_INFO_allow_unexpanded_output( "c_pp::parser::parser_event_handler::invoke( f1::parser::etype_patch_started )", c_pp_par->m_allow_args_separators );
	}
	if( type_mask & f1::parser::etype_patch_completed )
	{
		DBG_INFO_allow_unexpanded_output( "c_pp::parser::parser_event_handler::invoke( f1::parser::etype_patch_completed )", c_pp_par->m_allow_args_separators );
#if __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__
		c_pp_par->end_subsection();
#endif // __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__
	}
	if( type_mask & f1::parser::etype_all_patches_passed )
	{
		c_pp_par->m_allow_args_separators = true;
		/*if( !c_pp_par->m_arg_parse_stack.empty() )
		{
			argument_parsing_context_t & argparse = **c_pp_par->m_arg_parse_stack.rbegin();
			c_pp_par->m_allow_args_separators = argparse.m_allow_args_separators;
		}
		else
			c_pp_par->m_allow_args_separators = true;*/
		DBG_INFO_allow_unexpanded_output( "c_pp::parser::parser_event_handler::invoke( f1::parser::etype_all_patches_passed )", c_pp_par->m_allow_args_separators );
	}

	// Exit
	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Main parsing loop.
 *
 * @return (status_t) operation status.
 */
const status_t f1::c_pp::parser::parse()
{
	status_t status;
	//core::Tref<pp::Imacro> mac;

	// Cached settings
	const bool do_merge_strings = 0 != m_context->get_flags( pp::context::fl_merge_strings );

	// Prepare
	__debugbreak_if( nullptr == m_output )
		return STATUS_ERROR_NOT_SUPPORTED;
	m_args_parse_mode = 0;
	set_newline( true );
	on_source_included( *m_parser->get_current_source() );
	on_source_enter( *m_parser->get_current_source() );

	m_VA_ARGS_supported	= nullptr != find_item_standard( pp::Imacro::m_item_class_guid, pp::string( "__VA_ARGS__" ) );
	m_VA_OPT_supported	= nullptr != find_item_standard( pp::Imacro::m_item_class_guid, pp::string( "__VA_OPT__" ) );

	core::Tref<f1::c_pp::syntax>	cpp_syntax;
	cpp_syntax = m_syntax.dynamic_pointer_cast<f1::c_pp::syntax>();
	__debugbreak_if( nullptr == cpp_syntax )
		return STATUS_ERROR_INVALID_STATE;

	// Skip hashbang line
	if( '#' == m_parser->peek_char_at( 0 ) && '!' == m_parser->peek_char_at( 1 ) )
		m_parser->skip_to_nl();

	// Process text data
	m_seq_start = nullptr;
	m_finish_capture_line = false;
	bool spaces_just_parsed = false, previous_spaces;
	while( !is_eof() )
	{
		__assert( status.succeeded() );
		__assert( nullcstr == m_seq_start || m_parser->is_valid_ptr( m_seq_start ) );
		if( m_finish_capture_line )
			finish_capture_line();

		previous_spaces = spaces_just_parsed;
		spaces_just_parsed = false;

		if( get_context()->is_event_enabled( pp::context::event_id_t::ev_parse_iteration ) )
		{
			get_context()->invoke_event( pp::context::event_id_t::ev_parse_iteration, this );
			if( is_eof() )
				break;
		}

		// Fetch the next character
		int ch = m_parser->peek_char();
		if( on_char( ch ) )
			continue;

		// Parse the text that disappears in the output
		if( '\n' == ch || '\r' == ch )
		{
			const bool prev_disable_next_line_update = m_disable_next_line_update;
			//if( m_c_flags & c_flags_t::fl_keep_new_lines )
			//	goto done;
			if( ')' == m_args_parse_mode ||			// new line is in argument list and thus a part of an argument
				!m_patch_callback_list.empty() )	// new line is a part of a macro; treat is as a regular space
			{
				flush_chars();
				m_disable_line_update = true;
				m_parser->skip_nl();	// drop new line in argument list
				m_disable_line_update = false;
				set_newline( true );
				start_char_seq();
			}
			else
				status = fetch_newline();

			if( prev_disable_next_line_update == m_disable_next_line_update )
				m_disable_next_line_update = false;

			//if( m_capturing_line && nullptr == m_capture_stack.back().m_breakpoint )
			//{
			//	//m_finish_capture_line = true;
			//}
			//else
            if( !m_disable_next_line_update )//&& m_allow_args_separators )
			{
				output( "\n", 1 );
				get_output()->on_line_changed( m_parser->get_line() );
			}
			//output( " " );
			spaces_just_parsed = true;
			continue;
		}

		if( isspace( ch ) )	// skip spaces
		{
			__assert( '\n' != ch && '\r' != ch );	// should be parsed earlier
			flush_chars();							// flush all gathered chars
			start_char_seq();						// and point to the first whitespace character
			int space_count = 0;
			for(;;)
			{
				int next_ch = m_parser->peek_char();
				if( !isspace( next_ch ) || '\n' == next_ch || '\r' == next_ch )
				{
					if( nullcstr != m_seq_start )
						-- m_seq_start;				// get back to the last whitespace character
					break;
				}
				m_parser->skip_char();
				++ space_count;
				if( nullcstr != m_seq_start )
					++ m_seq_start;
			}
			if( 1 != space_count || '\x20' != ch )	// if there are more than one space or that single space char is not '\x20'
			{										// replace all spaces with a single space
				output( "\x20", 1 );
				m_seq_start = nullptr;				// ignore all parsed spaces
			}
			__assert( nullptr == m_seq_start || m_parser->is_valid_ptr( m_seq_start ) );
			spaces_just_parsed = true;
			continue;
		}

		if( '\\' == ch )
		{
			flush_chars();
			m_parser->push_pos();
			m_parser->skip_char();
			skip_real_spaces_and_comments();
			if( m_parser->is_nl() )
			{
				__assert( false == m_disable_line_update );
				m_disable_line_update = true;
				m_parser->skip_nl();	// skip newline without notifying output
				m_disable_line_update = false;
				m_parser->pop_discard_pos();
				start_char_seq();		// do not use parse_newline() because this will break text split with '\\\n'
				spaces_just_parsed = true;
				continue;
			}
			m_parser->pop_pos();
		}	// otherwise do not skip backslashes if they don't screen new lines

		if( '/' == ch && !(m_c_flags & c_flags_t::fl_disable_comments) )
		{
			if( !m_allow_args_separators || !(m_c_flags & c_flags_t::fl_keep_comments) )
				flush_chars();		// keep sequence with comments
			else
				start_char_seq();	// start new sequence here if not started

			if( parse_comment() )
			{
				if( !(m_c_flags & c_flags_t::fl_keep_comments) && !m_new_line )
					output( "\x20", 1 );
				spaces_just_parsed = true;
				continue;
			}
		}

		if( m_new_line && ch == m_esc_char )	// parse directive if escape character is found
		{
			m_new_line = false;
			//__assert( m_allow_args_separators );

			// Check whether parsing directives is allowed
			bool allow_directive = true;
			if( m_c_flags & c_flags_t::fl_disable_directives )
				allow_directive = false;
			if( (m_c_flags & c_flags_t::fl_disable_directives_in_args) && ')' == m_args_parse_mode )
				allow_directive = false;

			// Parse directive, if allowed
			flush_chars();
			m_directive_start = m_parser->get_ptr();
			m_parser->skip_char();
			if( allow_directive /*&& m_allow_args_separators*/ )
			{
				// The problem is that is no argument list will appear after the macro name
				// (thus outputting "macro" name as is), this warning incorrectly displayed
				// prior we recognized that the macro name does not have argument list.
				// (See macro_non_macro test).
				//if( 0 != m_args_parse_mode )
				//	message( syntax::IMSG_WARNING_UNEXPECTED_DIRECTIVE );

				m_pre_directive_can_exec = m_post_directive_can_exec = m_can_exec;

				if( on_esc_char().failed_status( status ) )
					continue;

				if( !m_capturing_line )
					m_can_exec = m_post_directive_can_exec;
			}
			m_directive_start = nullptr;
			m_directive_name_start = nullptr;
			//m_new_line = true;
			continue;
		}

		// Starting from this point, directives are not recognized
		set_newline( false );
		spaces_just_parsed = false;

		// Start gathering chars from here
		start_char_seq();

		if( m_allow_args_separators )
		{
			// Check for argument parsing mode
			// If expected '(' is not found, it's not a macro call
			// Store macro name and a single space if a space flag was set
			if( m_can_exec && '(' == m_args_parse_mode )
			{
				argument_parsing_context_t & argparse = **m_arg_parse_stack.rbegin();

				// Start parsing args
				if( '(' == ch )
				{
					m_parser->skip_char();
					m_seq_start = m_parser->get_ptr();
					m_args_parse_mode = ')';
					//m_allow_args_separators = true;
					DBG_INFO_allow_unexpanded_output( "'(' start parsing args", m_allow_args_separators );

					argparse.m_macro->on_macro_referenced();	// mark macro is used at this point

					// Reset all buffers parsed so far
					argparse.m_expanded_buffer_output.reset();
					m_unexpanded_buffer_output.reset();

					//Iterate
					continue;
				}
				else
				{
					// At this point, if macro that requires args do not have it,
					// consider it as a simple identifier only if it's not a predefined macro
					abort_arg_parse();

					// Store single space is spaces were found
					if( previous_spaces )
						output( "\x20", 1 );
				}
			}

			if( '(' == ch )//&& !m_capturing_line )
			{
				// It's a nested parentheses
				// If we are parsing macro argument list, not macro value (ie no patch installed), treat open
				// bracket as a delimiter
				if( ')' == m_args_parse_mode || m_capturing_line )
				{
					//if( m_allow_args_separators )
						m_parentheses_stack.push_back( ')' );
				}
			}
			else if( ')' == ch )
			{
				if( m_capturing_line )
				{
					// If we're capturing a line to the EOL (of unlimited length), then
					// in the case of maching pairs, the line ends at the ')' character,
					// not just at the EOL character.
					const capture_context & cap = m_capture_stack.back();
					if( nullptr == cap.m_breakpoint && cap.m_parentheses_stack_depth == m_parentheses_stack.size() )
					{
						m_parser->skip_char();
						flush_chars();
						finish_capture_line();
						continue;	// do not eat the ')' character
					}
				}
				if( ')' == m_args_parse_mode )//&& !m_capturing_line )
				{
					// If there are nested parentheses, skip them
					if( !m_parentheses_stack.empty() )
					{
						//if( m_allow_args_separators )
							m_parentheses_stack.pop_back();
					}
					else
					{
						// Prepare parsing macro
						flush_chars();			// flush chars excluding closing ')'
						m_parser->skip_char();	// skip closing ')'

						// Expand macros in gathered string
						argument_parsing_context_t & argparse = **m_arg_parse_stack.rbegin();

						//if( !argparse.m_arg_list->get_arg_list().empty() || !argparse.m_expanded_buffer_output.get_buffer().is_empty() )
						if( !argparse.m_arg_list->get_arg_list().empty() || !m_unexpanded_buffer_output.get_buffer().is_empty() )
							next_arg_parsed();	// store the last argument only if there are previous arguments or it's not empty
						else
							argparse.m_expanded_buffer_output.reset(),
							/*argparse.*/m_unexpanded_buffer_output.reset();


						m_unexpanded_buffer_output.move( &argparse.m_saved_unexpanded_buffer_output );

						//m_args_parse_mode = argparse.m_previous_args_parse_mode;
						//argparse.m_previous_args_parse_mode = 0;
						//m_args_parse_mode = 0;	// write to normal output now

						// Prepare and output unexpanded output
						pp::string call_buf;
						format_unexpanded_macro_call( call_buf, argparse );
						if( output_unexpanded( call_buf ).failed_status( status ) )
							return status;

						expand_macro( *argparse.m_macro, argparse.m_arg_list );

						end_arg_parse();

						// Done
						continue;
					}
				}
			}

			if( ',' == ch )
			{
				if( m_parentheses_stack.empty() && ')' == m_args_parse_mode )//&& !m_capturing_line )
				{
					flush_chars();			// flush gathered
					m_parser->skip_char();	// skip ','
					next_arg_parsed();
					continue;
				}
			}
		}

		if( '*' == ch )
		{
			//if( m_allow_args_separators )	// comments are already removed at this step?
			{								// no
				if( '/' == m_parser->peek_char_at( 1 ) )
					message( syntax::IMSG_WARNING_OUTSIDE_OF_COMMENT );
			}
		}

		// Skip strings
		// Note that parsing strings must precede parsing identifiers to distinguish between L"" and IDENT""
		flush_chars();
		const char * prefix_start = m_parser->data();
		char_encoding_t str_enc;
		//size_t size_prefix = 0;
		int str_end_ch;
		string_parse_flags_t string_parse_flags = string_parse_flags_t::null;
		if( parse_string_header( str_enc, str_end_ch, string_parse_flags/*, &size_prefix*/ ) )
		{
			if( m_c_flags & c_flags_t::fl_disable_string_escapes )
				string_parse_flags |= string_parse_flags_t::fl_parse_string_no_escapes;

			if( !m_capturing_line && do_merge_strings )	// do not merge strings in capture mode
				merge_strings( str_enc, str_end_ch, string_parse_flags );
			else
			{
				//parse_string( str_end_ch, string_parse_flags );	// ignore encoding prefix as it will be stored by flush_char
				m_seq_start = prefix_start;	// start output with the string prefix
				__assert( nullcstr == m_seq_start || m_parser->is_valid_ptr( m_seq_start ) );
				skip_string( str_end_ch, string_parse_flags, str_enc );	// ignore encoding prefix as it will be stored by flush_char
			}
			continue;
		}

		// Expand macros
		if( /*!m_allow_args_separators &&*/ /*')' != m_args_parse_mode && */is_ident_first_char( ch ) )
		{
			// Reset all macro state:
			//	If we at the top level, all recurrent macros that have been blocked from expansion
			//	during reparse, now get unblocked.
			//if( m_patch_callback_list.empty() && !m_recursive_macros.empty() )
			//{
			//	array_foreach( f1::pp::macro_list_t::iterator, it, m_recursive_macros )
			//	{
			//		__assert( 0 != it->get_flags( f1::pp::Imacro::macro_flags_t::fl_recursive_locked ) );
			//		it->modify_flags( 0, f1::pp::Imacro::macro_flags_t::fl_recursive_locked );
			//	}
			//	m_recursive_macros.clear();
			//}

			// Parse identifier
			if( parse_identifier().failed_status( status ) )
				return status;
			continue;
		}

		// Gather characters
//		if( m_char_table[ch] & fl_char_break )
//			flush_chars();
		start_char_seq();
		m_parser->skip_char();
	}
	flush_chars();
	if( '(' == m_args_parse_mode )	// invocation of a function-like macro name is in the end of file without argument list
		abort_arg_parse();
	__debugbreak_if( m_capturing_line )
		finish_capture_line();
	finish_parse();

	// Check for errors
	if( 0 == get_msgout_context().get_flags( f1::msgout::context::bf_fatal_error ) )
	{
		// Check for directive stack
		array_foreach( core::Tarray<pp::Iframe *>::iterator, it, m_frame_stack )
		{
			message( syntax::IMSG_ERROR_UNTERMINATED_DIRECTIVE, (*it)->get_openning_directive_name()	);
			message( (*it)->get_initial_location(), syntax::IMSG_INFO_STARTED_HERE );
			(*it)->leave();
		}

		// Check for unfinished macro expansions
	}
	else
	{
		array_foreach( core::Tarray<pp::Iframe *>::iterator, it, m_frame_stack )
			(*it)->leave();
	}

	if( !m_arg_parse_stack.empty() )
	{
		__assert( 0 != m_args_parse_mode );
		array_foreach( core::Tarray<argument_parsing_context_t*>::const_iterator, it, m_arg_parse_stack )
		{
			argument_parsing_context_t * desc = *it;
			message( syntax::IMSG_ERROR_UNTERMINATED_MACRO_CALL, desc->m_ident.c_str() );
			message( desc->m_location, syntax::IMSG_INFO_STARTED_HERE );
			delete desc;
		}
	}
	else
	{
		__assert( 0 == m_args_parse_mode );
	}

	__assert( !m_capturing_line );

	array_foreach( core::Tobject_array<macro_object>::const_iterator, mobj_it, m_macro_list )
	{
		array_foreach( pp::const_macro_list_t::const_iterator, mac_it, mobj_it->m_macro_list )
		{
			core::Tconst_ref<f1::pp::Imacro> mac = *mac_it;

			core::Tconst_ref<f1::parser::Isource> src = mac->get_definition_source();
			if( nullptr == src )
				continue;	// defined on the command line
			if( nullptr != src->get_parent() )
				continue;	// nested include

			if( 0 == mac->get_reference_count() )
			{
				const char * macro_name = mobj_it->get_default_name_string().c_str();
				if( nullptr == macro_name || nullchar == *macro_name )
				{
					macro_name = mac->get_name();
					if( nullptr == macro_name )
						macro_name = "?";
				}

				const pp::Imacro::definition * def = mac->get_first_definition();
				if( nullptr == def )
					message( syntax::IMSG_WARNING_MACRO_IS_NEVER_USED_S, macro_name );
				else
					message(
						def->m_definition_source->get_display_filename(),
						def->m_definition_line,
						syntax::IMSG_WARNING_MACRO_IS_NEVER_USED_S, macro_name
						);
			}
		}
	}

	// Dump elements
	if( !get_context()->get_dump_task_list().empty() )
		dump_items();

	// Exit
	__assert( status.succeeded() );
	return status;
}
const status_t f1::c_pp::parser::abort_parser()
{
	//flush_chars();	- discard data
	m_seq_start = nullptr;				// ignore all parsed spaces

	m_parser->abort();
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::finish_parse()
{
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::begin_arg_parse( const pp::string & ident, pp::Imacro & mac )
{
	DBG_INFO_argparse(( "begin_arg_parse( %s, 0x%08p )\n", ident.c_str(), &mac ));

	argument_parsing_context_t * argparse = new(std::nothrow) argument_parsing_context_t;
	__debugbreak_if( nullptr == argparse )
		return STATUS_ERROR_INSUFFICIENT_MEMORY;
	argparse->m_previous_storage = get_storage();
	argparse->m_previous_args_parse_mode = m_args_parse_mode;
	argparse->m_parentheses_stack.move( m_parentheses_stack );
	argparse->m_ident = ident;
	argparse->m_expanded_buffer_output.initialize( *m_context );
//	argparse->m_unexpanded_buffer_output.initialize( *m_context );
//	argparse->m_sub_unexpanded_buffer_output.initialize( *m_context );
	argparse->m_arg_list.createT<pp::arg_list>();
	argparse->m_arg_list->initialize();
	argparse->m_macro = &mac;
	m_parser->get_location( argparse->m_location );
	argparse->m_allow_args_separators = m_allow_args_separators;
	DBG_INFO_allow_unexpanded_output( "begin_arg_parse: saving", m_allow_args_separators );

	//argparse->m_saved_capturing_line = m_capturing_line;
	m_capture_save_stack.push_back( m_capturing_line );
	m_capturing_line = false;

	set_storage( &argparse->m_expanded_buffer_output );	// redirect output to a buffer
	m_args_parse_mode = '(';						// store expected character
	m_allow_args_separators = true;
	DBG_INFO_allow_unexpanded_output( "begin_arg_parse: setting", m_allow_args_separators );

	argparse->m_saved_unexpanded_buffer_output.initialize( *m_context );
	argparse->m_saved_unexpanded_buffer_output.move( &m_unexpanded_buffer_output );

	argparse->m_output.initialize();
	argparse->m_output.begin_output();
	argparse->m_previous_output = get_output();
	set_output( &argparse->m_output );

	m_arg_parse_stack.push_back( argparse );

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::end_arg_parse( bool abort )
{
	DBG_INFO_argparse(( "end_arg_parse( abort = %s )\n", abort ? "true" : "false" ));

	argument_parsing_context_t & argparse = **m_arg_parse_stack.rbegin();

	// Disconnect from the current output and connect to the previous one
	argparse.m_output.end_output();
	set_output( argparse.m_previous_output );
	set_storage( argparse.m_previous_storage );

	// Get access to the buffered data
	const core::buffer * buf = abort ?
		&m_unexpanded_buffer_output.get_buffer() :
		&argparse.m_expanded_buffer_output.get_buffer();

	// Check macro argument list matches macro parameter list
	if( !argparse.m_arg_list->get_arg_list().empty() )
	{
		__debugbreak_ifnot( nullptr != argparse.m_macro->get_param_list() )
		{
			if( !argparse.m_macro->get_param_list()->match_arg_list( *argparse.m_arg_list ) )
				message( syntax::IMSG_ERROR_ARG_LIST_PARAM_LIST_MISMATCH_S, argparse.m_ident.c_str() );
		}
	}

	// Update previous macro fetch
	if( m_arg_parse_stack.size() > 1 )
	{
		argument_parsing_context_t & prev_exp = **(m_arg_parse_stack.rbegin() + 1);

		//prev_exp.m_expanded_buffer_output.reset();
		if( 0 != buf->get_data_size() )
			prev_exp.m_expanded_buffer_output.write( (const char *) buf->get_ptr(), buf->get_data_size() );

		//prev_exp.m_unexpanded_buffer_output.reset();
//		__assert( prev_exp.m_unexpanded_buffer_output.get_buffer().is_empty() );
//		prev_exp.m_unexpanded_buffer_output.get_buffer().clear();

		//if( m_allow_args_separators )
		//if( prev_exp.m_unexpanded_buffer_output.get_buffer().is_empty() )
		{
			//buf = &argparse.m_unexpanded_buffer_output.get_buffer();
			//prev_exp.m_sub_unexpanded_buffer_output.write( (const char *) buf->get_ptr(), buf->get_data_size() );
	//	prev_exp.m_unexpanded_buffer_output.get_buffer().set_data( (const char *) buf->get_ptr(), buf->get_data_size() );
//		if( m_arg_parse_stack.size() == 2 )
//		{
//			buf = &argparse.m_unexpanded_buffer_output.get_buffer();
//			prev_exp.m_unexpanded_buffer_output.get_buffer().set_data( (const char *) buf->get_ptr(), buf->get_data_size() );
		}
	}
	else
	{
		if( 0 != buf->get_data_size() )
			argparse.m_previous_output->write( (const char *) buf->get_ptr(), buf->get_data_size() );

//		if( m_was_new_line )	- TODO: delete m_was_new_line?
//			argparse.m_previous_storage->write( "\n", 1 );
	}

	// Restore state
#if __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__
	//end_subsection();
#endif // __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__

//	m_capturing_line = argparse.m_saved_capturing_line;
	m_capturing_line = m_capture_save_stack.back();	m_capture_save_stack.pop_back();
	m_args_parse_mode = argparse.m_previous_args_parse_mode;
	m_parentheses_stack.move( argparse.m_parentheses_stack );
//	m_unexpanded_buffer_output.move( &argparse.m_saved_unexpanded_buffer_output );
	if( !argparse.m_allow_args_separators )
	{
		m_allow_args_separators = false;
		DBG_INFO_allow_unexpanded_output( "end_arg_parse: restoring", m_allow_args_separators );
	}

	// Pop one element from the stack
	delete &argparse;
	m_arg_parse_stack.pop_back();
	m_unexpanded_buffer_output.reset();

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::next_arg_parsed()
{
	status_t status;
	argument_parsing_context_t & argparse = **m_arg_parse_stack.rbegin();

	pp::string unexpanded_value;
	unexpanded_value.move( /*argparse.*/m_unexpanded_buffer_output.get_buffer() );
	//trim_spaces_and_comments( unexpanded_value, true );
	pp::string expanded_value;
	expanded_value.move( argparse.m_expanded_buffer_output.get_buffer() );
	trim_spaces_and_comments( expanded_value, false );

	argparse.m_arg_list->append_expanded_arg( unexpanded_value, expanded_value );

	argparse.m_expanded_buffer_output.reset();
	/*argparse.*/m_unexpanded_buffer_output.reset();

//	__debugbreak_ifnot( !m_patch_callback_list.empty() )
//	{
//		patch_callback * patch = *m_patch_callback_list.rbegin();
//		if( !argparse.m_recursive_macros.empty() )
//		{
//			patch->m_recursive_macros.append( argparse.m_recursive_macros );	// TODO: append_move
//			argparse.m_recursive_macros.clear();
//		}
//	}

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::abort_arg_parse()
{
	//__assert( '(' == m_args_parse_mode );
	//__assert( ')' == m_args_parse_mode );
	__debugbreak_if( m_arg_parse_stack.empty() )
		return STATUS_SUCCESS;
	flush_chars();

	const f1::c_pp::parser::argument_parsing_context_t & argparse = **m_arg_parse_stack.rbegin();

	//m_args_parse_mode = argparse.m_previous_args_parse_mode;
	core::Tref<pp::Imacro> mac = argparse.m_macro;
	pp::string ident = argparse.m_ident;
	end_arg_parse( true );

	if( mac->get_flags( pp::Imacro::macro_flags_t::fl_expression_only|pp::Imacro::macro_flags_t::fl_unique ) )
		message( syntax::IMSG_ERROR_EXPECTED_ARG_LIST_S, ident.c_str() );	// if a predefined macro missing arg list, report this
	else
		output( ident );

	on_line_changed( m_parser->get_line() );
	m_seq_start = m_parser->data();

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::format_unexpanded_macro_call( pp::string & call_buf, const argument_parsing_context_t & argparse ) const
{
	return format_unexpanded_macro_call( call_buf, argparse.m_ident, *argparse.m_macro, argparse.m_arg_list );
}
const status_t f1::c_pp::parser::format_unexpanded_macro_call( pp::string & call_buf,
	const pp::string & ident, const pp::Imacro & mac, const pp::arg_list * args ) const
{
	call_buf.add_part( ident );
	if( nullptr != args )
	{
		call_buf.add_part( "(" );
		array_foreach( pp::arg_list::arg_desc_list_t::const_iterator, it, args->get_arg_list() )
		{
			//__assert( !(*it).m_value.empty() );
			call_buf.add_part( (*it).m_value );
			if( !it.is_last() )
				call_buf.add_part( "," );
		}
		call_buf.add_part( ")" );
	}
	__unreferenced_parameter( mac );
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::output( const char * str, size_t len )
{
	if( !m_can_exec )
		return STATUS_SUCCESS;
	if( size_t(-1) == len )
		len = core::string::length( str );
	if( 0 == len )
		return STATUS_SUCCESS;

	if( 0 == m_args_parse_mode )
		return super::output( str, len );

	__debugbreak_ifnot( !m_arg_parse_stack.empty() )
	{
		argument_parsing_context_t & argparse = **m_arg_parse_stack.rbegin();
		argparse.m_expanded_buffer_output.write( str, len );
	/*if( m_arg_parse_stack.size() > 1 )
	{
		argument_parsing_context_t & prev_exp = **(m_arg_parse_stack.rbegin() + 1);
		prev_exp.m_unexpanded_buffer_output.write( str, len );
	}
	else*/
	{
//		if( m_allow_args_separators )
		if( 0 != m_args_parse_mode )
			/*argparse.*/m_unexpanded_buffer_output.write( str, len );
	}
	}

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::output_expanded( const char * str, size_t len )
{
	if( 0 == m_args_parse_mode )
		return output( str, len );

	argument_parsing_context_t & argparse = **m_arg_parse_stack.rbegin();
	argparse.m_expanded_buffer_output.write( str, len );

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::output_expanded( const pp::string & str )
{
	return output_expanded( str.data(), str.length() );
}
const status_t f1::c_pp::parser::output_unexpanded( const char * str, size_t len )
{
	//if( 0 == m_args_parse_mode )
//	if( !m_allow_args_separators )
//		return STATUS_SUCCESS;	// do not output anything unexpanded in normal mode

	/*if( m_arg_parse_stack.size() > 1 )
	{
		argument_parsing_context_t & prev_exp = **(m_arg_parse_stack.rbegin() + 1);
		prev_exp.m_unexpanded_buffer_output.write( str, len );
	}
	else
	{
		argument_parsing_context_t & argparse = **m_arg_parse_stack.rbegin();
		argparse.m_unexpanded_buffer_output.write( str, len );
	}*/
	m_unexpanded_buffer_output.write( str, len );

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::output_unexpanded( const pp::string & str )
{
	return output_unexpanded( str.data(), str.length() );
}

const status_t f1::c_pp::parser::capture_line( capture_callback * cb, core::object * param, size_t max_len )
{
	DBG_INFO_capture(( "CAPTURE: capture_line\n" ));

	if( 0 == max_len )
	{
		if( nullptr != cb )
			cb( param, *this, core::buffer() );
		return STATUS_SUCCESS;
	}

	capture_context * cap = m_capture_stack.append_new();

	cap->m_capture_callback = cb;
	cap->m_capture_callback_param = param;

	cap->m_capture_output = new(std::nothrow) capture_output;	// TODO: replace with static object
	cap->m_capture_output->initialize( *this );
	set_output( cap->m_capture_output, &cap->m_pre_capture_output ).assert_if_failed();

	cap->m_pre_capture_c_flags = m_c_flags;
	cap->m_pre_capture_can_exec = m_post_directive_can_exec;	//m_can_exec;
	cap->m_pre_capture_args_parse_mode = m_args_parse_mode;
	cap->m_pre_capturing_line = m_capturing_line;
	cap->m_parentheses_stack_depth = m_parentheses_stack.size();
	m_c_flags |= c_flags_t::fl_disable_directives;
	m_can_exec = true;
//	__assert( 0 == m_args_parse_mode );
	m_args_parse_mode = 0;
	m_capturing_line = true;

	cap->m_limited = size_t(-1) != max_len;	
	if( size_t(-1) != max_len )
	{
		cap->m_capture_line_breakpoint_callback = new(std::nothrow) capture_line_breakpoint_callback;
		cap->m_capture_line_breakpoint_callback->m_parser = this;
		m_parser->apply_breakpoint( max_len, *cap->m_capture_line_breakpoint_callback, &cap->m_breakpoint );
		DBG_INFO_breakpoints( "going to create" );
	}
	else
	{
		cap->m_capture_line_breakpoint_callback = nullptr;
		cap->m_breakpoint = nullptr;
		DBG_INFO_breakpoints( "not going to create" );
	}

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::abort_capture()
{
	return finish_capture_line( 0, false );
}
const status_t f1::c_pp::parser::finish_capture_line( f1::parser::lineno_t lineno, bool call_callback )
{
#if 1
	DBG_INFO_capture(( "CAPTURE: finish_capture_line\n" ));

	__debugbreak_if( m_capture_stack.empty() )
		return STATUS_ERROR_INVALID_STATE;

	capture_context * cap = &*m_capture_stack.rbegin();
	if( nullptr != cap->m_breakpoint )
		return STATUS_SUCCESS;

	if( 0 != m_args_parse_mode )
	{
		abort_arg_parse();
		message( syntax::IMSG_ERROR_EXPECTED_ARG_LIST );
	}

	if( 0 == lineno )
		lineno = m_parser->get_line();

	m_finish_capture_line = false;

	core::buffer buf;
	flush_chars();
	buf.move( cap->m_capture_output->get_buffer() );
	cap->m_capture_output = nullptr;
	set_output( cap->m_pre_capture_output );

//	if( !m_disable_next_line_update )
//		m_output->on_line_changed( lineno );
	m_c_flags = cap->m_pre_capture_c_flags;

	capture_callback * callback = cap->m_capture_callback;
	core::Tref<core::object> callback_param = cap->m_capture_callback_param;
	cap->m_pre_capture_output = nullptr;
	cap->m_capture_callback = nullptr;
	cap->m_capture_callback_param = nullptr;
	m_can_exec = cap->m_pre_capture_can_exec;
	m_args_parse_mode = cap->m_pre_capture_args_parse_mode;
	m_capturing_line = cap->m_pre_capturing_line;

	if( nullptr != cap->m_breakpoint )
	{
		m_parser->delete_breakpoint( *cap->m_breakpoint );
		delete cap->m_capture_line_breakpoint_callback;
	}

	m_capture_stack.pop_back();
	if( m_capture_stack.empty() )
		m_capturing_line = false;
	cap = nullptr;

	if( call_callback )
	{
		++ m_capture_callback_call;
		callback( callback_param, *this, buf );
		-- m_capture_callback_call;
	}
#else
	__debugbreak_if( !m_capturing_line )
		return STATUS_ERROR_INVALID_STATE;
	m_capturing_line = false;

	if( 0 == lineno )
		lineno = m_parser->get_line();

	set_output( m_pre_capture_output );
	m_output->on_line_changed( lineno );
	m_c_flags = m_pre_capture_c_flags;

	capture_callback * callback = m_capture_callback;
	core::Tref<core::object> callback_param = m_capture_callback_param;
	m_pre_capture_output = nullptr;
	m_capture_callback = nullptr;
	m_capture_callback_param = nullptr;
	m_can_exec = m_pre_capture_can_exec;
	m_args_parse_mode = m_pre_capture_args_parse_mode;

	callback( callback_param, *this, m_capture_output.get_buffer() );
#endif
	return STATUS_SUCCESS;
}
// Temporarily disable capturing
const status_t f1::c_pp::parser::disable_capture()
{
	/*__debugbreak_*/if( m_capture_stack.empty() )
		return STATUS_ERROR_INVALID_STATE;	// not in the capturing state
	capture_context * cap = &*m_capture_stack.rbegin();

	m_capturing_line = false;
	set_output( cap->m_pre_capture_output );
	m_c_flags = cap->m_pre_capture_c_flags;

	return STATUS_SUCCESS;
}
// Enable capturing again
const status_t f1::c_pp::parser::enable_capture()
{
	/*__debugbreak_*/if( m_capture_stack.empty() )
		return STATUS_ERROR_INVALID_STATE;	// not in the capturing state
	capture_context * cap = &*m_capture_stack.rbegin();

	m_capturing_line = true;
	set_output( cap->m_capture_output );
//	m_c_flags |= c_flags_t::fl_disable_directives;

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::capture_line_breakpoint_callback::on_breakpoint_reached()
{
	capture_context * cap = &*m_parser->m_capture_stack.rbegin();
	cap->m_breakpoint = nullptr;

	m_parser->flush_chars();
	m_parser->m_finish_capture_line = true;

	return STATUS_SUCCESS;
	//return m_parser->finish_capture_line();
}
f1::c_pp::parser::capture_output::capture_output( const core::Iidentifier * ident ) __noexcept:
	super( ident )
{
}
const status_t f1::c_pp::parser::capture_output::initialize( f1::c_pp::parser & parser )
{
	super::initialize();
	m_parser = &parser;
	return STATUS_SUCCESS;
}
const void f1::c_pp::parser::capture_output::reset()
{
	m_buffer.clear();
}
const status_t f1::c_pp::parser::capture_output::write( const char * string, size_t len )
{
	return m_buffer.append( string, len );
}
/*const status_t f1::c_pp::parser::capture_output::on_line_changed( f1::parser::lineno_t lineno ) noexcept
{
	__debugbreak_if( nullptr == m_parser )
		return STATUS_ERROR_INVALID_STATE;
	//return m_parser->finish_capture_line( lineno );
	return STATUS_SUCCESS;
}*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool f1::c_pp::parser::on_char( unsigned ch )
{
	__unreferenced_parameter( ch );
	return false;
}
bool f1::c_pp::parser::is_eof() const
{
	__debugbreak_if( nullptr == m_parser_hooks )
		return true;//STATUS_ERROR_INTERNAL;
	return m_parser_hooks->is_eof( *const_cast<self *>(this) );
}
const status_t f1::c_pp::parser::on_esc_char()
{
	__debugbreak_if( nullptr == m_parser_hooks )
		return STATUS_ERROR_INTERNAL;
	return m_parser_hooks->on_esc_char( *this );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Peek char from source buffer, taking in account digraphs and trigraphs.
 * @param[in] s (const char *&) pointer to the current position.
 * @param[in] end_s (const char *) pointer to the end of text data.
 * @return (int) character located at the s.
 *
 *	Note that if peek_char detects digraph or trigraph, it skips corresponding
 * number of symbols minus one, thus leaving s at the last character of digraph
 * or trigraph which will be different of the character returned.
 *
 *	This function is quite slow (because of digraph and trigraph support), consider
 * using function pointers instead.
 */
int f1::c_pp::parser::peek_char( const char *& s, const char * end_s ) const
{
	__debugbreak_if( s >= end_s )
		return '\0';

	// Parse digraph
	if( m_c_flags & c_flags_t::fl_process_digraphs )
	{
		if( end_s - s < 2 )
			return *s;
		size_t i = 0;
		for( const char * dig = m_digraph_map_first.data(); '\0' != *dig; dig += 2, ++ s )
		{
			if( * (uint16_t *) s == * (uint16_t *) dig )
			{
				s += 1;
				return m_digraph_map_second[i];
			}
		}
	}

	// Parse trigraph
	if( m_c_flags & c_flags_t::fl_process_trigraphs )
	{
		if( end_s - s < 3 )
			return *s;
		if( '?' != s[0] || '?' != s[1] )
			return *s;

		size_t i = 0;
		for( const char * pos = m_trigraph_map_first.data(); '\0' != pos[i]; ++ i )
		{
			if( pos[i] == s[2] )
			{
				s += 2;
				return m_trigraph_map_second.data()[pos - m_trigraph_map_first.data()];
			}
		}
	}

	// Exit
	return *s;
}
int f1::c_pp::parser::fetch_char( const char *& s, const char * end_s ) const
{
	__debugbreak_if( s >= end_s )
		return 0;
	int ch = peek_char( s, end_s );
	++ s;	// move to the next character
	return ch;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*const status_t f1::c_pp::parser::root_parser_hooks::on_esc_char( pp::Iparser & parser )
{
	core::Tref<c_pp::parser> c_pp_parser = type_cast<c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == c_pp_parser )
		return STATUS_ERROR_INVALID_STATE;
	return c_pp_parser->parse_directive();
}*/
const status_t f1::c_pp::parser::line_parser_hooks::on_esc_char( pp::Iparser & parser )
{
	core::Tref<c_pp::parser> c_pp_parser = type_cast<c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == c_pp_parser )
		return STATUS_ERROR_INVALID_STATE;
	return c_pp_parser->parse_directive();
}

int f1::c_pp::parser::get_end_bracket( int ch ) const
{
	if( '(' == ch )
		return ')';
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::parser::start_char_seq()
{
	if( nullptr == m_seq_start )
		m_seq_start = m_parser->data();
	__assert( m_parser->is_valid_ptr( m_seq_start ) );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::flush_chars()
{
	if( nullptr == m_seq_start )
		return STATUS_SUCCESS;
	__assert( m_parser->is_valid_ptr( m_seq_start ) );

	const char * end = m_parser->get_ptr();
	if( end != m_seq_start )
		output( m_seq_start, end - m_seq_start );
	m_seq_start = nullptr;

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::skip_lexem( const char * & s, const char * end_s, core::Tarray<char> & bracket_stack )
{
	if( s >= end_s )
		return STATUS_SUCCESS;

	switch( *s )
	{
	case '\\':
		++ s;
		if( '\n' == s[0] || '\r' == s[0] )
		{
			if( '\n' == s[0] && '\r' == s[1] )
				++ s;
			else if( '\r' == s[0] && '\n' == s[1] )
				++ s;
		}
		break;
	case '/':
		if( !skip_real_spaces_and_comments( s, end_s ) )
			++ s;
		break;
	case '(':
		bracket_stack.push_back( ')' );
		++ s;
		break;
	case '{':
		bracket_stack.push_back( '}' );
		++ s;
		break;
	case ')':
		if( !bracket_stack.empty() && ')' == bracket_stack.back() )
			bracket_stack.pop_back();
		++ s;
		break;
	case '}':
		if( !bracket_stack.empty() && '}' == bracket_stack.back() )
			bracket_stack.pop_back();
		++ s;
		break;
	default:
		if( isspace( *s ) )
		{
			skip_real_spaces_and_comments( s, end_s );
			break;
		}
		char_encoding_t str_enc;
		int str_end_ch;
		string_parse_flags_t string_parse_flags = string_parse_flags_t::null;
		if( parse_string_header( s, end_s, str_enc, str_end_ch, string_parse_flags/*, &size_prefix*/ ) )
		{
			if( m_c_flags & c_flags_t::fl_disable_string_escapes )
				string_parse_flags |= string_parse_flags_t::fl_parse_string_no_escapes;
			skip_string( s, end_s, str_end_ch, nullptr, string_parse_flags );	// ignore encoding prefix as it will be stored by flush_char
			break;
		}
		++ s;
		break;
	}
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::skip_to_separator( const char * & s, const char * end_s, int end_ch )
{
	core::Tarray<char>	bracket_stack;
	while( s < end_s )
	{
		if( bracket_stack.empty() && end_ch == *s )
			break;
		if( '\n' == *s || '\r' == *s )
			break;
		skip_lexem( s, end_s, bracket_stack );
	}
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::skip_to_separator( size_t & len, int end_ch )
{
	const char * start_s = m_parser->get_ptr();
	const char * end_s = m_parser->get_end_ptr();
	const char * s = start_s;
	status_t status = skip_to_separator( s, end_s, end_ch );
	len = s - start_s;
//	if( s != end_s )
//		-- len;
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}
const status_t f1::c_pp::parser::find_separator( size_t & len, int end_ch )
{
	const char * start_s = m_parser->get_ptr();
	const char * end_s = m_parser->get_end_ptr();
	const char * s = start_s;
	status_t status = skip_to_separator( s, end_s, end_ch );
	len = s - start_s;
//	if( s != end_s )
//		-- len;
	return status;
}

const status_t f1::c_pp::parser::skip_to_separator( const char * & s, const char * end_s, int end_ch1, int end_ch2 )
{
	core::Tarray<char>	bracket_stack;
	while( s < end_s )
	{
		if( bracket_stack.empty() && (end_ch1 == *s || end_ch2 == *s) )
			break;
		if( '\n' == *s || '\r' == *s )
			break;
		skip_lexem( s, end_s, bracket_stack );
	}
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::skip_to_separator( size_t & len, int end_ch1, int end_ch2 )
{
	const char * start_s = m_parser->get_ptr();
	const char * end_s = m_parser->get_end_ptr();
	const char * s = start_s;
	status_t status = skip_to_separator( s, end_s, end_ch1, end_ch2 );
	len = s - start_s;
//	if( s != end_s )
//		-- len;
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}
const status_t f1::c_pp::parser::find_separator( size_t & len, int end_ch1, int end_ch2 )
{
	const char * start_s = m_parser->get_ptr();
	const char * end_s = m_parser->get_end_ptr();
	const char * s = start_s;
	status_t status = skip_to_separator( s, end_s, end_ch1, end_ch2 );
	len = s - start_s;
//	if( s != end_s )
//		-- len;
	return status;
}

const status_t f1::c_pp::parser::skip_to_separator( const char * & s, const char * end_s, const char * end_ch )
{
	if( nullptr == end_ch )
		end_ch = "";

	core::Tarray<char>	bracket_stack;
	while( s < end_s )
	{
		if( bracket_stack.empty() )
		{
			for( const char * x = end_ch; nullchar != *x; ++ x )
			{
				if( *x == *s )
					goto done;
			}
		}
		if( '\n' == *s || '\r' == *s )
			break;
		skip_lexem( s, end_s, bracket_stack );
	}
done:
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::skip_to_separator( size_t & len, const char * end_ch )
{
	const char * start_s = m_parser->get_ptr();
	const char * end_s = m_parser->get_end_ptr();
	const char * s = start_s;
	status_t status = skip_to_separator( s, end_s, end_ch );
	len = s - start_s;
//	if( s != end_s )
//		-- len;
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}
const status_t f1::c_pp::parser::find_separator( size_t & len, const char * end_ch )
{
	const char * start_s = m_parser->get_ptr();
	const char * end_s = m_parser->get_end_ptr();
	const char * s = start_s;
	status_t status = skip_to_separator( s, end_s, end_ch );
	len = s - start_s;
//	if( s != end_s )
//		-- len;
	return status;
}

const status_t f1::c_pp::parser::parse_delimited_string( const char * & s, const char * end_s, int term_ch )
{
	core::Tarray<int>	stack;
	stack.push_back( term_ch );

	while( s < end_s )
	{
		if( '\'' == *s || '\"' == *s )
		{
			skip_string( s, end_s );
			continue;
		}
		if( *s == *stack.rbegin() )
		{
			stack.pop_back();
			if( stack.empty() )
				break;
		}
		else if( '(' == *s )
			stack.push_back( ')' );
		++ s;
	}

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::parse_delimited_string( pp::string & buf, int term_ch )
{
	const char * start = m_parser->get_ptr();
	const char * s = start;
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	parse_delimited_string( s, end_s, term_ch );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	buf.add_part( start, s - start );
	return STATUS_SUCCESS;
}

/*const status_t f1::c_pp::parser::parse_text( pp::string & buf )
{
	status_t status;

	// Parse expression string
	pp::Iparser::Iparser_hooks * old_hooks = get_parser_hooks();
	parser::line_parser_hooks buf_hooks;
	set_parser_hooks( buf_hooks );
	status = parse();
	set_parser_hooks( *old_hooks );
	if( status.failed() )
		return status;
	buf.move( buf_hooks.m_buffer );

	// Exit
	return STATUS_SUCCESS;
}*/

const status_t f1::c_pp::parser::dump_items()
{
	msgout::posix_file_FILE_output posix_file;

	array_foreach( core::Tarray<pp::context::dump_task>::const_iterator, task_it, get_context()->get_dump_task_list() )
	{
		const pp::context::dump_task & task = *task_it;
		msgout::context * msgout_context;
		msgout::context local_msgout_context;

		// Create stream if needed
		if( task.m_target_filename.empty() )
			msgout_context = &get_msgout_context();
		else
		{
			posix_file.set_filename( task.m_target_filename.c_str() );
			local_msgout_context.remove_all_outputs();
			local_msgout_context.append_output( posix_file );
			msgout_context = &local_msgout_context;
		}

		// Dump objects
		if( task.m_dump_flags & pp::context::dump_flags_t::fl_dump_directives )
		{
			const pp::directive_list * dirlist = get_directive_list();
			if( nullptr != dirlist ) {
				array_foreach( pp::const_directive_list_t::const_iterator, d_it, dirlist->get_directive_list() )
					msgout_context->print( "#%s\n", d_it->get_name() );
			}
		}
		if( task.m_dump_flags & pp::context::dump_flags_t::fl_dump_includes )
		{
			dump_includes( *msgout_context );
		}
		if( task.m_dump_flags & pp::context::dump_flags_t::fl_dump_macros )
		{
			core::string buf;
			array_foreach( core::Tobject_array<macro_object>::const_iterator, mobj_it, m_macro_list )
			{
				array_foreach( pp::const_macro_list_t::const_iterator, macro_it, mobj_it->m_macro_list )
				{
					if( macro_it->get_flags( pp::Imacro::macro_flags_t::fl_expression_only ) )
						continue;
					mobj_it->get_printable_full_name( buf );
					msgout_context->print( "%s\n", buf.c_str() );
				}
			}
		}
		if( task.m_dump_flags & pp::context::dump_flags_t::fl_dump_pragmas )
		{
			array_foreach( const_pragma_list_t::const_iterator, pragma_it, get_pragma_list() )
				msgout_context->print( "#pragma %s\n", pragma_it->get_name() );
		}
		if( task.m_dump_flags & pp::context::dump_flags_t::fl_dump_known_pragmas )
		{
			array_foreach( core::const_string_list_t::const_iterator, pragma_it, get_known_pragma_list() )
				msgout_context->print( "#pragma %s\n", (*pragma_it).c_str() );
		}
		if( task.m_dump_flags & pp::context::dump_flags_t::fl_dump_standards )
		{
			array_foreach( pp::standard_list_t::const_iterator, standard_it, m_context->get_standard_list() )
				msgout_context->print( "%s\n", standard_it->get_ident_string() );
		}
	}
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::dump_macro_stack( msgout::context & ctx )
{
	size_t arg_parsing_context_index = m_arg_parse_stack.size();

	if( 0 == arg_parsing_context_index && m_patch_callback_list.empty() )
		return STATUS_SUCCESS;

	ctx.print( "MACRO EXPANSION STACK\n" );

	// For all patches, dump them with intermediate argument parsing contexts
	for( size_t patch_index = m_patch_callback_list.size(); patch_index > 0; -- patch_index )
	{
		const patch_callback & patch = *m_patch_callback_list[patch_index - 1];

		// Dump intermediate argument parsing contexts
		while( arg_parsing_context_index > patch.m_arg_parse_context_top )
		{
			dump_arg_parsing_context( ctx, *m_arg_parse_stack[arg_parsing_context_index - 1] );
			-- arg_parsing_context_index;
		}

		// Dump patch
		dump_patch( ctx, patch );
	}

	// Dump the rest of argument parsing contexts
	while( arg_parsing_context_index > 0 )
	{
		dump_arg_parsing_context( ctx, *m_arg_parse_stack[arg_parsing_context_index - 1] );
		-- arg_parsing_context_index;
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::dump_patch( msgout::context & ctx, const patch_callback & patch )
{
	if( nullptr != patch.m_arg_list )
	{
		core::string buf;
		patch.m_arg_list->format( buf );
		ctx.print( "EXPN: %s( %s )\n", patch.m_macro->get_name(), buf.c_str() );
	}
	else
		ctx.print( "EXPN: %s\n", patch.m_macro->get_name() );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::dump_arg_parsing_context( msgout::context & ctx, const argument_parsing_context_t & arg )
{
	if( nullptr != arg.m_arg_list )
	{
		core::string buf;
		arg.m_arg_list->format( buf );
		ctx.print( "ARGS: %s( %s )\n", arg.m_macro->get_name(), buf.c_str() );
	}
	else
		ctx.print( "ARGS: %s\n", arg.m_macro->get_name() );

	// Exit
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Commands

f1::c_pp::parser::modify_c_flags_parser_command::modify_c_flags_parser_command( c_flags_t flags_to_set, c_flags_t flags_to_clear ):
	m_flags_to_set( flags_to_set ), m_flags_to_clear( flags_to_clear )
{
}
const status_t f1::c_pp::parser::modify_c_flags_parser_command::execute( pp::Iparser & parser )
{
	core::Tref<c_pp::parser> c_pp_par = type_cast<c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_STATE;

	c_pp_par->modify_c_flags( m_flags_to_set, m_flags_to_clear );

	return STATUS_SUCCESS;
}

f1::c_pp::parser::append_known_pragma_command::append_known_pragma_command( const core::const_string & ident, bool append ):
	m_ident( ident ), m_append( append )
{
}
const status_t f1::c_pp::parser::append_known_pragma_command::execute( pp::Iparser & parser )
{
	core::Tref<c_pp::parser> c_pp_par = type_cast<c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_STATE;

	if( m_append )
		c_pp_par->append_known_pragma( m_ident.data(), m_ident.length() );
	else
		c_pp_par->remove_known_pragma( m_ident.data(), m_ident.length() );

	return STATUS_SUCCESS;
}

f1::c_pp::parser::append_allowed_base_path_command::append_allowed_base_path_command( const core::const_string & path, bool append ):
	m_path( path ), m_append( append )
{
}
const status_t f1::c_pp::parser::append_allowed_base_path_command::execute( pp::Iparser & parser )
{
	core::Tref<c_pp::parser> c_pp_par = type_cast<c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_STATE;

	if( m_append )
		c_pp_par->append_allowed_base_dir( m_path );
	else
		c_pp_par->remove_allowed_base_dir( m_path );

	return STATUS_SUCCESS;
}
f1::c_pp::parser::append_disallowed_base_path_command::append_disallowed_base_path_command( const core::const_string & path, bool append ):
	m_path( path ), m_append( append )
{
}
const status_t f1::c_pp::parser::append_disallowed_base_path_command::execute( pp::Iparser & parser )
{
	core::Tref<c_pp::parser> c_pp_par = type_cast<c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_STATE;

	if( m_append )
		c_pp_par->append_disallowed_base_dir( m_path );
	else
		c_pp_par->remove_disallowed_base_dir( m_path );

	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Messages

const status_t f1::c_pp::parser::message( const char * file, f1::parser::lineno_t line, f1::msgout::msgid_t msg, va_list ap )
{
	status_t status;

	if( !m_pre_directive_can_exec && !m_can_exec )	// do not report anything when execution is disabled
		return STATUS_SUCCESS;

	if( super::message( file, line, msg, ap ).failed_status( status ) )
		return status;

	if( STATUS_WARNING_OPERATION_DISABLED == status )
		return status;

	if( msgout::severity_error == get_msgout_context().get_message_severity( msg ) && !m_subsection_stack.empty() )
	{
		core::Tconst_weak_ref<f1::parser::Isource>	src = nullptr;
		f1::parser::lineno_t	def_line = 0;

		const subsection_desc & desc = *m_subsection_stack.rbegin();
		if( nullptr != desc.m_macro )
		{
			def_line = desc.m_macro->get_definition_line();
			src = desc.m_macro->get_definition_source();
		}
		else if( nullptr != desc.m_frame )
		{
			def_line = desc.m_frame->get_initial_location().m_line;
			src = desc.m_frame->get_initial_location().m_source;
		}

		if( nullptr != src )
		{
			def_line += m_parser->get_patch_local_line();
			message( src->get_display_filename(), def_line, syntax::IMSG_INFO_LOCATED_HERE );
		}
	}

	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Execution stack

f1::pp::Iframe * f1::c_pp::parser::get_directive_frame()
{
	f1::pp::Iframe * frm = super::get_directive_frame();
	if( nullptr == frm || 0 == frm->get_flags( pp::Iframe::fl_initially_disabled ) )
		m_can_exec = true;
	return frm;
}
f1::pp::Iframe * f1::c_pp::parser::get_directive_frame( const f1::guid_t & openning_guid )
{
	f1::pp::Iframe * frm = super::get_directive_frame( openning_guid );
	if( 0 == frm->get_flags( pp::Iframe::fl_initially_disabled ) )
		m_can_exec = true;
	return frm;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Subsections

const status_t f1::c_pp::parser::begin_subsection( const pp::Imacro & mac ) noexcept
{
	__debugbreak_if( nullptr == m_output )
		return STATUS_ERROR_NOT_INITIALIZED;

	subsection_desc * desc = m_subsection_stack.append_new();
	desc->m_macro = &mac;

	return m_output->begin_subsection( mac.get_name() );
}
const status_t f1::c_pp::parser::begin_subsection( const pp::Iframe & frm, const pp::string & ident ) noexcept
{
	__debugbreak_if( nullptr == m_output )
		return STATUS_ERROR_NOT_INITIALIZED;

	subsection_desc * desc = m_subsection_stack.append_new();
	desc->m_frame = &frm;

	return m_output->begin_subsection( ident );
}

const status_t f1::c_pp::parser::begin_subsection( const char * ident, size_t len ) noexcept
{
	__debugbreak_if( nullptr == m_output )
		return STATUS_ERROR_NOT_INITIALIZED;

	(void) m_subsection_stack.append_new();

	return m_output->begin_subsection( ident, len );
}
const status_t f1::c_pp::parser::begin_subsection( const core::static_string & ident ) noexcept
{
	__debugbreak_if( nullptr == m_output )
		return STATUS_ERROR_NOT_INITIALIZED;

	(void) m_subsection_stack.append_new();

	return m_output->begin_subsection( ident );
}
const status_t f1::c_pp::parser::begin_subsection( const pp::string & ident ) noexcept
{
	__debugbreak_if( nullptr == m_output )
		return STATUS_ERROR_NOT_INITIALIZED;

	(void) m_subsection_stack.append_new();

	return m_output->begin_subsection( ident );
}

const status_t f1::c_pp::parser::end_subsection() noexcept
{
	__debugbreak_if( nullptr == m_output )
		return STATUS_ERROR_NOT_INITIALIZED;
	__debugbreak_if( m_subsection_stack.empty() )
		return STATUS_ERROR_NOT_INITIALIZED;

	flush_chars();
	m_subsection_stack.pop_back();

	return m_output->end_subsection();
}

/*END OF parser.cxx*/
