/*switch_message_severity.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_message_severity::switch_message_severity( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_message_severity::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_message_severity::get_long_name() const NOEXCEPT
{
	return "message-severity";
}
const char * f1::pp::frontend::f1pp::switch_message_severity::get_desc() const NOEXCEPT
{
	return "Change message severity level";
}
/*const char * f1::pp::frontend::f1pp::switch_message_severity::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_message_severity::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_message_severity::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	const char * s = rest.data();
	const char * end_s = rest.data_end();

	// Parse message identifier
	const char * ep;
	unsigned msgid = _strntoul( s, end_s - s, &ep, 10 );
	if( ep == s )
	{
		application::g_app->fatal_error( _T("%c%s: expected message identifier after the switch name"), switchar, core::string(name).c_str() );
		return STATUS_SUCCESS;
	}
	s = ep;

	// Parse separator
	if( s >= end_s || ':' != *s )
	{
		application::g_app->fatal_error( _T("%c%s: expected ':' after message identifier"), switchar, core::string(name).c_str() );
		return STATUS_SUCCESS;
	}
	++ s;

	// Parse severity
	if( s >= end_s )
	{
		application::g_app->fatal_error( _T("%c%s: expected severity name after ':'"), switchar, core::string(name).c_str() );
		return STATUS_SUCCESS;
	}

	msgout::severity_t severity = msgout::service::get().get_severity_id( s, end_s - s );
	if( msgout::severity_invalid == severity )
	{
		application::g_app->fatal_error( _T("%c%s: invalid severity name: %s"), switchar, core::string(name).c_str(),
			core::string( s, end_s - s ).c_str()
			);
		return STATUS_SUCCESS;
	}

	// Add command
	application::get_app()->get_context()->add_postinit_parser_command( *new(std::nothrow) change_message_severity(
		msgid, severity ) );

	// Exit
	return STATUS_SUCCESS;
}
f1::pp::frontend::f1pp::switch_message_severity::change_message_severity::change_message_severity( unsigned msgid, msgout::severity_t severity ):
	m_msgid( msgid ), m_severity( severity )
{
}
const status_t f1::pp::frontend::f1pp::switch_message_severity::change_message_severity::execute( pp::Iparser & parser )
{
	msgout::context & msg = parser.get_msgout_context();

	return msg.set_message_severity( m_msgid, m_severity );
}

/*END OF switch_message_severity.cxx*/
