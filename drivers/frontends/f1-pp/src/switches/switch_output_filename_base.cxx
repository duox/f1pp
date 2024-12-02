/*switch_output_filename_base.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_output_filename_base::switch_output_filename_base( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_output_filename_base::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_output_filename_base::get_long_name() const NOEXCEPT
{
	return "output-filename-base";
}
const char * f1::pp::frontend::f1pp::switch_output_filename_base::get_desc() const NOEXCEPT
{
	return "Specify base directory for file names";
}
/*const char * f1::pp::frontend::f1pp::switch_output_filename_base::get_params() const NOEXCEPT
{
	return "each|warning|error|none";
}*/
static const char help_text[] = {
	"Make file names stored in output files relative to the specified directory. "
	"If file isn't located in the specified directory or in some nested subdirectory of the specified directory, default rules apply.\n"
	"If a relative pathname is specified, it's transformed to the absolute pathname relatively to the current directory."
};
const char * f1::pp::frontend::f1pp::switch_output_filename_base::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_output_filename_base::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	// Set the path name
	if( '-' == switchar )
		m_base_path.clear();
	else
		m_base_path = rest;
	application::get_app()->get_context()->set_output_base_directory( m_base_path );

	// Exit
	__unreferenced_parameter( name );
	return STATUS_SUCCESS;
}

/*END OF switch_output_filename_base.cxx*/
