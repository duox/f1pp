/*application.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::frontend::application::application( const char * name ):
	super( name )
{
}

status_t f1::pp::frontend::application::preinitialize_application() __noexcept
{
	super::preinitialize_application();

	// Exit
	return STATUS_SUCCESS;
}
status_t  f1::pp::frontend::application::uninitialize_application() __noexcept 
{
//	if( 0 == m_files_processed && !GET_FLAG(m_flags, fl_no_help_needed) )
//		print_usage();
	return super::uninitialize_application();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::application::on_filename( const f1::core::const_string & filename ) NOEXCEPT
{
	return m_frontend_context.parse_file( filename.data(), filename.get_size() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::frontend::application::new_context( core::Tref<context> & res, const core::Iidentifier * ident )
{
	return res.createT<frontend::context>( ident );
}

/*END OF application.cxx*/
