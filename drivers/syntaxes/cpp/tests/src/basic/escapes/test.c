//test.c
/**
 * @brief Escape test.
 *
 *	Note that this test expects that the char is signed. Otherwise, each integer constant in
 * comparisons must be suffixed with 'U' or 'u' suffix.
 *
 */


// Escape forms 
#if '\1' == 1
PASSED
#else
NOT PASSED
#endif

#if L'\x{1234}' == 0x1234
PASSED
#else
NOT PASSED
#endif

// Escape codes
#if '\0' == 0 && '\x00' == 0
PASSED
#else
NOT PASSED
#endif

#if '\07' == 0x7 && '\x07' == 0x7 && '\a' == 7
PASSED
#else
NOT PASSED
#endif

#if '\10' == 0x8 && '\x08' == 0x8 && '\b' == 8
PASSED
#else
NOT PASSED
#endif

#if '\11' == 0x9 && '\x09' == 0x9 && '\t' == 9
PASSED
#else
NOT PASSED
#endif

#if '\12' == 0xA && '\x0A' == 0xA && '\n' == 10
PASSED
#else
NOT PASSED
#endif

#if '\13' == 0xB && '\x0B' == 0xB && '\v' == 11
PASSED
#else
NOT PASSED
#endif

#if '\14' == 0xC && '\x0C' == 0xC && '\f' == 12
PASSED
#else
NOT PASSED
#endif

#if '\15' == 0xD && '\x0D' == 0xD && '\r' == 13
PASSED
#else
NOT PASSED
#endif

#if '\134' == 0x5c && '\x05C' == 0x5c && '\\' == 92
PASSED
#else
NOT PASSED
#endif

#if '\77' == 0x3f && '\x03F' == 0x3f && '\?' == 63
PASSED
#else
NOT PASSED
#endif

#if '\47' == 0x27 && '\x027' == 0x27 && '\'' == 39
PASSED
#else
NOT PASSED
#endif

#if '\42' == 0x22 && '\x022' == 0x22 && '\"' == 34
PASSED
#else
NOT PASSED
#endif

// Check numeric escapes merging
#if '\08' == 0x38
PASSED
#else
NOT PASSED
#endif

#if '\x000Z' == 0x5A
PASSED
#else
NOT PASSED
#endif

// Check that hexadecimal constant is not limited by the number of digits
#if '\x0008' == 0x08
PASSED
#else
NOT PASSED
#endif

// Check numeric escapes
#if '\400' != 0x100		// warning: value is too big for a narrow character
PASSED
#else
NOT PASSED
#endif

#if L'\400' == 0x100	// no warning for wide character
PASSED
#else
NOT PASSED
#endif

// Check escapes in strings
"ab\"cd\"ef"
_Pragma("message(\"B\")")
