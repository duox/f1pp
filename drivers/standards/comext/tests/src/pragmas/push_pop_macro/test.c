#define A "1"
#pragma push_macro("A")
#undef A	// pseudo-undef
#define A "2"
#pragma message(A)
#pragma pop_macro("A")
#pragma message(A)

/*#pragma push_macro("A")
#define A "1"
#pragma pop_macro("A")
#pragma message(A)*/

/*#define A	"1"
A
#pragma push_macro("A")
#undef A
#define A(x)	"2"
A(1)
#pragma pop_macro("A")
A*/

/*#define AB	1
#pragma push_macro("AB")
#define AB	2*/

/*#pragma push_macro("A")
#define A "2"
#pragma message(A)
#pragma pop_macro("A")
#pragma message(A)*/

/*#define AB	"1"
#pragma push_macro("AB")
#pragma message(AB)
#pragma pop_macro("AB")
#pragma message(AB)
#pragma pop_macro("AB")
#pragma message(AB)*/

/*#define AB	"1"
#pragma pop_macro("AB")*/

