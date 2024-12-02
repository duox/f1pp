/*main.cpp*/
#include "pch.h"
#include "main.h"

HANDLE CreateTempFile( LPCTSTR Prefix, LPTSTR TempFileName, SIZE_T TempBufferSize )
{
	// Gets the temp path env string (no guarantee it's a valid path).
	char TempPathBuffer[1024];
	const DWORD dwRetVal = GetTempPath( countof(TempPathBuffer), TempPathBuffer );
	if( dwRetVal > countof(TempPathBuffer) || dwRetVal == 0 )
		return INVALID_HANDLE_VALUE;

	//  Generates a temporary file name. 
	char szTempFileName[MAX_PATH];
	const UINT uRetVal = GetTempFileName( TempPathBuffer, Prefix, 0, szTempFileName );
	if( uRetVal == 0 )
		return INVALID_HANDLE_VALUE;

	if( NULL != TempFileName )
	{
		if( strlen( szTempFileName ) >= TempBufferSize )
			return INVALID_HANDLE_VALUE;
		_tcsncpy( TempFileName, szTempFileName, TempBufferSize );
	}

	//  Creates the new file to write to for the upper-case version.
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 

	HANDLE hTempFile = CreateFile( szTempFileName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		&saAttr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL );
	if( hTempFile == INVALID_HANDLE_VALUE )
		return INVALID_HANDLE_VALUE;

	// Exit
	return hTempFile;
}

/*END OF main.cpp*/
