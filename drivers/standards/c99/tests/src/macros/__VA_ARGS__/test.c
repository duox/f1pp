
#define va1(...)		__VA_ARGS__
va1(1, 2, 3)

#define va2(...)		({ __VA_ARGS__ })
va2(int x = 0, x += y, x)

#define va3(...)		#__VA_ARGS__
va3(1, 2, 3)

// Indirect versions
#define iva1(...)		va1( __VA_ARGS__ )
iva1(1, 2, 3)

#define iva2(...)		va2( __VA_ARGS__ )
iva2(int x = 0, x += y, x)

#define iva3(...)		va3( __VA_ARGS__ )
iva3(1, 2, 3)
