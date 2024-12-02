
#define mac( arg )		result = arg

mac( 10 )
mac( mac( 10 ) )
mac( mac( mac( 10 ) ) )

ma\
c( 1\
0 )
ma\
c( ma\
c( ma\
c( 1\
0 ) ) )

mac(
#include "nums/10.h"
)
mac(
#include "nums/10nl.h"
)

#define f(x)	x
f( f(1) + f(2) )	// " 1  +  2 "
