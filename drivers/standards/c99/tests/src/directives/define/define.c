//define.c

/////////////////////////////////////////////////////////////////
// Valid data zone


// Object-like definition
#define OL_0
#define OL_1	1
OL_0
OL_1

// Function-like definition with fixed arg list
#define FLFA_0(x)
#define FLFA_1(x, y)	2
#define FLFA_2(x, y, z)	x
FLFA_0(1)
FLFA_1(1, 2)
FLFA_2(1, 2, 3)

#define FLFA_4(x,	\
 y)	x y
FLFA_4(1 2, 3 4)

// Function-like definition with variable arg list
// Tests for __VA_ARGS__, __VA_OPT__ and such are located in tests/macro/ subfolder
#define FLVA_0(x, ...)	x __VA_ARGS__
#define FLVA_1(...)		x __VA_ARGS__
#define FLVA_2(args ...)	x args
FLVA_0(1, 2, 3)
FLVA_1(1, 2, 3)
FLVA_2(1, 2, 3)


/////////////////////////////////////////////////////////////////
// Potentionally invalid data zone


#define OL_1			1				// redefinition of the same macro with same value (issues performance warning)
#define max (a, b)		__max(a, b)		// probably detached arglist
#define add(a, b)		a+b				// enclose parameters in parenthesis


/////////////////////////////////////////////////////////////////
// Invalid data zone


// No macro name specified
#define

// Invalid macro name
#define 1
#define 1	2

// Redefinition of existing macro with different value
#define OL_1	2
OL_1

// Invalid argument list of function-like macro
#define FLe_0(a		// unfinished parameter list
#define FLe_1(a	b)	// invalid parameter name
#define FLe_2(a,)	// unfinished argument list

// Invalid variadic macro
#define VAe_0(...arg)

// Redefine predefined macro
#define __FILE__

