//test.c

// Option OX does not exist
#if __has_option(OX)
NOT passed
#else
Passed
#endif

// Create option
#option OX

#if !__has_option(OX)
NOT passed
#else
Passed
#endif

// Modify option
#option OX = 10

#set X = option(OX)
#print "X =" X

#if !__has_option(OX)
NOT passed
#else
Passed
#endif

// Delete option
#option.delete OX

#if __has_option(OX)
NOT passed
#else
Passed
#endif

