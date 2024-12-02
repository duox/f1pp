/*switch_message.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_message::switch_message( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_message::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_message::get_long_name() const NOEXCEPT
{
	return "message";
}
const char * f1::pp::frontend::f1pp::switch_message::get_desc() const NOEXCEPT
{
	return "Set message range state";
}
/*const char * f1::pp::frontend::f1pp::switch_message::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_message::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_message::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	// Create command object
	core::Tref<cmd_message_group_state> cmd;
	cmd = new(std::nothrow) cmd_message_group_state;
	if( nullptr == cmd )
		return STATUS_ERROR_OBJECT_CREATION;

	// Parse the rest of the switch
	const char * s = rest.data();
	const char * end_s = rest.data_end();
	while( s < end_s )
	{
		// Create new range
		cmd_message_group_state::message_range_desc * range = cmd->m_message_ranges.append_new();
		__debugbreak_if( nullptr == range )
			return STATUS_SUCCESS;
		range->m_flags = 0;
		range->m_severity = msgout::severity_none;

		// Parse range
		if( isdigit( *s ) )
		{
			unsigned long ul = _strntoul( s, end_s - s, &s, 0 );
			range->m_first_message = msgout::msgid_t(ul);
			if( '-' != *s )
			{
				range->m_count = 1;
			}
			else
			{
				++ s;
				if( !isdigit( *s ) )
				{
					printf( "--%s: expected numeric after '-'", name );
					return STATUS_SUCCESS;
				}

				ul = _strntoul( s, end_s - s, &s, 0 );
				if( ul >= range->m_first_message )
					range->m_count = ul - range->m_first_message + 1;
				else
				{
					range->m_count = range->m_first_message - ul + 1;
					range->m_first_message = msgout::msgid_t(ul);
				}
			}
		}
		else
		{
			printf( "--%s: expected numeric", name );
			return STATUS_SUCCESS;	// 'all', 'all-warnings' etc not yet supported
		}

		// If it's an enumerator, just continue parsing
		if( ',' != *s )
			break;
		++ s;
	}

	// If it's a colon, read flags
	if( s >= end_s || ':' != *s )
	{
		// Otherwise, it's an error
		printf( "--%s: expected ':'", name );
		return STATUS_SUCCESS;
	}
	++ s;

	// Gather all state changes
	unsigned flags = 0;
	msgout::severity_t	severity = msgout::severity_none;
	for(;;)
	{
		// Parse identifier
		if( !isalpha( *s ) )
			break;
		if( end_s - s >= 7 && !memicmp( s, "disable", 7 ) )
		{
			flags |= cmd_message_group_state::message_range_desc::fl_disable_message;
			s += 7;
		}
		else if( end_s - s >= 6 && !memicmp( s, "enable", 6 ) )
		{
			flags |= cmd_message_group_state::message_range_desc::fl_enable_message;
			s += 6;
		}
		else if( end_s - s >= 5 && !memicmp( s, "error", 5 ) )
		{
			severity = msgout::severity_error;
			s += 5;
		}
		else if( end_s - s >= 7 && !memicmp( s, "warning", 7 ) )
		{
			severity = msgout::severity_warning;
			s += 7;
		}
		else
		{
			printf( "--%s: unknown flag", name );
		}

		// Check status
		if( s >= end_s )
			break;
		if( ',' != *s )
		{
			printf( "--%s: expected ','", name );
			return STATUS_SUCCESS;
		}
		++ s;
	}

	// Apply state changes
	cmd_message_group_state::message_range_desc * range = cmd->m_message_ranges.data();
	cmd_message_group_state::message_range_desc * range_end = cmd->m_message_ranges.data_end();
	for( ; range < range_end; ++ range )
	{
		range->m_flags = flags;
		range->m_severity = severity;
	}

	// Done, add the command
	application::get_app()->get_context()->add_postinit_parser_command( *cmd );

	// Exit
	__unreferenced_parameter( switchar );
	__unreferenced_parameter( name );
	return STATUS_SUCCESS;
}

const status_t f1::pp::frontend::f1pp::switch_message::cmd_message_group_state::execute( pp::Iparser & parser )
{
	status_t status;
	msgout::context & msgout_context = parser.get_msgout_context();
	unsigned i = 0;

	const cmd_message_group_state::message_range_desc * range = m_message_ranges.data();
	const cmd_message_group_state::message_range_desc * range_end = m_message_ranges.data_end();
	for( ; range < range_end; ++ range )
	{
		if( 0 != range->m_flags )
		{
			if( range->m_flags & message_range_desc::fl_disable_message )
			{
				status = msgout_context.enable_all_messages(
					range->m_first_message,
					range->m_first_message + range->m_count - 1,
					false
					);
			}
			else if( range->m_flags & message_range_desc::fl_enable_message )
			{
				status = msgout_context.enable_all_messages(
					range->m_first_message,
					range->m_first_message + range->m_count - 1,
					true
					);
			}
		}
		if( status.succeeded() && msgout::severity_none != range->m_severity )
		{
			for( ; i < range->m_count; ++ i )
			{
				status = msgout_context.set_message_severity(
					range->m_first_message + i,
					range->m_severity
					);
				if( status.failed() )
					break;
			}
			i = 0;
		}

		if( status.failed() )
		{
			if( 1 == range->m_count )
				printf( "--message: message id was not found: %d.\n",
					range->m_first_message );
			else
				printf( "--message: message id range was not found: %d..%d.\n",
					range->m_first_message + i, range->m_first_message + range->m_count - i - 1 );
		}
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF switch_message.cxx*/
