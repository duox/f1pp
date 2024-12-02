/*parser_macros.cxx*/
/**
 * @brief Macros-related stuff.
 */
#include "pch.h"
#include "main.h"

/*const status_t f1::c_pp::parser::expand_macros( pp::string & buf, const pp::string & src )
{
	status_t status;
	pp::macro_list_t macro_list;
	size_t macro_count = 0;
	bool order01 = true;
	pp::string buf0, buf1;
	pp::string * dst_buf = &buf0;
	const pp::string * src_buf = &src;

	// Run expansions over and over again until there were not expansions left
	for(;;)
	{
		if( expand_macros( *dst_buf, *src_buf, macro_list ).failed_status( status ) )
			return status;
		if( macro_list.size() == macro_count )
			break;
		macro_count = macro_list.size();

		order01 = !order01;
		if( order01 )
			dst_buf = &buf0,
			src_buf = &buf1;
		else
			dst_buf = &buf1,
			src_buf = &buf0;
		dst_buf->clear();
	}

	// Done, store data
	buf.move( *dst_buf );

	array_foreach( pp::macro_list_t::iterator, it, macro_list )
		it->modify_flags( 0, pp::Imacro::macro_flags_t::fl_being_expanded );

	// Exit
	return status;
}*/
const status_t f1::c_pp::parser::expand_macros( pp::string & buf, const pp::string & src )//, pp::macro_list_t & macro_list )
{
	const char * s = src.get_string().c_str();
	const char * end_s = s + src.get_string().size();
	pp::string ident;
	status_t status;
	core::Tarray<int>	char_stack;
//	const size_t macro_list_size = macro_list.size();

	const char * start = s;
	while( s < end_s )
	{
		if( '\"' == *s || '\'' == *s )
		{
			const int end_ch = *s++;
			skip_string( s, end_s, end_ch, nullptr );
		}
		else if( is_ident_first_char( *s ) )
		{
			// Parse identifier
			const char * mac_start = s;
			parse_macro_identifier( ident, s, end_s );
			__assert( !ident.is_empty() );

			// Try to find macro
			core::Tref<pp::Imacro> mac;
			if( find_macro( mac, ident.get_stringslice() ).failed() )
				continue;	// not a macro
//			core::Tref<c_pp::macro> mac_cpp = mac.dynamic_pointer_cast<c_pp::macro>();
			if( mac->get_flags( pp::Imacro::macro_flags_t::fl_being_expanded | f1::pp::Imacro::macro_flags_t::fl_recursive_locked ) )
			{
//				if( nullptr != mac_cpp )
//					mac_cpp->set_recursive_expansion_nesting( m_macro_expansion_nesting );
				if( mac->get_flags( pp::Imacro::macro_flags_t::fl_being_expanded ) )
					message( syntax::IMSG_WARNING_RECURSIVE_MACRO_CALL_S, ident.get_string().c_str() );
				continue;	// macro is in process of expansion, disallow recursive inclusions
			}
/*			if( nullptr != mac_cpp && 0 != mac_cpp->get_recursive_expansion_nesting() )
			{
				if( m_macro_expansion_nesting >= mac_cpp->get_recursive_expansion_nesting() )
					mac_cpp->set_recursive_expansion_nesting( 0 );
				else
					continue;
			}
*/
			if( start != mac_start )
			{
				buf.add_part( start, mac_start - start );
				start = mac_start;
			}

			// Parse argument list
			core::Tref<pp::arg_list> args;
			pp::arg_list arg_list;
			if( nullptr != mac->get_param_list() )
			{
				skip_real_spaces_and_comments( s, end_s );
				if( '(' != *s )
					continue;	// macro requires argument, but source code doesn't supplies them - it's not a macro

				if( parse_arg_list( arg_list, mac->get_param_list(), s, end_s ).failed_status( status ) )
					return status;
				args = &arg_list;
			}

			if( nullptr != args && 0 != args->get_arg_list().size() )
			{
				array_foreach( pp::arg_list::arg_desc_list_t::iterator, it, args->get_arg_list() )
				{
					pp::arg_list::arg_desc_t & arg = *it;
					if( expand_macros( arg.m_expanded_value, arg.m_value ).failed_status( status ) )
						return status;
				}
			}

			// Expand macro
			pp::string exp_buf;
			mac->modify_flags( pp::Imacro::macro_flags_t::fl_being_expanded );
			status = mac->expand( *this, exp_buf, args );
			if( status.failed() )
				return status;
			//pc->m_macro_list.append( mac );

			// Append text
			start = s;
			buf.add_part( exp_buf );
		}
		else
			++ s;
	}
	buf.add_part( start, s - start );

	// Block all macros
	/*pp::macro_list_t::iterator it( macro_list, macro_list_size );
	array_foreach_i( pp::macro_list_t::iterator, it, macro_list )
		it->modify_flags( pp::Imacro::macro_flags_t::fl_being_expanded );*/

	// Start new patch
//	m_parser->insert_patch( buf, pc );

	// Exit
	return STATUS_SUCCESS;
}

