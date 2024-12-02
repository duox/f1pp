/*macro_BASE_FILE.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::c_pp::gcc::macro_BASE_FILE::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined ).failed_status( status ) )
		return status;

	// Initialize object

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::c_pp::gcc::macro_BASE_FILE::get_name() const
{
	return "__BASE_FILE__";
}

const status_t f1::c_pp::gcc::macro_BASE_FILE::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	// Check current state
	__debugbreak_if( nullptr != args )
		return STATUS_ERROR_INTERNAL;

	// Get base source
	const char * str = "";
	f1::parser::Isource * src = parser.get_parser()->get_current_source();
	if( nullptr != src )
	{
		for( ; ; )
		{
			f1::parser::Isource * parent = src->get_parent();
			if( nullptr == parent )
			{
				str = src->get_display_filename();
				break;
			}
			src = parent;
		}
	}

	// Store macro
	buf.add_part( "\"" );
	buf.add_part( str );
	buf.add_part( "\"" );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF macro_BASE_FILE.cxx*/
