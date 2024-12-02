#ifdef __has_embed
# print 1.1:OK
#else
# print 1.1:NOT OK
#endif

#if __STDC_EMBED_NOT_FOUND__ == __has_embed( "__has_embed-file-.bin" )
# print 2.1:OK
#else
# print 2.1:NOT OK
#endif
#if 0 == __STDC_EMBED_NOT_FOUND__
# print 2.2:OK
#else
# print 2.2:NOT OK
#endif

#if __STDC_EMBED_EMPTY__ == __has_embed( "__has_embed-file0.bin" )
# print 3.1:OK
#else
# print 3.1:NOT OK
#endif
#if 2 == __STDC_EMBED_EMPTY__
# print 3.2:OK
#else
# print 3.2:NOT OK
#endif

#if __STDC_EMBED_FOUND__ == __has_embed( "__has_embed-file1.bin" )
# print 4.1:OK
#else
# print 4.1:NOT OK
#endif
#if 1 == __STDC_EMBED_FOUND__
# print 4.2:OK
#else
# print 4.2:NOT OK
#endif
