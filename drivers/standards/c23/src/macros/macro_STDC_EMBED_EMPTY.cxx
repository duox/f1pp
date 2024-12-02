/*macro_STDC_EMBED_EMPTY.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::c_pp::c23::macro_STDC_EMBED_EMPTY::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_predefined ).failed_status( status ) )
		return status;

	// Initialize object
//	core::Tref<ns::name> nm;
//	core::Tref<ns::name_space> ns = &parser.get_ns_context()->get_root_namespace();
//	if( create_name( nm, *ns, 0, "has_embed" ).failed_status( status ) )
//		return status;


	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::c_pp::c23::macro_STDC_EMBED_EMPTY::get_name() const
{
	return "__STDC_EMBED_EMPTY__";
}

const status_t f1::c_pp::c23::macro_STDC_EMBED_EMPTY::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	// Check current state
	__debugbreak_if( nullptr != args )
		return STATUS_ERROR_INTERNAL;

	// Store macro value
	buf.add_part( "2" );

	// Exit
	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}

/*END OF macro_STDC_EMBED_EMPTY.cxx*/
