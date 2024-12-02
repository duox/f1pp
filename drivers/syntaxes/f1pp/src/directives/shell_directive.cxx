/*shell_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::shell_directive::shell_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::shell_directive::get_name() const
{
	return "shell";
}
// {E23A1664-4208-40C0-9799-ACBA9994A5C6}
const f1::guid_t f1::f1pp::shell_directive::m_guid =
{ 0xe23a1664, 0x4208, 0x40c0, { 0x97, 0x99, 0xac, 0xba, 0x99, 0x94, 0xa5, 0xc6 } };
const f1::guid_t & f1::f1pp::shell_directive::get_guid() const
{
	return m_guid;
}

// Operations

const status_t f1::f1pp::shell_directive::execute_commands( f1::f1pp::parser & f1par, const pp::string & cmd )
{
	// Create temporary batch file
	_TCHAR szTempFileName[1000] = "";
	HANDLE hTempFile = CreateTempFile( TEXT("f1pp"), szTempFileName, countof(szTempFileName) );
    if( INVALID_HANDLE_VALUE == hTempFile )
	{
		return _T('\0') == szTempFileName[0] ?
			f1par.message( syntax::IMSG_ERROR_CANNOT_CREATE_TEMPFILE ):
			f1par.message( syntax::IMSG_ERROR_CANNOT_CREATE_TEMPFILE_S, szTempFileName );
	}
	DWORD nBytes = 0;
	WriteFile( hTempFile, "@echo off\r\n", sizeof("@echo off\r\n") - 1, &nBytes, NULL );
	WriteFile( hTempFile, cmd.data(), DWORD(cmd.size()), &nBytes, NULL );
	CloseHandle( hTempFile );

	// Rename extension
	_TCHAR szNewTempFileName[1000];
	strncpy( szNewTempFileName, szTempFileName, countof(szNewTempFileName) - 1 );
	szNewTempFileName[countof(szNewTempFileName) - 1] = _T('\0');
	PathRenameExtension( szNewTempFileName, ".bat" );

	MoveFile( szTempFileName, szNewTempFileName );

	// Format command line
	_TCHAR szCmdLine[1024];
	strcpy( szCmdLine, getenv( "COMSPEC" ) );
	strcat( szCmdLine, " /C " );
	strcat( szCmdLine, szNewTempFileName );

	// Process batch file
	//status_t status = create_process_and_wait( f1par, NULL, szCmdLine, true );
	status_t status = super::execute_commands( f1par, pp::string(szCmdLine) );
	DeleteFile( szNewTempFileName );

	// Exit
	return status;
}


/*END OF shell_directive.cxx*/
