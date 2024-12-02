//test.c

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Parameterized includes

#include "test.h"/**/( 1, 2.0 )
#include "test.h"/**/( "abc", defg )

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include to the macro

#include MM="test.h" (10, 20)
#print "Include contents: MM =" MM

// Use M for the second time; M should not be expanded
#include MM="test.h" (10, 20)
#print "Include contents: MM =" MM

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Multiple includes

// This macro has the same name as the parameter in test.h,
// so it should be shadowed during preprocessing of test.h
#define AA	0
#print "Before: AA =" AA

#define XX	9
#include "test.h"/**/(1, AA)/**/,/**/"test.h"/**/(XX, 2)

#print "MM =" MM			// should remain unchanged
#print "After: AA =" AA		// should remain unchanged

// Multiple includes + include to macro

#include /**/XX/**/=/**/"test.h"/**/(1, AA)/**/,/**/YY/**/+=/**/"test.h"/**/(MM, 2)
#print "XX =" XX
#print "YY =" YY

// Multiline macro parameter

#macro LL
 1
 2
#end
#include /**/XX/**/=/**/"test.h"/**/(1, LL)/**/,/**/XX/**/+=/**/"test.h"/**/(LL, 2)
#print "XX =" XX
