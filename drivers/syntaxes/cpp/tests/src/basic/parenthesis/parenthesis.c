#define A	(,)
A

#define B	(a)(b)
B

#define C	((a)(b))
C

#define D(a)	((a)(b))
D(1)

#define E(a)	D( (a) )
E(1)

#define F(a, b)	D( (a, b) )
F(1, 2)

