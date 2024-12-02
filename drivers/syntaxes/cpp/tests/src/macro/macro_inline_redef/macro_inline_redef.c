//macro_inline_redef.c

#define macro_inline_redef( a, b )	a b
#define MACRO_INLINE_REDEF	1
macro_inline_redef(
	MACRO_INLINE_REDEF,
	#undef MACRO_INLINE_REDEF
	#define MACRO_INLINE_REDEF	2
	MACRO_INLINE_REDEF
	) // Should produce 1 2, not 1 1

macro_inline_redef(
	macro_inline_redef( 1, 2 ),
	#undef macro_inline_redef
	#define macro_inline_redef( a, b )	x, x
	macro_inline_redef( 1, 2 )
	) // Should produce 1 2 x, x (comma doesn't get accounted for parameter separator)

macro_inline_redef
	#undef macro_inline_redef
	#define macro_inline_redef( a, b )	a b
	(
	macro_inline_redef( 1, 2 ),
	macro_inline_redef( 1, 2 )
	) // Should produce 1 2 1 2 (comma doesn't get accounted for parameter separator)

macro_inline_redef
	#undef macro_inline_redef
	#define macro_inline_redef	1	// no args now
	(
	macro_inline_redef( 1, 2 ),
	macro_inline_redef( 1, 2 )
	) // Should produce  1(1, 2) 1(1, 2) (comma doesn't get accounted for parameter separator)

// END OF macro_inline_redef.c
