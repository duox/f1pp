#define X	2
#enum E1
	#define Y	7
	A1 = 0,
	A2 X + Y,
	#if 0
		A3
	#else
		A4
	#endif
#end enum
#print "E1.A1 = " E1.A1
#print "E1.A2 = " E1.A2
#print "E1.A3 = " E1.A3
#print "   A4 = " A4
#using namespace E1
#print "   A4 = " A4
#print "E1.X = " E1.X

#enum E2 : E1
	Z1
#end enum
#print "E2.A2 = " E2.A2
#print "E2.Z1 = " E2.Z1


#define e1	1
#enum E
	[aaa] e1, e2 = 10
	e3, e4,
	//#region
	e5
	e6
#end
#undef E.e5
#foreach e: E
# print e
#end
#print e1
