//valid.c
// This test contains f1.pp genuine test and some WG21 examples.

// Code below is taken (with modifications) from the following source
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1042r1.html

// Macro substitution checks
#define LPAREN() (
#define G(Q) 42
#define F(R, X, ...)  __VA_OPT__(G R X) )
int x = F(LPAREN(), 0, <:-);  // replaced by int x = 42;

#define GX(x, ...) f(0 __VA_OPT__(,) __VA_ARGS__)
#define EMP
GX(EMP)  // replaced by f(0)

#define H3(X, ...) #__VA_OPT__(X##X X##X)
H3(, 0)  // replaced by ""

#define H4(X, ...) __VA_OPT__(a X ## X) ## b
H4(, 1)  // replaced by a b

#define H5A(...) __VA_OPT__()/**/__VA_OPT__()
#define H5B(X) a ## X ## b
#define H5C(X) H5B(X)
H5C(H5A())  // replaced by ab

// Code below is taken (with modifications) from the following source:
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0306r4.html

// Using __VA_OPT__ to remove extra ','
#define LOG(msg, ...) printf(msg __VA_OPT__(,) __VA_ARGS__)
LOG("hello world");         // => printf("hello world");
LOG("hello world", );       // => printf("hello world");
LOG("hello %s", "world");   // => printf("hello %s", "world");

// Using __VA_OPT__ to add a piece of code
#define SDEF(sname, ...) S sname __VA_OPT__(= { __VA_ARGS__ })
SDEF(foo);           // => S foo;
SDEF(bar, 1, 2, 3);  // => S bar = { 1, 2, 3 };

#define LOG2(...)                                      \
    printf("at line=%d" __VA_OPT__(": "), __LINE__);  \
    __VA_OPT__(printf(__VA_ARGS__);)                  \
    printf("\n")
LOG2();                          // => printf("at line=%d", 46); printf("\n");
LOG2("All well in zone %n", n);  // => printf("at line=%d: ", 47); printf("All well in zone %n", n); printf("\n");
