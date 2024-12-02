/*macro_INCLUDE_LEVEL.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::c_pp::gcc::macro_INCLUDE_LEVEL::initialize( const f1::pp::attribute_list * attrs,
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

const char * f1::c_pp::gcc::macro_INCLUDE_LEVEL::get_name() const
{
	return "__INCLUDE_LEVEL__";
}

const status_t f1::c_pp::gcc::macro_INCLUDE_LEVEL::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	// Check current state
	__debugbreak_if( nullptr != args )
		return STATUS_ERROR_INTERNAL;

	// Count levels
	int count = 0;
	f1::parser::Isource * src = parser.get_parser()->get_current_source();
	if( nullptr != src )
	{
		for( ; ; )
		{
			src = src->get_parent();
			if( nullptr == src )
				break;
			++ count;
		}
	}

	char str[128];
	_ltoa_s( count, str, _countof(str) );

	// Store macro
	buf.add_part_alloc( str );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF macro_INCLUDE_LEVEL.cxx*/
