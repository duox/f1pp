//LINE.c

A
// The folllowing directive should not change the line numbering
#line 6
__LINE__	// should evaluate to 6
B

C
// The folllowing directive should not change the line numbering
#line __LINE__
__LINE__	// should evaluate to 11
D

__LINE__
