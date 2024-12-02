/*test.c*/

#if ~0ull == 0u + ~0u 
// C++11 preprocessor arithmetic! 0u has the same representation as 0ull,\
// hence ~0ull == 0u + ~0u 
1:VALID
#else 
// non-C++11 preprocessor arithmetic. 0ul does not have the same \
// representation as 0ull, hence ~0ull != 0u + ~0u 
1:INVALID
#endif


// The upper limit of the #line <integer> preprocessor directives
// has been increased from 32,767 to 2,147,483,647 for the C++11
// preprocessor in conformance with the C99 preprocessor.
#line 1000000   //Valid in C++11, but invalid in C++98
__LINE__

/*END OF test.c*/
