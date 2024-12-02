/*switch_export_messages.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_export_messages::switch_export_messages( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_export_messages::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_export_messages::get_long_name() const NOEXCEPT
{
	return "export-messages";
}
const char * f1::pp::frontend::f1pp::switch_export_messages::get_desc() const NOEXCEPT
{
	return "Export messages to a message file";
}
/*const char * f1::pp::frontend::f1pp::switch_export_messages::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_export_messages::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_export_messages::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
#if 1
	application::get_app()->get_context()->add_postinit_parser_command( *new(std::nothrow) export_messages_command(
		rest.data(), rest.data_end() ) );
#else
	core::string m_export_file_name( rest.data(), rest.length() );
	if( m_export_file_name.empty() )
	{
		application::g_app->fatal_error( _T("%c%s: expected file name after the switch name"), switchar, core::string(name).c_str() );
		return STATUS_SUCCESS;
	}

	msgout::context & msgout_context = application::get_app()->get_context()->get_msgout_context();

	// Gather all identifiers
	core::Tarray<msgout::msgid_t> msg_list;
	array_foreach( msgout::source_list_t::const_iterator, it, msgout_context.get_source_list() )
	{
		it->get_msgid_list( msg_list );
	}

	// generate list of groups
	core::Tarray<msgout::Isource::group_def> group_list;
	msgout::Isource::group_def * group = group_list.append_new();
	group->m_msg_list.move( msg_list );

	// Write data to file
	msgout::source_xml source;
	status_t status = source.export_to_file( m_export_file_name.c_str(), group_list );
	if( status.failed() )
	{
		application::g_app->fatal_error( _T("%c%s: error exporting messages to %s"), switchar, core::string(name).c_str(), m_export_file_name.c_str() );
		return STATUS_SUCCESS;
	}
#endif

	// Exit
	__unreferenced_parameter( switchar );
	__unreferenced_parameter( name );
	return STATUS_SUCCESS;
}

#if 1
f1::pp::frontend::f1pp::switch_export_messages::export_messages_command::export_messages_command( const char * name, const char * name_end ):
	m_export_file_name( name, name_end - name )
{
}
const status_t f1::pp::frontend::f1pp::switch_export_messages::export_messages_command::execute( pp::Iparser & parser )
{
	msgout::context & msgout_context = parser.get_msgout_context();

	// Gather all identifiers
#if 1
	core::Tarray<msgout::msgid_t> msg_list;
	array_foreach( msgout::group_list_t::const_iterator, it, msgout_context.get_group_list() )
	{
		it->get_msgid_list( msg_list );
	}
#else
	core::Tarray<msgout::msgid_t> msg_list;
	array_foreach( msgout::source_list_t::const_iterator, it, msgout_context.get_source_list() )
	{
		it->get_msgid_list( msg_list );
	}
#endif
	// generate list of groups
	core::Tarray<msgout::Isource::group_def> group_list;
	msgout::Isource::group_def * group = group_list.append_new();
	group->m_msg_list.move( msg_list );

	// Write data to file
	msgout::source_xml source;	// TODO: use XML library directly
	source.export_to_file( m_export_file_name.c_str(), group_list );

	// Exit
	return STATUS_SUCCESS;
}
#endif
/*END OF switch_export_messages.cxx*/
