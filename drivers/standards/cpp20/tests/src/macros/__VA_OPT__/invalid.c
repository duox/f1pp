/*invalid.c*/

// No argument to __VA_OPT__
#define INV(x, ...)		x __VA_OPT__
INV(1)

/*END OF invalid.c*/
