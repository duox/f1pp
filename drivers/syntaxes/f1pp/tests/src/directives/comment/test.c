/*test.c*/

// Different variations of nested comments
#comment AA		// named comment
aaa.aaa
# comment		// nested unnamed comment
aaa.bbb
# end			// end of nested unnamed comment
# comment BB	// nested named comment
aaa.ccc
#  end comment	// junk end of comment (need comment id), must be ignored
aaa.ddd
# end comment BB// end of nested named comment
aaa.eee
#end comment	// junk end of comment (need comment id), must be ignored
aaa.fff
#endcomment A	// junk end of named comment (partial name, must be ignored)
aaa.ggg
//#endcomment AAA	// junk end of named comment (bigger name, must be ignored)
aaa.hhh
#endcomment AA	// !!! end of named comment
000

// Ignoring nested C/C++ comments
#comment BB
bbb.aaa
/*
#end comment BB
*/
bbb.bbb
#end comment BB
001

/*END OF test.c*/
