/*macro_TIME.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::c_pp::c99::macro_TIME::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined ).failed_status( status ) )
		return status;

	// Initialize object
//	core::Tref<ns::name> nm;
//	core::Tref<ns::name_space> ns = &parser.get_ns_context()->get_root_namespace();
//	if( create_name( nm, *ns, 0, "__TIME__" ).failed_status( status ) )
//		return status;

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::c_pp::c99::macro_TIME::get_name() const
{
	return "__TIME__";
}

const status_t f1::c_pp::c99::macro_TIME::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	// Check current state
	__debugbreak_if( nullptr != args )
		return STATUS_ERROR_INTERNAL;

	core::Tref<f1::c_pp::parser> c_par = type_cast<f1::c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == c_par )
		return STATUS_ERROR_INVALID_TYPE;

	// Warn if enabled warnings on non-reproducible output
	if( c_par->get_c_message_flags( c_pp::parser::c_message_flags_t::fl_warn_time_macros ) )
		c_par->message( syntax::IMSG_WARNING_MACRO_EXP_NON_REPRODUCIBLE );

	// Store macro
	time_t aclock;
	time( &aclock );							// get time in seconds
	struct tm *newtime = localtime( &aclock );	// convert time to struct tm form

	char buffer[128];
	strftime( buffer, countof(buffer), "%H:%M:%S", newtime );

	buf.add_part_alloc( buffer );		// Store formatted time without 

	// Exit
	return STATUS_SUCCESS;
}

/*END OF macro_TIME.cxx*/
