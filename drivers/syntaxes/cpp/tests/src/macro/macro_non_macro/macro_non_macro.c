//macro_non_macro.c

#define a(x)	b##x
a(0)
a /**/ (0)
a
a /**/
a(0)
a

a
#define b
(0)

a
#if 0
(0)
#else
"(0)"
#endif
