//test.c

#pragma option(A=1)
#set AX=option(A)
#print "A =" AX

//#pragma option(push,A=2)
#pragma option(push)
#pragma option(A=2)
#set AX=option(A)
#print "A =" AX

#pragma option(pop)
#set AX=option(A)
#print "A =" AX
