/*switch_output_filename.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_output_filename::switch_output_filename( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_output_filename::get_name() const NOEXCEPT
{
	return "o";
}
const char * f1::pp::frontend::f1pp::switch_output_filename::get_long_name() const NOEXCEPT
{
	return "output-filename";
}
const char * f1::pp::frontend::f1pp::switch_output_filename::get_desc() const NOEXCEPT
{
	return "Specify output file names";
}
/*const char * f1::pp::frontend::f1pp::switch_output_filename::get_params() const NOEXCEPT
{
	return "each|warning|error|none";
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_output_filename::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_output_filename::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	// Clear the file name
	if( '-' == switchar )
	{
		application::get_app()->get_context()->set_output_file_name( nullcstr );
		return STATUS_SUCCESS;
	}

	// Set the file name
	f1::core::const_string ident = rest;
	ident.dequotize();

	application::get_app()->get_context()->set_output_file_name( ident );

	// Exit
	__unreferenced_parameter( name );
	return STATUS_SUCCESS;
}

/*END OF switch_output_filename.cxx*/
