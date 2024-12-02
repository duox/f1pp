// #define test
#define X	1
#[weak]define X	2
#print "X =" X

#[weak]define Y	1
#[weak]define Y	2
#print "Y =" Y

#[weak]define Z	1
#define Z	2
#print "Z =" Z

// #set test
#print "===="

#set X	1
#[weak]set X	2
#print "X =" X

#[weak]set Y	1
#[weak]set Y	2
#print "Y =" Y

#[weak]set Z	1
#set Z	2
#print "Z =" Z
