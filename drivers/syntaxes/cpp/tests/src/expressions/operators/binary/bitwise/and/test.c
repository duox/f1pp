//test.c

#undef NUL		// used for testing undefined macros usage
#define ZERO		0
#define ONE			1
#define MSG_ZERO	_Pragma("message(\"0\")") 0
//#define MSG_ONE		_Pragma("message(\"1\")") 1

// Simple expression with macro substitutions
#if 1 & 0 == 0 & 1
PASSED
#endif

// Simple expression with macro substitutions
#if ONE & ZERO == 0
PASSED
#endif

// Simple expression with macro substitutions one of wich is undefined
#if ONE & NUL == 0
PASSED
#endif

// Priorities
#if ONE ^ ONE & ZERO
PASSED
#endif

// Lazy evaluation
#if 0 == (ZERO & MSG_ZERO)
PASSED
#endif
