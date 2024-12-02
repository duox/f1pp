
#if deprecated_directive(abort)
# print "#abort is marked deprecated"
#else
# print "Fail"
#endif

#if !deprecated_directive(print)
# print "#print is not marked deprecated"
#else
# print "Fail"
#endif
