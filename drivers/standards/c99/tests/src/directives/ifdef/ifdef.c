//ifdef.c

//////////////////////////////////////////////////////////////////
// Valid constructs

// #ifdef
#ifdef __FILE__
VAL.0: PASSED
#else
VAL.0: NOT PASSED
#endif // #if 1

// #ifndef
#ifndef __FAYLE__
VAL.0: PASSED
#else
VAL.0: NOT PASSED
#endif // #if 1

// Nested #ifdef/#ifndef
#ifdef __FILE__
 #ifndef __FAYLE__
VAL.0: PASSED
 #else
VAL.0: NOT PASSED
 #endif // #if 1
#else
 #ifndef __FAYLE__
VAL.0: NOT PASSED
 #else
VAL.0: NOT PASSED
 #endif // #if 1
#endif // #if 1

//////////////////////////////////////////////////////////////////
// Potentionally invalid constructs


//////////////////////////////////////////////////////////////////
// Invalid constructs

// No macro name
#ifdef
INV.0: NOT PASSED
#else
INV.0: PASSED
#endif

#ifndef
INV.0: NOT PASSED
#else
INV.0: PASSED
#endif

// Too many tokens
// Issues warning so first clause must pass.
#ifdef __FILE__ __LINE__
INV:1: PASSED
#else
INV:1: NOT PASSED
#endif

#ifndef __FILE__ __LINE__
INV.1: NOT PASSED
#else
INV.1: PASSED
#endif
