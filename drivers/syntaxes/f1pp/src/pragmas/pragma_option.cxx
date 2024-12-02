/*pragma_option.cxx*/
/** @file
 *
 * @brief Implementation of `#pragma option(opt, ...)'.
 *
 */
#include "../pch.h"
#include "../main.h"

f1::f1pp::pragma_option::pragma_option( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::f1pp::pragma_option::get_name() const
{
	return "option";
}
// {ADA0910E-BF1E-41EF-9F90-A65AA0A51D48}
const f1::guid_t f1::f1pp::pragma_option::m_guid =
{ 0xada0910e, 0xbf1e, 0x41ef, { 0x9f, 0x90, 0xa6, 0x5a, 0xa0, 0xa5, 0x1d, 0x48 } };
const f1::guid_t & f1::f1pp::pragma_option::get_guid() const
{
	return m_guid;
}

// Operations
const status_t f1::f1pp::pragma_option::parse( pp::Iparser & parser, const pp::string & param_str ) const
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	const char * s = param_str.data();
	const char * end_s = param_str.data_end();
	if( nullptr == s )
		return parser.message( syntax::IMSG_WARNING_EXPECTED_S, "'('" );

	// Parse opening parenthesis
	f1par->skip_real_spaces_and_comments( s, end_s );
	bool parenthesis = false;
	if( '(' == *s )
	{
		parenthesis = true;
		++ s;
	}

	// Parse command, if present ('push', 'pop' or 'delete')
	f1par->skip_real_spaces_and_comments( s, end_s );
	bool this_is_push;
	if( true == (this_is_push = f1par->skip_directive_identifier( "push", s, end_s )) ||
		f1par->skip_directive_identifier( "delete", s, end_s ) )
	{
		// Check whether it's '#pragma option(push) form
		f1par->skip_real_spaces_and_comments( s, end_s );
		if( s >= end_s || ')' == *s )
			status = this_is_push ?
				f1par->get_context()->push_all_options():
				f1par->get_context()->remove_all_options();
		else
		{
			core::Tarray<pp::string> ident_list;
			f1par->parse_identifier_list( ident_list, s, end_s );
			if( ident_list.empty() )
				;//status = parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER_LIST );
			else
				status = this_is_push ?
					parser.get_context()->push_options( ident_list ):
					parser.get_context()->remove_options( ident_list );
		}

	}
	else if( f1par->skip_directive_identifier( "pop", s, end_s ) )
		status = f1par->get_context()->pop_options();
	else
		status = parse_options( *f1par, s, end_s, parenthesis );

	// Parse closing parenthesis
	f1par->skip_real_spaces_and_comments( s, end_s );
	if( parenthesis )
	{
		if( ')' != *s )
			return parser.message( syntax::IMSG_WARNING_EXPECTED_S, "')'" );
		++ s;
	}

	// Check end of statement
	f1par->skip_real_spaces_and_comments( s, end_s );
	if( s < end_s )
		return parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::pragma_option::parse_options( f1pp::parser & f1par,
	const char *& s, const char * end_s, bool parenthesis ) const
{
	status_t status;

	// Parse all comma-separated options
	for(;;)
	{
		f1par.skip_real_spaces_and_comments( s, end_s );
		if( s >= end_s )
			break;
		if( parenthesis && ')' == *s )
			break;

		// Parse identifier
		pp::string ident;
		f1par.parse_definition_identifier( ident, s, end_s );
		if( ident.empty() )
			return f1par.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );

		// Parse assignment
		f1par.skip_real_spaces_and_comments( s, end_s );
		pp::variant result;
		if( '=' == *s )
		{
			++ s;

			// Determine expression length
			f1par.skip_real_spaces_and_comments( s, end_s );
			const char * option_start = s;
			f1par.skip_to_separator( s, end_s, ',', parenthesis ? ')' : 0 );

			// Parse expression if present
			if( option_start != s )
				status = f1par.parse_expression( result, option_start, s );
			else
				result.set_bool( true );
		}
		else
			result.set_bool( true );
		if( !result.is_valid() )
			break;

		// Set option value
		core::Tref<pp::Ioption> opt;
		f1par.get_context()->find_option( opt, ident );
		if( nullptr == opt )
		{
			status = f1par.get_context()->create_option( opt, result.get_type(), f1::guid_t::null, ident.data(), ident.length() );
			__debugbreak_if( nullptr == opt )
				return status;
		}
		opt->set_value( result );

		// Iterate
		if( s >= end_s || (parenthesis && ')' == *s) )
			break;
		++ s;
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_option.cxx*/
