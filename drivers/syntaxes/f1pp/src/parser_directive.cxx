/*parser_directive.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::f1pp::parser::parse_directive()
{
	status_t status;

	// Check current state
	m_directive_attribute_list = nullptr;

	// Skip spaces and comments after the escape character
	skip_real_spaces_and_comments();
	if( m_parser->is_nl() )
		return STATUS_SUCCESS;	// empty directive

	// Check for shi-bang
	if( m_parser->peek_char( '!' ) )
	{
		skip_to_newline();
		return STATUS_SUCCESS;
	}

	// Try to parse attribute list
	if( m_parser->peek_char( '[' ) )
	{
		// Parse ttribute list
		if( parse_attribute_list( m_directive_attribute_list ).failed_status( status ) )
			return status;

		// Process some predefined attributes
		core::Tref<pp::attribute> attr;
		bool has_warn, has_message = false;
		has_warn = m_directive_attribute_list->find_attribute( attr, pp::string("warn") ).succeeded();
		if( !has_warn )
			has_message = m_directive_attribute_list->find_attribute( attr, pp::string("message") ).succeeded();
		if( nullptr != attr && (has_warn || has_message) )
		{
			core::Tconst_ref<pp::arg_list> args = attr->get_arg_list();
			if( nullptr == args || args->get_arg_list().empty() )
				message( syntax::IMSG_ERROR_EXPECTED_ARG_LIST );
			else
			{
				if( 1 != args->get_arg_list().size() )
					message( syntax::IMSG_WARNING_TOO_MANY_ARGUMENTS_S, has_warn ? "warn" : "message" );

				const pp::arg_list::arg_desc_t & arg = *args->get_arg_list().begin();
				pp::string val;
				unstringify_chars( val, arg.m_expanded_value.empty() ? arg.m_expanded_value : arg.m_value );
				message( has_warn ? c_pp::syntax::IMSG_WARNING_USER : syntax::IMSG_RAW, val.c_str() );
			}
		}
	}

	// Parse label if ':' is found
	skip_real_spaces_and_comments();
	if( m_parser->skip_char( ':' ) )
	{
		// Parse label name
		skip_real_spaces_and_comments();
		pp::string ident;
		parse_definition_identifier( ident );
		if( ident.is_empty() )
		{
			message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
			return STATUS_SUCCESS;
		}

		// Check for duplicate label
		core::Tconst_ref<label> lab;
		if( find_label( lab, ident ).succeeded() )
		{
			f1::parser::Iparser::position pos;
			m_parser->get_pos( pos );
			if( pos != lab->get_position() )
			{
				message( f1pp::syntax::IMSG_ERROR_LABEL_ALREADY_DEFINED_S );
				return STATUS_SUCCESS;
			}
		}

		// Create label at the current position
		if( create_label( lab, ident ).failed() )
		{
			message( f1pp::syntax::IMSG_ERROR_LABEL_ALREADY_DEFINED_S );
			return STATUS_SUCCESS;
		}

		// Simulate begining of the line
		set_newline();
	}

	// Try to parse prefix operator; it it succeeds, parse expression then
	if( nullptr != parse_prefix_unary_operator() )
	{
		pp::variant result;
		return parse_expression( result );
	}

	// Exit
	return super::parse_directive();
}
const status_t f1::f1pp::parser::on_execute_directive( pp::Idirective & obj )
{
	status_t status = super::on_execute_directive( obj );
//	m_directive_attribute_list.remove_all_attributes();
	return status;
}
const status_t f1::f1pp::parser::on_unknown_directive( const pp::string & ident )
{
	// Check for bare assignment (without `set' directive name)
	const char * ptr = m_parser->get_ptr();
	skip_real_spaces_and_comments();
	if( get_assignment_directive().parse_assignment( *m_parser ) )
	{
		m_parser->set_ptr( m_directive_name_start );
		return m_assignment_directive->parse( *this );
	}
	m_parser->set_ptr( ptr );

	// Exit
	status_t status = super::on_unknown_directive( ident );
//	m_directive_attribute_list.remove_all_attributes();
	return status;
}
const status_t f1::f1pp::parser::output_directive( pp::Idirective & drective )
{
	pp::string buf;
	parse_to_newline( buf );

	output( "#", 1 );
	if( nullptr != m_directive_attribute_list && !m_directive_attribute_list->get_attribute_list().empty() )
		m_directive_attribute_list->output( *this );
	output( drective.get_name() );
	output( buf );

	return STATUS_SUCCESS;
}

f1::f1pp::assignment_directive & f1::f1pp::parser::get_assignment_directive()
{
	if( nullptr == m_assignment_directive )
	{
		core::Tref<assignment_directive> drct;
		find_directive<assignment_directive>( drct );
		m_assignment_directive = drct;
		__debugbreak_if( nullptr == m_assignment_directive )
		{
			drct.createT<assignment_directive>();	// create dummy object
			m_assignment_directive = (assignment_directive *) drct.dynamic_pointer_cast<assignment_directive>();
		}
	}
	return *m_assignment_directive;
}

/*END OF parser_directive.cxx*/
