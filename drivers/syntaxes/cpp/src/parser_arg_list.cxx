/*parser_arg_list.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::c_pp::parser::parse_arg( pp::string & arg,
	const char *& s, const char * end_s, int end_ch, unsigned flags )
{
	core::Tarray<int>	char_stack;
	pp::string inp;
	pp::string tmpbuf;
	int cur_end_ch;

	// Get arg end
	const char * start = s;
	for( ; s < end_s ; )
	{
		switch( *s )
		{
		case '\\':
			inp.add_part( start, s - start );
			++ s;
			if( !skip_newline( s, end_s ) && parse_esc_char( tmpbuf, s, end_s ).failed() )
				message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
			start = s;
			continue;
		case '\"':
		case '\'':
			cur_end_ch = *s ++;
			skip_string( s, end_s, cur_end_ch, nullptr );
			continue;
		case '(':
			char_stack.push_back( ')' );
			break;
		case ')':
			if( char_stack.empty() )
				goto done;
			char_stack.pop_back();
			break;
		case ',':
			if( char_stack.empty() )
				goto done;
			break;
		case '\n': case'\r':
			if( !(flags & fl_parse_args_allow_newlines) && char_stack.empty() )
				goto done;
			break;
		default:
			if( ((('\n' == end_ch || '\r' == end_ch) && s >= end_s) || end_ch == *s) && char_stack.empty() )
				goto done;
			break;
		}
		++ s;
	}
 done:
	inp.add_part( start, s - start );
	if( !char_stack.empty() )
		return message( syntax::IMSG_ERROR_UNEXPECTED_MACRO_END );

	// Add result
	trim_spaces_and_comments( inp, true );
	arg.add_part_alloc( inp );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::parse_arg_list( pp::arg_list & args,
	const pp::param_list * params, const char *& s, const char * end_s, int end_ch, unsigned flags )
{
	status_t status;

	// Parse open bracket
	// Note that we don't skip spaces before
	if( 0 == end_ch )
	{
		end_ch = get_end_bracket( *s );
		if( 0 == end_ch )
			return message( syntax::IMSG_ERROR_EXPECTED_S, "'('" );
		++ s;
	}
	const char * const start_arg = s;

	// Parse all arguments
	for(;;)
	{
		// Check for eof
		//skip_real_spaces_and_comments( s, end_s );
		if( s >= end_s )
			return message( syntax::IMSG_ERROR_UNEXPECTED_EOL );

		// Parse argument
		pp::string value;
		if( parse_arg( value, s, end_s, end_ch, flags ).failed_status( status ) )
			return status;

		__assert( s >= end_s || ',' == *s || '\n' == end_ch && ('\n' == *s || '\r' == *s || s >= end_s) || end_ch == *s );

		// Expand macros
		pp::string expanded_value;
		if( expand_macros( expanded_value, value ).failed_status( status ) )
			return status;

		// Add new argument
		args.append_expanded_arg( value, expanded_value );

		// Parse separator
		if( end_ch == *s )
			break;
		if( '\n' == end_ch && ('\n' == *s || '\r' == *s || s >= end_s) )
			break;
		if( ',' != *s )
			return message( syntax::IMSG_ERROR_EXPECTED_S, "',' or ')'" );
		++ s;
	}
	__assert( '\n' == end_ch && ('\n' == *s || '\r' == *s || s >= end_s) || end_ch == *s );
	const char * const end_arg = s;
	++ s;

	// Now check for single empty argument ()
	// This is required when macro takes one parameter and only a pair of empty parenthesis was given
	if( nullptr != params )
	{
		if( args.get_arg_list().empty() && 1 == params->get_param_list().size() )
		{
			args.append_arg( pp::string( start_arg, end_arg - start_arg ) );
		}
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::parse_arg_list( core::Tref<pp::arg_list> & args,
	const pp::param_list * params, const char *& s, const char * end_s, int end_ch, unsigned flags )
{
	status_t status;

	// Create argument list object
	core::Tref<pp::arg_list> al;
	al.createT<pp::arg_list>();
	al->initialize();

	// Parse argument list
	if( parse_arg_list( *al, params, s, end_s, end_ch, flags ).failed_status( status ) )
		return status;

	// Exit
	args = al;
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::parse_arg_list( pp::arg_list & args,
	const pp::param_list * params, int end_ch, unsigned flags )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source

	status_t status = parse_arg_list( args, params, s, end_s, end_ch, flags );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}
const status_t f1::c_pp::parser::parse_arg_list( core::Tref<pp::arg_list> & args, const pp::param_list * params, int end_ch, unsigned flags )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source

	status_t status = parse_arg_list( args, params, s, end_s, end_ch, flags );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}

/*END OF parser_arg_list.cxx*/
