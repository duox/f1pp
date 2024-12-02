/*macro_COUNTER.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::comext::macro_COUNTER::macro_COUNTER():
	m_counter( 0 )
{
}

const status_t f1::c_pp::comext::macro_COUNTER::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, pp::Imacro::macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined ).failed_status( status ) )
		return status;

	// Initialize object
//	core::Tref<ns::name> nm;
//	core::Tref<ns::name_space> ns = &parser.get_ns_context()->get_root_namespace();
//	if( create_name( nm, *ns, 0, "__COUNTER__" ).failed_status( status ) )
//		return status;

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::c_pp::comext::macro_COUNTER::get_name() const
{
	return "__COUNTER__";
}

const status_t f1::c_pp::comext::macro_COUNTER::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	// Check current state
	__debugbreak_if( nullptr != args )
		return STATUS_ERROR_INTERNAL;

	// Store macro
	char buffer[128];
	_itoa( m_counter, buffer, 10 );
	++ m_counter;

	buf.add_part_alloc( buffer );		// Store formatted time without 

	// Exit
	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}

/*END OF macro_COUNTER.cxx*/
