
#define X	0
#switch X
	#case 0
		#print "X = 0"
	#case 1
		#fallthrough
	#case 2
		#print "X = 1 or 2"
#end

#define Y	1
#switch Y
	#case 0
		#print "Y = 0"
	#case 1
		#fallthrough
	#case 2
		#print "Y = 1 or 2"
#end

#define Z	2
#switch Z
	#case 0
		#print "Z = 0"
	#case 1
		#fallthrough
	#case 2
		#print "Z = 1 or 2"
#end
