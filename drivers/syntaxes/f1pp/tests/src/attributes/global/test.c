/*test.c*/
/** @file
 * @brief The `global' attribute test.
 */

// `#alias' directive support
#namespace NS
# define D1		1
# define D2		2
# [global]alias AX = D1
#		  alias AY = D2
#end

NS.AX AX NS.AY AY

// `#concat' directive support
#namespace NS
# [global]concat CX "1"
#		  concat CY "2"
#end

NS.CX CX NS.CY CY

// `#define' directive support
#namespace NS
# [global]define DX	1
#		  define DY	2
#end

NS.DX DX NS.DY DY

// `#set' directive support
#namespace NS
# [global]set SX	1
#		  set SY	2
#end

NS.SX SX NS.SY SY

// `#enum' directive support
#namespace NS
# [global]enum EX
	A
# end
#		  enum EY
	B
# end
#end

NS.EX.A EX.A NS.EY.B EY.B

/*END OF test.c*/
