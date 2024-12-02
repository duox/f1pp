/*macro_GLOBAL_LINE.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::f1pp::macro_GLOBAL_LINE::initialize( const f1::pp::attribute_list * attrs,
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

const char * f1::f1pp::macro_GLOBAL_LINE::get_name() const
{
	return "__GLOBAL_LINE__";
}

const status_t f1::f1pp::macro_GLOBAL_LINE::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	char str[128];
	_ltoa_s( parser.get_base_output()->get_global_line(), str, 10 );

	buf.add_part_alloc( str );

	__unreferenced_parameter( args );
	return STATUS_SUCCESS;
}

/*END OF macro_GLOBAL_LINE.cxx*/
