#define X	0
#function myfunc( a, b )
	#define X	2
	#ifdef X
	#	print "X is defined: X = " X
	#else
	#	print "X is not defined"
	#endif
	#return 10
#end
#function dieci()
	#define X	2
	ifdef X
		print "X is defined: X = " X
	else
		print "X is not defined"
	endif
	return 1
#end
//#if dieci()
#if 10 == myfunc( 1, 2 )
# print "DONE!"
#else
# print "not done..."
#endif
#print "X = " X

/*#function myfunc( a, b )
	#define X	2
	#ifdef X
	#	print "X is defined: X = " X
	#else
	#	print "X is not defined"
	#endif
	#return 10
#end*/

#function printf( format, ... )
# format STR format __VA_ARGS__
# print STR
# return 0
#end
printf( "A=%d", 10 )

/*#function printf( format, ... )
# format STR = format __VA_ARGS__
# print STR
# return countof STR		// return number of printed characters
#end
#set N = printf( "A = %d", 10 )
#print "N =" N*/


#function myfunc2( a, ... )
	//#undef a
	//#set a = -1
	#print "a + " __VA_ARGS__
	#return 0
#end
myfunc2( 10, 100 )
myfunc2( 20, 100, 200 )
