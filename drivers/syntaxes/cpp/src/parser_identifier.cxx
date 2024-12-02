/*parser_identifier.cxx*/
#include "pch.h"
#include "main.h"

bool f1::c_pp::parser::is_ident_first_char( int c ) const
{
	if( '$' == c )
	{
		if( m_c_flags & c_flags_t::fl_dollar_sign_in_idents )
		{
			const_cast<self *>(this)->message( syntax::IMSG_WARNING_NONSTD_CHAR_IN_IDENTIFIER, '$' );	// TODO
			return true;
		}
		return false;
	}
	if( '@' == c )
	{
		if( m_c_flags & c_flags_t::fl_at_sign_in_idents )
		{
			const_cast<self *>(this)->message( syntax::IMSG_WARNING_NONSTD_CHAR_IN_IDENTIFIER, '@' );	// TODO
			return true;
		}
		return false;
	}
	return '_' == c || isalpha( c );
}
bool f1::c_pp::parser::is_ident_char( int c ) const
{
	return '_' == c || isalnum( c );
}

bool f1::c_pp::parser::skip_definition_identifier( const char * string, const char *& s, const char * end_s )
{
	if( s >= end_s )
		return false;
	if( !is_ident_first_char( *s ) )
		return false;

	bool res = true;
	const char * start = s;
	const char * pos = string;
	for( ; s < end_s; ++ s )
	{
		if( !is_ident_char( *s ) )
		{
			if( '\\' != *s )
				break;
			if( '\n' != s[1] && '\r' != s[1] )
				break;
				
			++ s;
			__assert( false == m_disable_line_update );
			m_disable_line_update = true;
			skip_newline( s, end_s );
			m_disable_line_update = false;
			skip_real_spaces_and_comments( s, end_s );	// skip whitespace in the start of the next line
		}
		else
		{
			if( '\0' == *pos )
				res = false;
			else if( *pos != *s )
				res = false;
			else
				++ pos;
		}
	}

	if( !res )
		s = start;
	return res;
}
bool f1::c_pp::parser::parse_definition_identifier( pp::string & buf, const char *& s, const char * end_s, f1::parser::Iparser * parser )
{
	const char * start = s;

	buf.clear();

	if( s >= end_s )
		return false;
	if( !is_ident_first_char( *s ) )
		return false;

	for( ++ s; s < end_s; ++ s )
	{
		if( !is_ident_char( *s ) )
		{
			buf.add_part( start, s - start );
			start = s;
			if( '\\' == *s )
			{
				if( '\n' != s[1] && '\r' != s[1] )
					break;
				
				++ s;
				__assert( false == m_disable_line_update );
				m_disable_line_update = true;
				skip_newline( s, end_s, parser );
				m_disable_line_update = false;

				skip_real_spaces_and_comments( s, end_s );	// skip whitespace in the start of the next line

				start = s;
				continue;
			}
			break;
		}
	}
	buf.add_part( start, s - start );

	return true;
}
const status_t f1::c_pp::parser::parse_definition_identifier( pp::string & buf, f1::parser::Iparser & parser )
{
	const char * s = parser.get_ptr();
	const char * const end_s = m_parser->get_end_ptr();				// capture end pointer for the current patch or source
	if( parse_definition_identifier( buf, s, end_s, &parser ) && s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		parser.set_ptr( s );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::parse_definition_identifier( pp::string & buf )
{
	return parse_definition_identifier( buf, *m_parser );
}


bool f1::c_pp::parser::parse_macro_identifier( pp::string & buf, const char *& s, const char * end_s, f1::parser::Iparser * parser )
{
	const char * sep = nullcstr;
	const char * last_s = s;
	pp::string prev;
	for(;;)
	{
		pp::string temp;
		if( !parse_definition_identifier( temp, s, end_s, parser ) )
			break;

		if( nullcstr != sep )
		{
			buf.add_part_move( prev );
			buf.add_part( '.' == *sep ? "." : "::" );
		}
		prev.move( temp );

		last_s = s;
		skip_real_spaces_and_comments( s, end_s );
		if( s >= end_s )
			break;

		sep = s;
		if( '.' == *s )
			++ s;
		else if( s + 1 < end_s && ':' == s[0] && ':' == s[1] )
			s += 2;
		else
			break;
	}
	buf.add_part_move( prev );
	s = last_s;

	return !buf.is_empty();
}
// TODO: an identifier can be split among several files
const status_t f1::c_pp::parser::parse_macro_identifier( pp::string & buf, f1::parser::Iparser & parser )
{
	const char * s = parser.get_ptr();
	const char * const end_s = m_parser->get_end_ptr();				// capture end pointer for the current patch or source
	if( parse_macro_identifier( buf, s, end_s, &parser ) && s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		parser.set_ptr( s );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::parse_macro_identifier( pp::string & buf )
{
	return parse_macro_identifier( buf, *m_parser );
}
const status_t f1::c_pp::parser::on_unknown_macro_identifier( core::Tconst_ref<pp::Imacro> & res, const core::stringslice & ident ) const
{
	// TODO: speed the search up
//	const pp::Istandard * standard = pp::service::get().find_item_standard( pp::Imacro::m_item_class_guid, pp::string(ident) );
//	if( nullptr != standard )
//		const_cast<self *>(this)->message( syntax::IMSG_WARNING_NOT_AVAILABLE_STANDARD_SS, pp::string(ident).c_str(), standard->get_description_string() );

	return super::on_unknown_macro_identifier( res, ident );
}

bool f1::c_pp::parser::skip_directive_identifier( const char * string, const char *& s, const char * end_s )
{
	return skip_definition_identifier( string, s, end_s );
}
bool f1::c_pp::parser::parse_directive_identifier( pp::string & buf, const char *& s, const char * end_s, f1::parser::Iparser * parser )
{
	return parse_definition_identifier( buf, s, end_s, parser );
}
const status_t f1::c_pp::parser::parse_directive_identifier( pp::string & buf, f1::parser::Iparser & parser )
{
	const char * s = parser.get_ptr();
	const char * const end_s = m_parser->get_end_ptr();				// capture end pointer for the current patch or source
	if( parse_definition_identifier( buf, s, end_s, &parser ) && s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		parser.set_ptr( s );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::parse_directive_identifier( pp::string & buf )
{
	return parse_definition_identifier( buf, *m_parser );
}
/*const status_t f1::c_pp::parser::on_unknown_directive_identifier( core::Tconst_ref<pp::Idirective> & res, const core::stringslice & ident ) const
{
	// TODO: speed the search up
//	const pp::Istandard * standard = pp::service::get().find_item_standard( pp::Idirective::m_item_class_guid, pp::string(ident) );
//	if( nullptr != standard )
//		const_cast<self *>(this)->message( syntax::IMSG_WARNING_NOT_AVAILABLE_STANDARD_SS, pp::string(ident).c_str(), standard->get_description_string() );

	return super::on_unknown_directive_identifier( res, ident );
}*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::parser::parse_identifier()
{
	core::Tref<pp::Imacro> mac;
	status_t status;
	pp::string buf;

	if( m_c_flags & c_flags_t::fl_do_not_expand_macros || !m_can_exec )
	{
		start_char_seq();

		pp::string ident;
		parse_macro_identifier( ident );
		return STATUS_INFORMATION_NOT_PROCESSED;
	}

	flush_chars();

	// Get current mode
	int args_parse_mode = 0;
	f1::c_pp::parser::argument_parsing_context_t * argparse = nullptr;
	if( !m_arg_parse_stack.empty() && ')' == m_args_parse_mode )
	{
		argparse = *m_arg_parse_stack.rbegin();
		args_parse_mode = m_args_parse_mode;	//argparse->m_previous_args_parse_mode;
	}

	// Parse identifier
	pp::string ident;
	if( parse_macro_identifier( ident ).failed_status( status ) )
		return status;

	// Find macro
	status = find_macro( mac, ident.get_stringslice() );
	__assert( status.failed() || nullptr != mac );

	// Check whether the macro can be expanded
	bool can_expand = true;
	if( nullptr == mac )
		can_expand = false;
	else
	{
		if( 0 == args_parse_mode && 0 != mac->get_flags( pp::Imacro::macro_flags_t::fl_recursive_locked ) )
			can_expand = false;
		else
			can_expand = 0 == mac->get_flags( pp::Imacro::macro_flags_t::fl_do_not_expand | pp::Imacro::macro_flags_t::fl_being_expanded | pp::Imacro::macro_flags_t::fl_disabled | pp::Imacro::macro_flags_t::fl_recursive_locked );
	}

	if( !can_expand )
//	if( nullptr == mac || 0 != mac->get_flags( pp::Imacro::macro_flags_t::fl_do_not_expand | pp::Imacro::macro_flags_t::fl_being_expanded | pp::Imacro::macro_flags_t::fl_disabled | pp::Imacro::macro_flags_t::fl_recursive_locked ) )
	{
		// A disabled macro
		if( nullptr != mac )
		{
			mac->on_macro_referenced();

			// Check for recursive expansion
			if( pp::Imacro::macro_flags_t::fl_being_expanded == mac->get_flags( pp::Imacro::macro_flags_t::fl_being_expanded | pp::Imacro::macro_flags_t::fl_recursive_locked ) )
			{
				// If macro arguments are currently being expanded for some macro at some level above,
				// add recurrent macro to the list of recurrent macros
				if( nullptr != argparse )//&& ')' == args_parse_mode )
				{
					argparse->m_recursive_macros.append_unique( mac );
					mac->modify_flags( pp::Imacro::macro_flags_t::fl_recursive_locked );
				}
			}
		}

		// Not a macro to expand - place normalized identifier to output
		if( output( ident.data(), ident.size() ).failed_status( status ) )
			return status;
		return STATUS_SUCCESS;
	}

	// If macro doesn't need argument list or can parse it itself
	if( 0 != mac->get_flags( pp::Imacro::macro_flags_t::fl_self_parse_arglist ) || nullptr == mac->get_param_list() )
	{
		output_unexpanded( ident );
		//begin_arg_parse( ident, *mac );
		expand_macro( *mac );
		//end_arg_parse();
	}
	else
	{
		// Otherwise, skip the argument list and process it only when closing ')' is found
		// This implies that any nested macro call is skipped as well and only top macro invocation
		// serves as a invocation marker.
		begin_arg_parse( ident, *mac );

		// Output macro identifier to unexpanded buffer
//		if( output_unexpanded( ident ).failed_status( status ) )
//			return status;
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF parser_identifier.cxx*/
