
#define f(x)	x x
f( 1
#undef f
#define f	2
f)

// 1 2 1 2


// calloc as a function
ptr = calloc( 1,
#if !DEBUG_ENABLED
	10
#else
	16 + 10 + 16
#endif // DEBUG_ENABLED
);

// calloc as a macro
#define calloc(m, n)	calloc_dbg(m ,n)
ptr = calloc( 1,
#if !DEBUG_ENABLED
	10
#else
	16 + 10 + 16
#endif // DEBUG_ENABLED
);
