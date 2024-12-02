//test.c

#define COUNT	5
#print "Begin loop:"
#set A = 0
#repeat COUNT*2
	#set A = A + 1
	#print "A = " A
#end
#print "End loop"

#i = 0
#repeat M:10
# print i ": M = " M
# i += 1
#end
#print "M = " M

// Nested repeat loops
#repeat i:4
# repeat j:4
#  repeat k:4
#   if i == j && i == k
#	 print "i, j, k = " i, j, k
#   end
#  end
# end
#end
