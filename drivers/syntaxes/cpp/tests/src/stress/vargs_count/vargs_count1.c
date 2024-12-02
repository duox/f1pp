//vargs_count1.c
// Source: https://stackoverflow.com/questions/11317474/macro-to-count-number-of-arguments/11742317
// Modified to reflect test goal

#define PP_ARG_N( \
          _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
         _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
         _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
         _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
         _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
         _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
         _61, _62, _63, N, ...) N

/* Note 63 is removed */
#define PP_RSEQ_N()                                        \
         62, 61, 60,                                       \
         59, 58, 57, 56, 55, 54, 53, 52, 51, 50,           \
         49, 48, 47, 46, 45, 44, 43, 42, 41, 40,           \
         39, 38, 37, 36, 35, 34, 33, 32, 31, 30,           \
         29, 28, 27, 26, 25, 24, 23, 22, 21, 20,           \
         19, 18, 17, 16, 15, 14, 13, 12, 11, 10,           \
          9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define ARG_COUNT_(...)    PP_ARG_N(__VA_ARGS__)

/* Note dummy first argument _ and ##__VA_ARGS__ instead of __VA_ARGS__ */
#define ARG_COUNT(...)     ARG_COUNT_(_, ##__VA_ARGS__, PP_RSEQ_N())

//#define ARG_COUNT(...)     ARG_COUNT(__VA_ARGS__)

// 0
ARG_COUNT()
// 1
ARG_COUNT(a)
ARG_COUNT(())
// 2
ARG_COUNT(a,b)
ARG_COUNT(,b)
ARG_COUNT(a,)
ARG_COUNT(,)
ARG_COUNT((),())
// 3
ARG_COUNT(a,b,c)
ARG_COUNT(,b,c)
ARG_COUNT(a,,c)
ARG_COUNT(a,b,)
ARG_COUNT(,,)
ARG_COUNT((),(),())

//END OF vargs_count1.c
