
#if 1 + 2 * 9 + 5 * 6 == 49
1:PASSED
#endif

//#define X
//#if 1
//#if defined X
//#if 1 || (2/0 || 1)
//#if 0 && (0 || 1)
#if 8 == 2*(2 + 2)
//#if 1U ? 0 : 1
//#if /**/ /**/ -/**/+/**/1L
/*#if 1\
	-\
	1*/
2:PASSED
#else
2:NOTPASSED
#endif

#if 1 <= 4
3:PASSED
#else
3:NOTPASSED
#endif

/*#if 1 > "str"
3:PASSED
#else
3:NOTPASSED
#endif*/
