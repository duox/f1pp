/*invalid.c*/

////////////////////////////////////////////////////////////////////
// C++17 doesn't allow for macros in the __has_include argument list
// Macros are allowed under the F1PP syntax.
//

/**
 * Check files to be found, use macros.
 * The file "includes\existing.h" should exist.
 */
#define QUOTED_DIRECTORY	"includes"
#define QUOTED_FILENAME		"\existing.h"
#if !__has_include( QUOTED_DIRECTORY QUOTED_FILENAME )
1:PASSED
#else
1:NOTPASSED
#endif // def __has_include( "includes\existing.h" )

/**
 * Check files to be found, use macros.
 * The file <includes\existing.h> should exist.
 */
#define ANGLE_DIRECTORY		<includes>
#define ANGLE_FILENAME		<\existing.h>
#if !__has_include( ANGLE_DIRECTORY ANGLE_FILENAME )
2:PASSED
#else
2:NOTPASSED
#endif // def __has_include( "includes\existing.h" )

/*END OF invalid.c*/
