//test.c

#set A = 1
#set B = A <= 1

#set A = A + 2

#print "A =" A

///////////////////////////////////

#set A	1
#print "A =" A
#set A:float += 2
#print "A =" A

///////////////////////////////////

#set A = 2
+ 3
#print "A =" A

///////////////////////////////////

#set X 0
#set X = 0; ++ X ++ = X * 2	// ++ X, X = X * 2, ++ X => 1, 2, 3
//#set X = 4 * 2	// ++ X, X = X * 2, ++ X => 1, 2, 3
#print "X =" X

///////////////////////////////////
/*#set A "ABCD" & "EFG"
#set A += "HIJK"
#pragma message( "A = " STRINGIZE(A) )
#if A == "ABCDE" + "FGHIJK"
# pragma message( "YES" )
#else
# pragma message( "NO" )
#endif*/


#set X = 1
#set X ?= 0
#print "X =" X

///////////////////////////////////

#set X:int = 5; Y:int = 4
#print "X =" X ", Y =" Y
