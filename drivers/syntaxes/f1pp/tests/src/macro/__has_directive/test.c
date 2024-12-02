//test.c

#if __has_directive(define)
# print "1: passed"
#else
# print "1: NOT passed"
#endif

#if !__has_directive(non_existing_directive)
# print "2: passed"
#else
# print "2: NOT passed"
#endif
