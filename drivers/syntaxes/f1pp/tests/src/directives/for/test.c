
#set i = 0
//#for i:float=0; i <= 5 ; i += 1
#for ; i <= 5 ; i += 1
# print "i = " i
//# set i ++
#end
# print "Out i = " i

#set i = 0
#for i:float=0; i <= 5 ; i += 1
# print "i = " i
//# set i ++
#endfor
# print "Out i = " i

// Nested for loops
#for i = 0; i < 4; ++ i
# for j = 0; j < 4; ++ j
#  for k = 0; k < 4; ++ k
#	print "i, j, k = " i, j, k
#  end for//k
# end	 //j
#endfor //i
