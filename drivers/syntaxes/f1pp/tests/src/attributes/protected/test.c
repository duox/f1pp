//test.c

// #concat directive
[#concat]
CX
#[protected]concat CX	1
CX
#undef CX
CX	// CX should remain 1

// #define directive
[#define]
DX
#[protected]define DX	1
DX
#undef DX
DX	// DX should remain 1

// #enum directive
[#enum]
ENX.A
#enum ENX
	[protected]A=1
#end
ENX.A
#undef ENX.A
ENX.A	// EX should remain 1

// #env directive (see test definition where F1_PP_TEST_ENV is defined as 1)
[#env]
EN
#[protected]env EN	"F1_PP_TEST_ENV"
EN
#undef EN
EN	// EX should remain 1

// #exec directive
[#exec]
EX
#[protected]exec EX	= "cmd.exe /c echo 1"
EX
#undef EX
EX	// EX should remain 1

// #format directive
[#format]
FX
#[protected]format FX = "%x" ( 1 )
FX
#undef FX
FX	// FX should remain 1

// #function directive
[#function]
FUNC()
#[protected]function FUNC()
	return 1
#end function
FUNC()
#undef FUNC
FUNC()	// FUNC should remain 1

// #macro directive
[#macro]
MX
#[protected]macro MX
	1
#end macro
MX
#undef MX
MX	// FX should remain 1

// #set directive
[#set]
SX
#[protected]set SX	1
SX
#set SX	2
SX	// SX should remain 1
#undef SX
SX

// #shell directive
[#shell]
SHX
#[protected]shell SHX	= "echo 1"
SHX
#undef SHX
SHX	// SHX should remain 1
