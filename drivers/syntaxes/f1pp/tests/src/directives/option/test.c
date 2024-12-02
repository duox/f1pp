
/*#option X = 10
#pragma option(push X)
#option X = 11
#set N = option(X) + 10
#print "N =" N
#pragma option(pop)
#set N = option(X) + 10
#print "N =" N*/



#option X = 10
#pragma option(X=(11+2))
#set N = option(X)
#print "N =" N

///////////////////////////////////////////////////////////

#option X=10
#option.push "X"

#option X=11
#set N=option(X)
#print "X =" N

#option.pop
#set N=option(X)
#print "X =" N

///////////////////////////////////////////////////////////
