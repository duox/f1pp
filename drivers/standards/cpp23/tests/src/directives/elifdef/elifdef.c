//elifdef.c

//////////////////////////////////////////////////////////////////
// Valid constructs

#define M1

#ifdef M0
VALID.0: NOT PASSED
#elifdef M1
VALID.0: PASSED
#endif // #ifdef M0

#ifdef M0
VALID.0: NOT PASSED
#elifndef M0		// test again against M0
VALID.0: PASSED
#endif // #ifdef M0

//////////////////////////////////////////////////////////////////
// Potentionally invalid constructs


//////////////////////////////////////////////////////////////////
// Invalid constructs

// No macro name
#ifdef M0
INVALID.0: NOT PASSED
#elifdef
INVALID.0: NOT PASSED
#else
INVALID.0: PASSED
#endif

// Too many tokens
// Issues warning so first clause must pass.
#if 0
INVALID.1: NOT PASSED
#elifdef __FILE__ __LINE__
INVALID.1: PASSED
#else
INVALID.1: NOT PASSED
#endif

