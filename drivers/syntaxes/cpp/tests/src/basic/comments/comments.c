/*comments.c*/

//

/\
/ Comment (ISO/IEC 9899:2023 6.4.9.3 Example)

//**/X

// A multi-line comment in multi-line macros without EOL screening
#define A(x)	\
	/*
	*/\
	x
A(1)		// outputs "1"

// A single-line comment in multi-line macros with EOL screening
#define B(x)	\
	x\
	// \
	// \
	x
B(1)		// outputs "1", not "1 1"

// Comment are ignored when macros are substituted
#define BEGIN	/##*
#define END		*##/

BEGIN
END

#define GLUE(x, y)	x##y
GLUE(/,/) X
GLUE(/,*) X GLUE(*,/)

// Disabled nested comments (issues a warning)
/* /* */

// End-comment outside of a comment
*/

// Single-line comment in the end of macro definition
#define C	1 //
C 2			// outputs "1 2"

