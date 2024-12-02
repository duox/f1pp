//varargs.c

// Standard format, with __VA_ARG__ and __VA_OPT__ support
#define VA(format, ...)		printf( format __VA_OPT__(,) __VA_ARGS__ )
VA( "%d %c %s\n", 7, 'J', "PPP" );
VA( "plain string\n" );

// GCC compatible format, with named vararg argument
#define VM(format, args...)	printf( format, ## args )
VM( "%d %c %s\n", 7, 'J', "PPP" );
VM( "plain string\n" );

// NULL terminated list of arguments
#define VO(format, ...)		print_args( format __VA_OPT__(,) __VA_ARGS__ __VA_OPT__(, NULL) )
VO( "%d %c %s\n", 7, 'J', "PPP" );
VO( "plain string\n" );

//END OF varargs.c
