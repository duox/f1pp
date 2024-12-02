/*macro_VA_ARGS.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::c_pp::c99::macro_VA_ARGS::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_disabled | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined ).failed_status( status ) )
		return status;

	// Initialize object
//	core::Tref<ns::name> nm;
//	core::Tref<ns::name_space> ns = &parser.get_ns_context()->get_root_namespace();
//	__debugbreak_if( create_name( nm, *ns, 0, "__VA_ARGS__" ).failed_status( status ) )
//		return status;

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::c_pp::c99::macro_VA_ARGS::get_name() const
{
	return "__VA_ARGS__";
}

const status_t f1::c_pp::c99::macro_VA_ARGS::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	// Check current state
	__debugbreak_if( nullptr != args )
		return STATUS_ERROR_INTERNAL;

	// Display error message, since __VA_ARGS__ is processed separately
	parser.message( syntax::IMSG_ERROR_INVALID_USE_OF, "__VA_ARGS__" );

	// Exit
	__unreferenced_parameter( buf );
	return STATUS_SUCCESS;
}

/*END OF macro_VA_ARGS.cxx*/
