
/*#repeat j:10
# repeat i:20
#  print "i = " i
#  if i > 5 && j == 3
#   break #repeat 2
#  end
# end
#end*/

/*#repeat j:10
# for i = 0; i < 20; ++ i
#  print "i = " i
#  if i > 5 && j == 3
#   break #repeat
#  end
# end
#end*/

// Nested while loops
#for j = 0; j < 100; ++ j
# print "for j "
# for i = 0; i<100; ++ i
#  print "for i "
#  if i == 5
#   break
#  end if
#  print "i = " i ", j = " j
# end
# if j == 3
#  break
# end if
#end
#print "---"

// Early exit from endless loop
#while true
# break
# print "Not working"
#end
#print "Working"
