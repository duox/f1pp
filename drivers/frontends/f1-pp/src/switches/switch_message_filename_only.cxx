/*switch_message_filename_only.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_message_filename_only::switch_message_filename_only( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_message_filename_only::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_message_filename_only::get_long_name() const NOEXCEPT
{
	return "message-filename-only";
}
const char * f1::pp::frontend::f1pp::switch_message_filename_only::get_desc() const NOEXCEPT
{
	return "Display filenames only in messages";
}
/*const char * f1::pp::frontend::f1pp::switch_message_filename_only::get_params() const NOEXCEPT
{
	return "each|warning|error|none";
}*/
static const char help_text[] = {
	"Strip full path names to file names when writing to console."
};
const char * f1::pp::frontend::f1pp::switch_message_filename_only::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_message_filename_only::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	if( _T('-') == switchar )
		application::get_app()->get_context()->modify_flags( 0, pp::context::fl_message_file_names_only );
	else
		application::get_app()->get_context()->modify_flags( pp::context::fl_message_file_names_only );

	if( nullchar != *rest )
		application::get_app()->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}

/*END OF switch_message_filename_only.cxx*/
