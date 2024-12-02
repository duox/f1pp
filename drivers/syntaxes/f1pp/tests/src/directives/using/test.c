//test.c

// Using entire namespace
#namespace NS
# define X		1
# define Y		7
#end
#using namespace NS
#print "X =" X
#print "Y =" Y

// Using individual elements
#define B		3
#namespace NS2
# define A		1
# define B		7
#end
#using NS2.A
#print "A =" A
#print "B =" B
#print "NS.B =" NS2.B

// Overloaded macros
#using NS2.B	// should generate error, B is not changed
#print "B =" B
