#switchdef
#default:
	#print "Default clause"	
#case X
	#print "X is defined"
	#fallthrough
#case Y
	#print "X or Y is defined"
#case Z
	#print "Z is defined"
#end
#switchndef
#default:
	#print "Default clause"	
#case X
	#print "X is not defined"
	#fallthrough
#case Y
	#print "X or Y is not defined"
#case Z
	#print "Z is not defined"
#end

#print "------------------------"	

#define X
#define Z

#print "------------------------"	

#switchdef
#default:
	#print "Default clause"	
#case X
	#print "X is defined"
	#fallthrough
#case Y
	#print "X or Y is defined"
#case Z
	#print "Z is defined"
#end
#switchndef
#default:
	#print "Default clause"	
#case X
	#print "X is not defined"
	#fallthrough
#case Y
	#print "X or Y is not defined"
#case Z
	#print "Z is not defined"
#end

#print "------------------------"	

#undef X

#print "------------------------"	

#switchdef
#default:
	#print "Default clause"	
#case X
	#print "X is defined"
	#fallthrough
#case Y
	#print "X or Y is defined"
#case Z
	#print "Z is defined"
#end
#switchndef
#default:
	#print "Default clause"	
#case X
	#print "X is not defined"
	#fallthrough
#case Y
	#print "X or Y is not defined"
#case Z
	#print "Z is not defined"
#end

#print "------------------------"	

#switchdef
# case Y
	#print "Z is defined"
# case X || Z
	#print "Either X or Z is defined"
#end switchdef

