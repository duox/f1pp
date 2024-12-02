
#define CONCAT_( x, y ) x ## y
#define CONCAT( x, y ) CONCAT_( x, y )
#define IS_SINGLE_1(...) 0
#define IGNORE(...)
#define IS_SINGLE_2_0           0 IGNORE(
#define IS_SINGLE_2_IS_SINGLE_1 1 IGNORE(
#define IS_SINGLE( x ) CONCAT( IS_SINGLE_2_, IS_SINGLE_1 x ) )
IS_SINGLE((x, y)) // 0
IS_SINGLE(x)      // 1
