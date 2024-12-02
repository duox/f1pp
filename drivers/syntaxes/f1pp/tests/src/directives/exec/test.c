
/////////////////////////////////////////////////////////////
// SINGLE LINE INVOCATIONS

// Execute command
#exec "cmd.exe /c echo XXX"

// Store string to the M macro
#exec M="cmd.exe /c echo YYY"
#print __LINE__ ": M =" M

// Store integer to the N macro
#exec N:int="cmd.exe /c echo 11"
#print __LINE__ ": N =" N

// Command not found, but no error message should be printed; W will retain its value
#W=2
#[soft] exec W="$cmd@.exe /c echo EEE"
#print __LINE__ ": W =" W

/////////////////////////////////////////////////////////////
// MULTI-LINE INVOCATIONS

// Execute command
#define COMMAND	cmd
#exec
	COMMAND.exe /c echo XXX
#endexec

// Store string to the M macro
#exec M=
	COMMAND.exe /c echo YYY
#endexec
#print __LINE__ ": M =" M

// Store integer to the N macro
#exec N:int=
	cmd.exe /c echo 11
#endexec
#print __LINE__ ": N =" N

// The second command not found, but no error message should be printed; W must retain its initial value of 1
#W=1
#[soft] exec W=
	cmd.exe /c echo 3
	$cmd@.exe /c echo EEE
#endexec
#print __LINE__ ": W =" W

/////////////////////////////////////////////////////////////
// ADVANCED TESTS

// Macro substitution inside the command line
#define CMD_NAME cmd
#W = 1
#exec W:int/**/+?=/**/CMD_NAME.exe /c echo 2
#print __LINE__ ": W =" W

/////////////////////////////////////////////////////////////
// INVALID INVOCATIONS
/*
// No command line
#exec P:float
#exec P:float =

// Command not found
#W="SDF"
#exec W="!cmd).exe /c echo EEE"
#print "W =" W

*/
