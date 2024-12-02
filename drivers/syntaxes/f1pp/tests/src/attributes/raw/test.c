/////////////////////
// #exec directive

#define A	1
#exec "CMD.exe /c echo A"
#[raw]exec "CMD.exe /c echo A"

/////////////////////
// #foreach directive

#foreach i:A, A, A
	i
#end

#[raw]foreach i:A, A, A
	i
#end
