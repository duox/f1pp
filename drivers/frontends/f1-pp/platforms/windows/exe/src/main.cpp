/*main.cpp*/
#include "pch.h"
#include "main.h"

static LONG	WINAPI UnhandledExceptionFilterCallback( struct	_EXCEPTION_POINTERS	* ExceptionInfo	)
{
	// Ignore breakpoints when debugger	is not attached
#ifdef _DEBUG
	if(	EXCEPTION_BREAKPOINT ==	ExceptionInfo->ExceptionRecord->ExceptionCode && !IsDebuggerPresent() )
		return EXCEPTION_EXECUTE_HANDLER;
#endif

	// Continue	search for exception handler
	return EXCEPTION_CONTINUE_SEARCH;
}

/*static int exception_filter( unsigned int code, struct _EXCEPTION_POINTERS * ep )
{
	__unreferenced_parameter( code );
	__unreferenced_parameter( ep );

	// Ignore breakpoints when debugger	is not attached
#ifdef _DEBUG
	if(	EXCEPTION_BREAKPOINT ==	code && !IsDebuggerPresent() )
		return EXCEPTION_EXECUTE_HANDLER;
#endif

	// Continue	search for exception handler
	return EXCEPTION_CONTINUE_SEARCH;
}*/

int	__cdecl	main( int argc,	char **	argv )
{
//	__try {
#ifdef _WINDOWS_
//		SetConsoleOutputCP(	65001 );
//		SetUnhandledExceptionFilter( &UnhandledExceptionFilterCallback );
#endif // def _WINDOWS_

//	__assert( false	);
		int	exit_code =	f1::pp::frontend::f1pp::application().run( argc, argv );

#ifdef _DEBUG
//		if(	f1::util::application::g_app->.get_flags( f1::pp::service::fl_debug_output ) )
//			f1::core::object::dump_object_leaks();
#endif // def _DEBUG
//		getch();
//		_CrtDumpMemoryLeaks();
		return exit_code;
//	} __except(	exception_filter( GetExceptionCode(), GetExceptionInformation() ) )
//	{
//	}
}

#pragma	comment(lib, "rpcrt4.lib")
#pragma	comment(lib, "shlwapi.lib")
#pragma	comment(lib, "version.lib")

/*END OF main.cpp*/
