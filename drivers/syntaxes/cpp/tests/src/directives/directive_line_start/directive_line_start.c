// Base definition
#define A


// Defines with spaces
 #define B

# define C

 # define D


 // Defines with comments
/**/#define E

#/**/define F

/**/#/**/define G


// Defines with comments and spaces at left
 /**/#define H

# /**/define I

 /**/# /**/define J


// Defines with comments and spaces at right
/**/ #define K

#/**/ define L

/**/ #/**/ define M


// Defines with comments and spaces at both sides
 /**/ #define N

# /**/ define O

 /**/ # /**/ define P


// Test output
#if defined(A) && \
	defined(B) && defined(C) && defined(D) && \
	defined(E) && defined(F) && defined(G) && \
	defined(H) && defined(I) && defined(J) && \
	defined(K) && defined(L) && defined(M) && \
	defined(N) && defined(O) && defined(P)
PASSED
#else
NOT PASSED
#endif
