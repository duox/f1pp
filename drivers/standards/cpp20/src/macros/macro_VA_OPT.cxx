/*macro_VA_OPT.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::c_pp::cpp20::macro_VA_OPT::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_disabled | macro_flags_t::fl_predefined | macro_flags_t::fl_self_parse_arglist ).failed_status( status ) )
		return status;

	// Initialize object
	m_params.append_param( 0, "text" );
	set_param_list( &m_params );

//	core::Tref<ns::name> nm;
//	core::Tref<ns::name_space> ns = &parser.get_ns_context()->get_root_namespace();
//	__debugbreak_if( create_name( nm, *ns, 0, "__VA_OPT__" ).failed_status( status ) )
//		return status;

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::c_pp::cpp20::macro_VA_OPT::get_name() const
{
	return "__VA_OPT__";
}

const status_t f1::c_pp::cpp20::macro_VA_OPT::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	// Check current state
	__debugbreak_if( nullptr != args )
		return STATUS_ERROR_INTERNAL;

	// Display error message, since __VA_OPT__ is processed separately
	parser.message( syntax::IMSG_ERROR_INVALID_USE_OF, "__VA_OPT__" );

	// Exit
	__unreferenced_parameter( buf );
	return STATUS_SUCCESS;
}

/*END OF macro_VA_OPT.cxx*/
