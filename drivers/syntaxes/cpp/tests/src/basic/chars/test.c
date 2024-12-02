
// Check ASCII codes
#if '1' == 0x31 && 'Z' == 0x5A
PASSED
#else
NOT PASSED
#endif

// Check character omission
#if 0
`@$\`"\`"
#endif

// Multi-character character constant
#if 'ab' == '\x61\x62'
PASSED
#else
NOT PASSED
#endif

#if '\aa' == '\7\x61'
PASSED
#else
NOT PASSED
#endif

// Multi-character wide character constant
#if L'ab' == L'\x61\x62' 
PASSED
#else
NOT PASSED
#endif

#if L'ab' != 'ab' 
PASSED
#else
NOT PASSED
#endif
