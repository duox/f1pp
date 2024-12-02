//if.c

//////////////////////////////////////////////////////////////////
// Valid constructs

// Simple expression
#if 1
1: PASSED
#else
1: NOT PASSED
#endif // #if 1

// More complicated expression
#if 1 == 3 - 2
2: PASSED
#else
2: NOT PASSED
#endif // #if 1 == 3 - 2

// Macro expansion in expressions
#define ONE		1
#define TWO		2
#define THREE	3
#if ONE == THREE - TWO
3: PASSED
#else
3: NOT PASSED
#endif // #if ONE == THREE - TWO

// Nested #if's
#if 2 == 2
4.1: PASSED
# if 2 == 2
4.2: PASSED
# else
4.3: NOT PASSED
# endif
#else
4.4: NOT PASSED
# if 2 == 2
4.5: NOT PASSED
# else
4.6: NOT PASSED
# endif
#endif

// Sequential ifs/elif/endif
#if ONE == 1
5.1: PASSED
#elif ONE == 2
5.2: NOT PASSED
#else
5.3: NOT PASSED
#endif

// Invalid #endif is not detected
#if 0
# if 1
# endif X
#endif

// Ignore all invalid constructs between #if 0/#endif
#if 0
# invalid
# include
# if 1 +
# elif 1 2
# else X
# endif Y
# line -2
# error
# warning
`
#endif

#if 1
OK
#elif 1
FAILED: REP 1
#elif 1
FAILED: REP 2
#else
FAILED
#endif

//////////////////////////////////////////////////////////////////
// Potentionally invalid constructs

/*#if 1 > 2
#else
#if ()	// invalid expression skipped
#endif
#endif*/

/*#if 0
#elifndef UNDEFINED_MACRO
#define ELIFDEF_SUPPORTED
#else
#endif*/

//////////////////////////////////////////////////////////////////
// Invalid constructs

// No expression
#if
INV:0:NOT PASSED
#else
INV:0:PASSED
#endif

// Invalid expression
// More expression tests are located in tests/expr/ folder
#if 1 +
INV:1:NOT PASSED
#else
INV:1:PASSED
#endif

#if 1 2
INV:2:PASSED
#else
INV:2:NOT PASSED
#endif

// Extra characters after #endif
#if 0
INV:3:NOT PASSED
#else
INV:3:PASSED
#endif X
