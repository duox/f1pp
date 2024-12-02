//test.c

// Single namespace
#namespace NS
# define A	1
A
#end
NS.A
#using NS.A
A

#namespace NS2 = NS
NS2.A

#alias NSA = NS.A
NSA

// Nested namespaces
#namespace MATH
#  namespace CONST
#    define PI	3.14156
PI
#  end
CONST.PI
#end namespace
MATH.CONST.PI

#namespace MC = MATH.CONST
MC.PI

//END OF test.c
