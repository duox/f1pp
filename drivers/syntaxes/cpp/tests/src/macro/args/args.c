#define a( x, y )	#x #y
a( 1, 0 )
a ( 1, 0 )
a /**/ ( 1, 0 )

a( 1, )
a( , 0 )
a( , )
a(,)

#define b( x )	#x
b( 0 )
b (1)

b()	// must be a warning

// A macro call is separated with another macro call
#define none()
#define identity(x) x
identity(none none() ())
