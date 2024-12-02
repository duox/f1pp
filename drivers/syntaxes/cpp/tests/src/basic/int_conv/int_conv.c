
// If one operand is unsigned, the result is unsigned.
#if (1 - 2u > 0)	// Usual implicit type conversion.
1: PASSED
#else
1: NOTPASSED
#endif

// If any of integer suffixes is missing, the value is of integer type
#if 0 > 0xFFFFFFFFFFFFFFFF
2: PASSED
#else
2: NOTPASSED
#endif

// Simple pass
#if -1 < 0
3: PASSED
#else
3: NOTPASSED
#endif

// In intmax_t arithmetics, the 0xF..F represents -1
#if -1 == 0xFFFFFFFFFFFFFFFF
4: PASSED
#else
4: NOTPASSED
#endif

