//test.c

#env MY_ENV = "MY_ENV"
#set MY_ENV		1
#print "MY_ENV =" MY_ENV
#if 1 == env("MY_ENV")
# print PASSED
#else
# print NOT PASSED
#endif

