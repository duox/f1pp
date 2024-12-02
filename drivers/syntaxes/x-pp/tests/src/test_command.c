/*test_command.c*/

#command dSAY( xpos, ypos, text, color:OPT ) => @ xpos, ypos SAY text [COLOR color] #$nl 
	dSetPos( xpos, ypos );
	#ifdef color
		dSetColor( color );
	#endif
	dPrint( text );
#end command

@ 10, 10 SAY "text"

/*END OF test_command.c*/
