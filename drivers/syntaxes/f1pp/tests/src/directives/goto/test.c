#if 1
#:label1
#pragma message( "AAA" )
#endif
#goto label2
#pragma message( "BBB" )
#:label2
#pragma message( "CCC" )

/*#i = 0
#for i = 0; i < 4; ++ i
# repeat j:4
//# for j = 0; j < 1; ++ j
#:label
#  print "i, j = " i, j
# endrepeat
//# endfor
#endfor
#print "----"
#i = 1
#goto label*/
