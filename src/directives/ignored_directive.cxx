/*ignored_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::ignored_directive::ignored_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

unsigned f1::pp::ignored_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return cap_unimplemented & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::pp::ignored_directive::parse( pp::Iparser & parser )
{
	DBG_PRINT( "WARNING: call to unsupported parse.\n" );
	parser.get_parser()->skip_to_nl();
	return STATUS_ERROR_NOT_SUPPORTED;
}

/*END OF ignored_directive.cxx*/
