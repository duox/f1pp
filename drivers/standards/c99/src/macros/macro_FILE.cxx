/*macro_FILE.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::c_pp::c99::macro_FILE::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined ).failed_status( status ) )
		return status;

	// Initialize object
//	core::Tref<ns::name> nm;
//	core::Tref<ns::name_space> ns = &parser.get_ns_context()->get_root_namespace();
//	if( create_name( nm, *ns, 0, "__FILE__" ).failed_status( status ) )
//		return status;

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::c_pp::c99::macro_FILE::get_name() const
{
	return "__FILE__";
}

const status_t f1::c_pp::c99::macro_FILE::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	// Check current state
	__debugbreak_if( nullptr != args )
		return STATUS_ERROR_INTERNAL;

	core::Tref<f1::c_pp::parser> c_par = type_cast<f1::c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == c_par )
		return STATUS_ERROR_INVALID_TYPE;

	// Store macro
	c_par->stringify_chars( buf,
		pp::string( parser.get_parser()->get_current_file_name() ),
		'\"',
		c_pp::parser::fl_stringify_slashes
		);

	// Exit
	return STATUS_SUCCESS;
}

/*END OF macro_FILE.cxx*/
