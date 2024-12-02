//test.c

#define AB

// Normal usage
#if defined AB
PASS
#else
FAIL
#endif

#if !defined AB
FAIL
#else
PASS
#endif

#if defined( AB )
PASS
#else
FAIL
#endif

#if !defined( AB )
FAIL
#else
PASS
#endif

// Computed variants
#define D( x )		defined x
#if D( ABX )
FAIL	// ABX is not defined
#else
PASS
#endif

#define DC( x, y )	defined x ## y
#if DC( A, B )
PASS
#else
FAIL
#endif

// Redefinition of `defined'
#define defined( x )	defined x	// error

// `defined' without arguments
#if defined
FAIL
#else
PASS
#endif

// defined() self-parser
#define r_paren )
#if defined( x r_paren
#endif
