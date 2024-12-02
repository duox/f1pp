/*valid.c*/
/** @file
 *
 * @brief Source file for __has_include test.
 *
 *	Note that backslash must be processed as-is, without treating it as a escape character.
 */

/**
 * Check pseudo-function availability.
 */
#ifdef __has_include
1:PASSED
#else
1:NOTPASSED
#endif // def __has_include

/**
 * Check files to be found.
 * The file "includes\existing.h" should exist.
 */
#if __has_include( "includes\existing.h" )
2:PASSED
#else
2:NOTPASSED
#endif // def __has_include( "includes\existing.h" )

/**
 * Check files to be found on the system path list.
 * The file <includes\existing.h> should exist.
 */
#if __has_include( <includes\existing.h> )
3:PASSED
#else
3:NOTPASSED
#endif // def __has_include( <includes\existing.h> )


/**
 * Check files to be not found.
 * The file "includes\nonexisting.h" should not exist.
 */
#if !__has_include( "includes\nonexisting.h" )
4:PASSED
#else
4:NOTPASSED
#endif // def !__has_include( "includes\nonexisting.h" )

/**
 * Check files to be not found on the system path list.
 * The file <includes\nonexisting.h> should not exist.
 */
#if !__has_include( <includes\nonexisting.h> )
5:PASSED
#else
5:NOTPASSED
#endif // def !__has_include( <includes\nonexisting.h> )

/**
 * Check files to be found, use macros.
 * The file "includes\existing.h" should exist.
 */
#define QUOTED_DIRECTORY	"includes"
#define QUOTED_FILENAME		"\existing.h"
#if __has_include( QUOTED_DIRECTORY QUOTED_FILENAME )
6:PASSED
#else
6:NOTPASSED
#endif // def __has_include( "includes\existing.h" )

/**
 * Check files to be found, use macros.
 * The file <includes\existing.h> should exist.
 */
#define ANGLE_DIRECTORY		<includes>
#define ANGLE_FILENAME		<\existing.h>
#if __has_include( ANGLE_DIRECTORY ANGLE_FILENAME )
7:PASSED
#else
7:NOTPASSED
#endif // def __has_include( "includes\existing.h" )

/*END OF valid.c*/
