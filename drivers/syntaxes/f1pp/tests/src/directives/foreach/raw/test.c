
#print "Forward iterations for 1, \"2\", 3, \"4\", 5"
#foreach i: 1, "2", 3, "4", 5
# print "i =" i
#end

#print "Backward iterations for 1, \"2\", 3, \"4\", 5"
#[backward]foreach j: 1, "2", 3, "4", 5
# print "j =" j
#end

#macro mac_va_args_forward_forward_back( a, b, c, d )
# foreach i:a, b, c, d
#  foreach j:d, c, b, a
#    print "i, j =" i, j
#  end
# end
#end
#print "mac_va_args_forward_forward_back:"
mac_va_args_forward_forward_back( 1, 2, 3, 4 )

#macro mac_va_args_forward_backward ...
# foreach i:__VA_ARGS__
# 	[backward] foreach j:__VA_ARGS__
#  		print "i, j =" i, j
#	end
# end
#end
#print "mac_va_args_forward_backward:"
mac_va_args_forward_backward 1, 2, 3, 4