/*const status_t f1::c_pp::parser::expand_macros( core::string & buf )
{
	status_t status;

	buf.clear();

	// Parse identifier
	pp::string ident;
	m_parser->push_pos();
	if( parse_macro_identifier( ident ).failed_status( status ) )
		return status;

	// Find macro
	core::Tref<pp::Imacro> mac;
	if( m_context->find_macro( mac, ident.get_stringslice() ).failed_status( status ) )
	{
		m_parser->pop_pos();
		return status;
	}
	m_parser->pop_discard_pos();

	// Gather arguments if they are required
	pp::arg_list args;
	core::Tref<pp::arg_list> arg_ptr = nullptr;
	if( nullptr != mac->get_param_list() )
	{
		if( parse_arg_list( args ).failed_status( status ) )
			return status;
		arg_ptr = &args;
	}

	// Expand macro
	if( mac->expand( *this, buf, arg_ptr ).failed_status( status ) )
		return status;

	// Exit
	return STATUS_SUCCESS;
}*/

const status_t f1::c_pp::parser::expand_macro( pp::Imacro & mac, const pp::arg_list * args )
{
	status_t status;

	// Mark macro as being expanded to prevent recursive expansions
	__assert( 0 == mac.get_flags( f1::pp::Imacro::macro_flags_t::fl_being_expanded ) );
	mac.modify_flags( pp::Imacro::macro_flags_t::fl_being_expanded );

	// If macro requires an argument list, but can parse them itself
	pp::string buf;
	if( 0 != mac.get_flags( pp::Imacro::macro_flags_t::fl_self_parse_arglist ) )
		status = mac.parse( *this, buf );			// let macro parse the text
	else
		status = mac.expand( *this, buf, args );	// expand the macro as usual

	// Macro is expanded, now reparse the result
	if( status.succeeded() && !buf.empty() )
	{
		DBG_INFO_patch(( "expand_macro: begin patch\n" ));
#if __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__
		begin_subsection( mac.get_name() );
#endif // __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__

		patch_callback * cb = new(std::nothrow) patch_callback;
		m_patch_callback_list.append( cb );
		cb->m_macro = &mac;
		cb->m_arg_list = args;
		cb->m_saved_unexpanded_buffer_output.initialize( *get_context() );
		cb->m_saved_unexpanded_buffer_output.move( &m_unexpanded_buffer_output );
		cb->m_saved_allow_unexpanded_output = m_allow_args_separators;
		DBG_INFO_allow_unexpanded_output( "expand_macro: saving in patch_allback", m_allow_args_separators );
		cb->m_saved_c_flags = get_c_flags();
		//cb->m_saved_capturing_line = m_capturing_line;	m_capturing_line = false;
		m_capture_save_stack.push_back( m_capturing_line );	m_capturing_line = false;
		cb->m_arg_parse_context_top = m_arg_parse_stack.size();
		//cb->m_saved_args_parse_mode = m_args_parse_mode;

		if( !m_arg_parse_stack.empty() )
		{
			f1::c_pp::parser::argument_parsing_context_t & argparse = **m_arg_parse_stack.rbegin();
			cb->m_unlock_all = argparse.m_macro == &mac;
			if( cb->m_unlock_all  )
				cb->m_recursive_macros.move( argparse.m_recursive_macros );
			else
				cb->m_recursive_macros.append( argparse.m_recursive_macros );
		}

		m_parser->insert_patch( buf, cb, this );
		modify_c_flags( c_flags_t::fl_disable_comments );
		//m_args_parse_mode = 0;

		if( m_arg_parse_stack.empty() || nullptr == m_arg_parse_stack.back()->m_macro || 0 == m_arg_parse_stack.back()->m_macro->get_flags( pp::Imacro::macro_flags_t::fl_function ) )
			m_allow_args_separators = false;
		DBG_INFO_allow_unexpanded_output( "expand_macro: setting", m_allow_args_separators );
	}
	else
		mac.modify_flags( null, pp::Imacro::macro_flags_t::fl_being_expanded );

	// Exit
	return status;
}
void f1::c_pp::parser::patch_callback::on_patch_completed( f1::parser::Iparser & parser, void * param ) noexcept
{
	DBG_INFO_patch(( "expand_macro: patch completed\n" ));

//	core::Tref<c_pp::parser> cpppar = type_cast<c_pp::parser *>(m_macro->get_parser());
	core::Tref<c_pp::parser> cpppar = type_cast<c_pp::parser *>(reinterpret_cast<pp::Iparser *>(param));
	c_pp::parser * c_pp_par = cpppar;	// TODO: remove

	__debugbreak_if( nullptr == cpppar )
		return;

	// Reset all macro state:
	//	If we at the top level, all recurrent macros that have been blocked from expansion
	//	during reparse, now get unblocked.
	if( !m_recursive_macros.empty() )
	{
		if( m_unlock_all && !parser.is_patch_present() && cpppar->m_arg_parse_stack.empty() )
		{
			array_foreach( f1::pp::macro_list_t::iterator, it, m_recursive_macros )
			{
				__assert( null != it->get_flags( f1::pp::Imacro::macro_flags_t::fl_recursive_locked ) );
				it->modify_flags( null, f1::pp::Imacro::macro_flags_t::fl_recursive_locked );
			}
		}
		m_recursive_macros.clear();
	}

	// Finish expanding macro
	__assert( 0 != m_macro->get_flags( f1::pp::Imacro::macro_flags_t::fl_being_expanded ) );
	m_macro->modify_flags( null, f1::pp::Imacro::macro_flags_t::fl_being_expanded );

//	__assert( signed(c_pp_par->m_macro_expansion_nesting) > 0 );
//	c_pp_par->m_macro_expansion_nesting --;

	if( !c_pp_par->m_allow_args_separators )
	//if( 1 )//!c_pp_par->m_allow_args_separators )
	{
		c_pp_par->m_allow_args_separators = m_saved_allow_unexpanded_output;
		DBG_INFO_allow_unexpanded_output( "patch_callback::on_patch_completed: restoring", c_pp_par->m_allow_args_separators );
	}
	else
		DBG_INFO_allow_unexpanded_output( "patch_callback::on_patch_completed: not restoring", c_pp_par->m_allow_args_separators );
	c_pp_par->m_c_flags = m_saved_c_flags;
	//c_pp_par->m_capturing_line = m_saved_capturing_line;
	c_pp_par->m_capturing_line = c_pp_par->m_capture_save_stack.back();	c_pp_par->m_capture_save_stack.pop_back();
	//c_pp_par->m_args_parse_mode = m_saved_args_parse_mode;

	// Finish patching
	if( ')' == c_pp_par->m_args_parse_mode )	// if arguments are not being parsed by previous layer, merge buffers
	{
		//if( c_pp_par->m_unexpanded_buffer_output.get_buffer().is_empty() )
		if( !m_saved_unexpanded_buffer_output.get_buffer().is_empty() )
			c_pp_par->m_unexpanded_buffer_output.move( &m_saved_unexpanded_buffer_output );
	}
	__assert( this == *c_pp_par->m_patch_callback_list.rbegin() );
	c_pp_par->m_patch_callback_list.pop_back();
	delete this;	// TODO: use pools

	// Exit
	__unreferenced_parameter( parser );
}

