#define __STRINGIZE(v)	#v
#define STRINGIZE(v)	__STRINGIZE(v)

#concat N	`1` __STRINGIZE(v) `2`
#pragma message( "N = " STRINGIZE(N) )
