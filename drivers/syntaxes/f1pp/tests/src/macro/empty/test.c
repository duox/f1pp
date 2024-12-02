
#define A	_Pragma("message(\"A\")")
#define A0
#define AA	AA

#if empty A		// A is empty (_Pragma disappears)
A
#endif
#if empty A0	// A0 is empty
A
#endif
#if empty AA	// AA is not empty
AA
#endif
