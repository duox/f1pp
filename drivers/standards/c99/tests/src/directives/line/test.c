//test.c

/////////////////////////////////////////////////////////////////
// Valid data zone

#line 10 "line.c"
#if __LINE__ == 10
# line 9 "test.c"		// restore line numbering
VAL.1: PASSED
#else
# line 12 "test.c"		// restore line numbering
VAL.1: NOT PASSED
#endif

/////////////////////////////////////////////////////////////////
// Potentionally invalid data zone



/////////////////////////////////////////////////////////////////
// Invalid data zone

// no line number
#line
#line "file"

// unterminated string
#line 29 "file
