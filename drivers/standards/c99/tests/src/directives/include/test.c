//test.c


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Valid constructs

// normal includes
---- __LINE__ ----
#include "./common/include.0.h"
---- __LINE__ ----
#include ".\common\include.1.h"	// comment
---- __LINE__ ----
#include "common\include.2.h"	/* comment */
---- __LINE__ ----
#include ".\./common\include.3.h"
---- __LINE__ ----
#include <include.4.h>
---- __LINE__ ----
#include <././include.5.h>

// escapes in pathnames (\\ should be converted to \ and all other escapes should be ignored)
---- __LINE__ ----
#include ".\\.\common\file1.h"

// direct string concatenation (backslashes are not considered escapes)
---- __LINE__ ----
#include "common\" "include." "0" ".h"

---- __LINE__ ----
#define ZERO	"0"
#include "common\include." ZERO ".h"

---- __LINE__ ----
#define y(x)	"include." #x ".h"
#include "common\" y(0)

---- __LINE__ ----
#define z(x)	"include." ## #x ## ".h"
#include "common\" z(0)

// stringification
---- __LINE__ ----
#define STRINGIFY_MACRO(x) STR(x)
#define STR(x) #x
#define EXPAND(x) x

#define CONCAT5(n1, n2, n3, n4, n5) STRINGIFY_MACRO(EXPAND(n1)EXPAND(n2)EXPAND(n3)EXPAND(n4)EXPAND(n5))

#define INC0 	./common/
#define INC1	.
#define INC2 	include.

CONCAT5(INC0,INC1,/,INC2,0.h)

#include CONCAT5(INC0,INC1,/,INC2,0.h)

// system headers

---- __LINE__ ----
#define A	<
#define Z	>
#define D	.
#define CAT7_( a, b, c, d, e, f, g )	a##b##c##d##e##f##g
#define CAT7( a, b, c, d, e, f, g )		CAT7_( a, b, c, d, e, f, g )

CAT7( A, include, D, 0, D, h, Z )
#include CAT7( A, include, D, 0, D, h, Z )

// Including chain of files with relative filenames
---- __LINE__ ----
#include "common/file1.h"

// Launch recursive include
#define RECURSIVE	1
#include "common/recur.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Invalid constructs

// No filename
#include
#include /* some comment */

// Invalid file name specifications
#undef MAC
#include MAC	// non-existing macro

// Invalid string
#include "
#include "xyz
#include <
#include <xyz
#include "xyz>
#include <xyz"

// Non-existing file
#include ".//...."

// Including self
//#include __FILE__
