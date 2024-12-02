#define A(s) s##s##s##s##s##s##s##s
#define B(s) A(s##s##s##s##s##s##s##s)
#define C(s) B(s##s##s##s##s##s##s##s)
#define D(s) C(s##s##s##s##s##s##s##s)
#define E(s) D(s##s##s##s##s##s##s##s)
#define F(s) E(s##s##s##s##s##s##s##s)
#define G(s) F(s##s##s##s##s##s##s##s)
a G(foo)