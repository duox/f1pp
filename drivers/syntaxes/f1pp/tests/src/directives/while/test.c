//test.c

#set A = 10
#while A
# print "A = " A
# set A=A-1
#endwhile

#i = 0
#while N: i < 10
# print i ": N = " N
# i += 1
//# i ++
#end while
#print "N = " N

// Nested while loops
#i = 4
#while i > 0
# j = 4
# while j > 0
#  k = 4
#  while k > 0
#   print "i, j, k = " i, j, k
#   set k --
#  end while
#  set -- j
# end while
# set -- i
#end while
