/*switch_import_messages.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_import_messages::switch_import_messages( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_import_messages::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_import_messages::get_long_name() const NOEXCEPT
{
	return "import-messages";
}
const char * f1::pp::frontend::f1pp::switch_import_messages::get_desc() const NOEXCEPT
{
	return "Import messages from a message file";
}
/*const char * f1::pp::frontend::f1pp::switch_import_messages::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_import_messages::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_import_messages::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	application::get_app()->get_context()->add_preinit_parser_command( *new(std::nothrow) import_messages_command(
		rest.data(), rest.data_end() ) );

	__unreferenced_parameter( switchar );
	__unreferenced_parameter( name );
	return STATUS_SUCCESS;
}

f1::pp::frontend::f1pp::switch_import_messages::import_messages_command::import_messages_command( const char * name, const char * name_end ):
	m_import_file_name( name, name_end - name )
{
}
const status_t f1::pp::frontend::f1pp::switch_import_messages::import_messages_command::execute( pp::Iparser & parser )
{
	status_t status;
	msgout::context & msgout_context = parser.get_msgout_context();

	// Determine format by extension and create source
	core::Tref<msgout::source_memory> src;
	size_t ext_offset = m_import_file_name.find_extension();
	const char * ext = m_import_file_name.get_string().c_str() + ext_offset;
	if( !_stricmp( ext, ".ini" ) )
		src.createT<msgout::source_ini>();
	else if( !_stricmp( ext, ".xml" ) )
		src.createT<msgout::source_xml>();
	else
	{
		application::g_app->fatal_error( _T("%s: unrecognized message file extension: %s"),
			application::g_app->get_app_name(), ext );
		return STATUS_ERROR_NOT_SUPPORTED;
	}

	// Load file
	if( src->load( m_import_file_name.c_str() ).failed_status( status ) )
	{
		application::g_app->fatal_error( _T("%s: cannot load message file: %s"),
			application::g_app->get_app_name(), m_import_file_name.c_str() );
		return status;
	}

	// Append source
	msgout_context.append_source( *src );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF switch_import_messages.cxx*/
