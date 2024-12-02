//encoding.c
/** @file
 *
 * @brief Encoding test.
 *
 *	f1pp must be called with --merge-strings option enabled to generate expected results. Otherwise, mixed
 * encoding warnings might not be issued. Note that normally string merging is performed after preprocessing
 * stage so it might be expected that other preprocessor won't merge strings.
 *
 *	The "E" ending in the end of some lines prevents unwanted strings merging.
 *
 *	All mixed encoding merge must generate corresponding warning.
 */

// General strings
// String tokens separation tests
"A" "B" E
"A"/**/"B" E
"A"
"B" E

// "L" prefix (wide characters)
"A"/**/L"B" E
L"A"/**/"B" E
L"A"/**/L"B" E

// "u8" prefix (UTF-8)
"A"/**/u8"B" E
u8"A"/**/"B" E
u8"A"/**/u8"B" E

// "u" prefix (UTF-16)
"A"/**/u"B" E
u"A"/**/"B" E
u"A"/**/u"B" E

// "U" prefix (UTF-32)
"A"/**/u8"B" E
u8"A"/**/"B" E
u8"A"/**/u8"B" E

END
//END OF encoding.c
