/*Ifunction.cxx*/
#include "pch.h"
#include "main.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::pp::Ifunction::get_name( size_t * len ) const
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( len );
	return nullcstr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::pp::Ifunction::parse( pp::Iparser & parser, pp::variant & result ) const
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( result );
	return STATUS_ERROR_PURE_CALL;
}

/*END OF Ifunction.cxx*/
