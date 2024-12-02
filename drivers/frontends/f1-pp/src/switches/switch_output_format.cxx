/*switch_output_format.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_output_format::switch_output_format( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_output_format::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_output_format::get_long_name() const NOEXCEPT
{
	return "output-format";
}
const char * f1::pp::frontend::f1pp::switch_output_format::get_desc() const NOEXCEPT
{
	return "Set or list output formats";
}
/*const char * f1::pp::frontend::f1pp::switch_output_format::get_params() const NOEXCEPT
{
	return "<format>";
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_output_format::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_output_format::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	// Check current state
	if( rest.is_empty() )
	{
		application::g_app->fatal_error( _T("%c%s: expected output format name or LIST"), switchar, core::string(name).c_str() );
		return STATUS_ERROR_INVALID_SYNTAX;
	}

	// List formats
	if( 4 == rest.length() && core::string::equal_nocase( rest.data(), "list", rest.length() ) )
	{
		application::g_app->message( "Output classes:\n" );
		array_foreach( pp::output_class_list_t::const_iterator, it, pp::service::get().get_output_class_list() )
		{
			core::string buf;
			it->get_guid().format( buf );
			application::g_app->message(" %s\t%s\n", buf.c_str(), it->get_name() );
		}
		application::g_app->modify_flags( application::fl_no_help_needed );
		return STATUS_SUCCESS;
	}

	// Fetch GUID
	core::Tref<pp::Ioutput_class> output_class = nullptr;
	if( '{' == *rest.data() )
	{
		f1::guid_t guid;
		const char * ep;
		status_t status = guid.parseA( rest.data(), rest.length(), &ep );
		if( status.failed() )
		{
			application::g_app->fatal_error( _T("%c%s: expected GUID inside { and }"), switchar, core::string(name).c_str() );
			return STATUS_ERROR_INVALID_SYNTAX;
		}
		if( ep < rest.data_end() )
		{
			application::g_app->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
			return STATUS_ERROR_INVALID_SYNTAX;
		}

		array_foreach( pp::output_class_list_t::const_iterator, it, pp::service::get().get_output_class_list() )
		{
			if( it->get_guid() == guid )
			{
				output_class = *it;
				break;
			}
		}
	}
	// Fetch identifier
	else
	{
		array_foreach( pp::output_class_list_t::const_iterator, it, pp::service::get().get_output_class_list() )
		{
			if( core::string::equal_nocase( rest.data(), it->get_name(), rest.length() ) )
			{
				output_class = *it;
				break;
			}
		}
	}
	if( nullptr == output_class )
	{
		application::g_app->fatal_error( _T("%c%s: expected output format name or LIST"), switchar, core::string(name).c_str() );
		return STATUS_ERROR_INVALID_SYNTAX;
	}

	// Create output
	application::get_app()->set_output_class( *output_class );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF switch_output_format.cxx*/
