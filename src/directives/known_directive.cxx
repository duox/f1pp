/*known_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::known_directive::known_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

unsigned f1::pp::known_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;//cap_unimplemented & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::pp::known_directive::parse( pp::Iparser & parser )
{
	parser.get_parser()->skip_to_nl();

	const char * start = parser.get_directive_start();
	__debugbreak_if( nullcstr == start )
		return STATUS_ERROR_INVALID_STATE;

	const char * end = parser.get_parser()->get_ptr();
	__debugbreak_if( nullcstr == end )
		return STATUS_ERROR_INVALID_STATE;
	__debugbreak_if( start >= end )
		return STATUS_ERROR_INVALID_STATE;

	return parser.get_output()->write(
		start,
		end - start
		);
}

/*END OF known_directive.cxx*/
