/*macro_LINE.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::c_pp::c99::macro_LINE::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined ).failed_status( status ) )
		return status;

	// Initialize object
//	core::Tref<ns::name> nm;
//	core::Tref<ns::name_space> ns = &parser.get_ns_context()->get_root_namespace();
//	if( create_name( nm, *ns, 0, "__LINE__" ).failed_status( status ) )
//		return status;

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::c_pp::c99::macro_LINE::get_name() const
{
	return "__LINE__";
}

const status_t f1::c_pp::c99::macro_LINE::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	// Check current state
	__debugbreak_if( nullptr != args )
		return STATUS_ERROR_INTERNAL;

	// Store macro
	f1::parser::lineno_t line = parser.get_parser()->get_line();
	char buffer[128];
	_itoa( line, buffer, 10 );
	buf.add_part_alloc( buffer );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF macro_LINE.cxx*/
