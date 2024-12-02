
#if _Pragma("message(\"A\")") 1
OK
#endif

_Pragma("message(\"B\")")

_Pragma( /*A1!*/ "message(\"C\")" /*B2?*/ )

///// Following _Pragmas should all diagnose

_Pragma("#define MACRO")
#ifdef MACRO
INVALID
#endif // def MACRO

_Pragma					// no parameters
_Pragma()				// null parameter
_Pragma( "" )			// empty string
_Pragma( str )			// not a string
_Pragma( message("AA") )
_Pragma( 1 )			// not a string

_Pragma( "abc" def )
_Pragma( abc "def" )
_Pragma( abc "def" ghi )

// Check that _Pragma is processed differently than macros
// STR should expand and _Pragma should detect ending closing parenthesis
#define DO_PRAGMA _Pragma
#define STR "message( \"OK\" )")
_Pragma ( STR
DO_PRAGMA (STR

// Two unclosed _Pragmas in the end of file should both report error
_Pragma(
_Pragma(
