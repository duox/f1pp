// RUN: %clang_cc1 -E %s | grep '1: aaab 2'
// RUN: %clang_cc1 -E %s | grep '2: 2 baaa'
// RUN: %clang_cc1 -E %s | grep '3: 2 xx'

#define a(n) aaa ## n
#define b 2
1: a(b b)   // aaab 2    second `b' gets expanded, not the first one

#undef a
#undef b
#define a(n) n ## aaa
#define b 2
2: a(b b)   // 2 baaa    first `b' gets expanded, not the second one

#define baaa xx
3: a(b b)   // 2 xx

#define A(B,C) B ## C

4: !A(,)!

