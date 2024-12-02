/*macro_FILE_NAME.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::c_pp::gcc::macro_FILE_NAME::initialize( const f1::pp::attribute_list * attrs,
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

const char * f1::c_pp::gcc::macro_FILE_NAME::get_name() const
{
	return "__FILE_NAME__";
}

const status_t f1::c_pp::gcc::macro_FILE_NAME::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	// Check current state
	__debugbreak_if( nullptr != args )
		return STATUS_ERROR_INTERNAL;

	// Get file part
	const char * name = PathFindFileName( parser.get_parser()->get_current_file_name() );

	// Store macro
	buf.add_part( "\"" );
	buf.add_part( name );
	buf.add_part( "\"" );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF macro_FILE_NAME.cxx*/