const status_t f1::c_pp::parser::display_macro_location( const pp::Imacro & mac, unsigned msgid )
{
	core::Tconst_weak_ref<f1::parser::Isource> src = mac.get_definition_source();
	if( nullptr != src )
		message( 
			m_context->get_flags( pp::context::fl_msg_full_path ) ?
				m_full_current_file_name.c_str() : m_parser->get_current_file_name(),
			mac.get_definition_line(), msgid );
	else if( mac.get_flags( pp::Imacro::macro_flags_t::fl_predefined ) )
		message( syntax::IMSG_INFO_PREDEFINED );
	else
		message( syntax::IMSG_INFO_DEFINED_IN_COMMAND_LINE );
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::on_macro_expanded( pp::Imacro & mac )
{
	if( mac.get_flags( pp::Imacro::macro_flags_t::fl_deprecated ) )
	{
		const f1::pp::string & deprecation_text = mac.get_deprecation_text();
		if( deprecation_text.empty() )
			message( syntax::IMSG_WARNING_DEPRECATED_MACRO_S, mac.get_name() );
		else
			message( syntax::IMSG_WARNING_DEPRECATED_MACRO_S_S, mac.get_name(), deprecation_text.c_str() );
	}

	return super::on_macro_expanded( mac );
}

const f1::c_pp::parser::patch_callback * f1::c_pp::parser::get_current_patch_callback() const
{
	return 1 > m_patch_callback_list.size() ? nullptr : *m_patch_callback_list.rbegin();
}
const f1::c_pp::parser::patch_callback * f1::c_pp::parser::get_previous_patch_callback() const
{
	patch_callback_list_t::const_reverse_iterator it( m_patch_callback_list );
	if( !it.valid() )
		return nullptr;
	it.next();
	if( !it.valid() )
		return nullptr;
	return *it;
}
const f1::c_pp::parser::patch_callback_list_t & f1::c_pp::parser::get_argument_patch_callback_list() const
{
	return m_patch_callback_list;
}

/*const f1::c_pp::parser::argument_parsing_context_t * f1::c_pp::parser::get_current_macro_expansion() const
{
	return 1 > m_arg_parse_stack.size() ? nullptr : *m_arg_parse_stack.rbegin();
}
const f1::c_pp::parser::argument_parsing_context_t * f1::c_pp::parser::get_previous_macro_expansion() const
{
	argument_parsing_context_list_t::const_reverse_iterator it( m_arg_parse_stack );
	if( !it.valid() )
		return nullptr;
	it.next();
	if( !it.valid() )
		return nullptr;
	return *it;
}
const f1::c_pp::parser::argument_parsing_context_list_t & f1::c_pp::parser::get_argument_parsing_context_list() const
{
	return m_arg_parse_stack;
}*/

/*const f1::pp::Imacro * f1::c_pp::parser::get_currently_expanding_macro() const
{
	if( 1 > m_arg_parse_stack.size() )
		return nullptr;
	argument_parsing_context_t* desc = *m_arg_parse_stack.rbegin();
	return m_macro;
}
const f1::pp::Imacro * f1::c_pp::parser::get_previously_expanding_macro() const;
{
	if( 1 > m_arg_parse_stack.size() )
		return nullptr;
	argument_parsing_context_t* desc = *m_arg_parse_stack.rbegin();
	return m_macro;
}*/

/*END OF parser_macros.cxx*/
