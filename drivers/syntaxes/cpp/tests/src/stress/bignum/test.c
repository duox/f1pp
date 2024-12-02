//test.c

#define BIG64 18446744073709551615ull
#define BIG32 4294967295ul

#define XX BIG64/BIG32
#define YY BIG64%BIG32

// Check division result
#if XX == 4294967297ull
PASSED
#else
NOT PASSED
#endif

// Check modulo result
#if YY == 0
PASSED
#else
NOT PASSED
#endif

