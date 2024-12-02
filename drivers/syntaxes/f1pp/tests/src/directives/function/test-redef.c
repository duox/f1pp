#function myfunc( a, ... )
	//#undef a
	//#set a = -1
	#print "a = " __VA_ARGS__
	#return 0
#end
myfunc( 10, 100 )
myfunc( 20, 200 )
