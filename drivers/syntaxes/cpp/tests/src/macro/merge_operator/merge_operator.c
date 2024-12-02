// Test of recursion that must be disabled
#define II	I ## I
II;

#define DDD10 "xxx"

// Merge two parameters and form name of another macro that later gets substituted
#define A(a, b)	a/**/##/**/b
const char * sss = A(DDD, 10);		// should assign "xxx"

// Merge two tokens and form name of another macro that later gets substituted
#define B()	DDD##\
	10
const char * sss = B();				// should assign "xxx"

// Merge token and parameter and form name of another macro that later gets substituted
#define C(b)	DDD ##b
const char * sss = C(10);			// should assign "xxx"


// Merge parameter and non-identifier
#define D(a)	a ## ++
D(i);								// forms "i++" (looses spaces)
#define E(a)	-- ## a
E(i);								// forms "--i" (looses spaces)

// Merge non-parameter identifier and non-identifier
#define F	i ## ++
F;									// forms "i++" (looses spaces)
#define G	-- ## i
G;									// forms "--i" (looses spaces)


// Test of recursion that must be disabled
#define H	H ## i
H;

// Merge chain
#define MERGE3(a, b, c)		a ## b ## c
MERGE3( H, 1, 0 );


// Spaces behaviour during merge
#define glue_arg_arg(a, b)	a   ##     b
glue_arg_arg( a, 1 )
glue_arg_arg( a    b, c    d )

#define glue_ident_arg( a )	IDENT##/**/a
glue_ident_arg( 1 )

#define glue_arg_ident( a )	a##IDENT
glue_arg_ident( 1 )

#define glue_ident_ident( a )	ident##IDENT
glue_ident_ident( a )

// Variable argument list merge
#define vam( format, ... )		printf( format, ## __VA_ARGS__ )
vam( "plain string" );
vam( "%d %d", 1, 1 );

