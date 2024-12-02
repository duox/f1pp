#define a(x)	a#x
a(0)

#define b(x)	a      #       x
b(1)
b(a(1))

#define c(x)	a /*c*/     #  /*c*/     x
c(1)
c(b(1))
c(b(a(1)))

#define AX(x)	Ax = #x
AX(1)
