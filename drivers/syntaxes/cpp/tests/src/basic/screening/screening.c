
// Normal screening
A
\
B
__LINE__	// should be 6
C\
D

// Pseudo-invalid screening (with spaces), should be processed as expected
E\ // comment
F

// Single pass
__LINE__	// should be 15
"A\
B\
C"
__LINE__	// should be 19

// Double pass
__LINE__	// should be 22
#pragma message( "A\
			B\
			C" )
__LINE__	// should be 26
