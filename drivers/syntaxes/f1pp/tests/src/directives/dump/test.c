//test.c

#namespace NS1
# namespace NS2
#  define MAC	1
#  define PAC	1
#dump MAC
# end namespace
#dump NS2.MAC
#end namespace
#dump NS1.NS2.MAC

#print "Dumping NS1:"
#dump NS1
#print "Dumping NS1.NS2:"
#dump NS1.NS2
