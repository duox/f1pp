/*parser_param_list.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::c_pp::parser::parse_param_list( pp::param_list & params, int end_ch, unsigned flags )
{
	status_t status;
	status_t exit_status = STATUS_SUCCESS;
	bool was_vararg = false;
	bool do_not_add_param = false;
	char end_seq[4] = "\n\r\0";
	bool good = false;

	if( 0 == end_ch )
	{
		end_ch = get_end_bracket( m_parser->peek_char() );
		if( 0 == end_ch )
		{
			if( 0 == (flags & fl_parse_param_list_silent) )
				status = message( syntax::IMSG_ERROR_EXPECTED_S, "'('" );
			return status;
		}
		m_parser->skip_char();
	}
	end_seq[2] = char(end_ch);

	skip_real_spaces_and_comments();
	if( m_parser->skip_char( end_ch ) )
		return STATUS_SUCCESS;

	while( !m_parser->is_eof() )
	{
		if( was_vararg )
		{
			if( !do_not_add_param )
			{
				do_not_add_param = true;	// bad vararg
				if( 0 == (flags & fl_parse_param_list_silent) )
					message( syntax::IMSG_ERROR_EXPECTED_S_AFTER_S, end_seq + 2, "..." );
			}
		}

		if( '.' == m_parser->get_ptr()[0] && '.' == m_parser->get_ptr()[1] && '.' == m_parser->get_ptr()[2] )
		{
			m_parser->skip_chars( 3 );
			if( !do_not_add_param )
				params.append_param( pp::param_list::fl_vararg, nullcstr );
			was_vararg = true;
		}
		else if( is_ident_first_char( *m_parser->get_ptr() ) )
		{
			pp::string ident;
			if( parse_definition_identifier( ident ).failed_status( status ) )
				return status;

			unsigned flags = 0;
			skip_real_spaces_and_comments();
			if( '.' == m_parser->get_ptr()[0] && '.' == m_parser->get_ptr()[1] && '.' == m_parser->get_ptr()[2] )
			{
				was_vararg = true;
				m_parser->skip_chars( 3 );
				flags = pp::param_list::fl_vararg;
			}

			if( !do_not_add_param )
				params.append_param( flags, ident.get_stringslice() );
		}
		else
		{
			if( 0 == (flags & fl_parse_param_list_silent) )
				message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
			exit_status = STATUS_ERROR_INVALID_SYNTAX;
			do_not_add_param = true;
		}

		skip_real_spaces_and_comments();
		if( ('\n' == end_ch && m_parser->is_nl()) || m_parser->skip_char( end_ch ) )
		{
			good = true;
			break;
		}
		if( !m_parser->skip_char( ',' ) )
		{
			status = on_invalid_param( flags );
			if( status.failed() && exit_status.succeeded() )
				exit_status = status;
			if( m_parser->peek_one_of_chars( end_seq ) )
				break;
			if( exit_status.failed() )
				break;
		}
		skip_real_spaces_and_comments();
	}
	if( !good && exit_status.succeeded() )	// do not output any more messages if args are already considered incorrect
	{
		if( 0 == (flags & fl_parse_param_list_silent) )
			message( syntax::IMSG_ERROR_EXPECTED_S, end_seq + 2 );
		exit_status = STATUS_ERROR_INVALID_SYNTAX;
	}

	return exit_status;
}
const status_t f1::c_pp::parser::parse_param_list( core::Tref<pp::param_list> & params, int end_ch, unsigned flags )
{
	status_t status;

	core::Tref<pp::param_list> obj;
	if( obj.createT<pp::param_list>().failed_status( status ) )
		return status;

	if( parse_param_list( *obj, end_ch, flags ).failed_status( status ) )
		return status;

	params = obj;
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::on_invalid_param( unsigned flags )
{
	if( 0 == (flags & fl_parse_param_list_silent) )
		message( syntax::IMSG_ERROR_EXPECTED_S_OR_S, "','", "')'" );

	while( !m_parser->is_eof() )
	{
		if( m_parser->skip_char( ',' ) || m_parser->peek_one_of_chars( ")\n\r" ) )
			break;
		m_parser->skip_char();
	}

	return STATUS_ERROR_INVALID_SYNTAX;
}

/*END OF parser_param_list.cxx*/
