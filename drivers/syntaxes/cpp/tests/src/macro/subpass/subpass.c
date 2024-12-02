
#define A(x)	int a = x;

// Non-broken arglist
#define B		A
B (0)

// Broken arglist 1
#define C		A(
C 0)

// Broken arglist 2
#define D		A(0
D )

