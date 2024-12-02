/*main.cpp*/
#include "pch.h"
#include "main.h"

BOOL WINAPI DllMain( HINSTANCE hInstance, DWORD fdwReason, LPVOID lpReserved )
{
	switch( fdwReason ) 
	{ 
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;

	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}

	UNREFERENCED_PARAMETER( hInstance );
	UNREFERENCED_PARAMETER( lpReserved );
	return TRUE;
}

/*END OF main.cpp*/
