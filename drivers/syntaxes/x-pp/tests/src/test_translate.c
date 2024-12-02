/*test_translate.c*/

#translate dBase_SAY(xpos, ypos, text) => @ xpos, ypos #[SAY text#]
	SetPos( xpos, ypos );
	Print( text );
#end translate

@ 10, 10 SAY "text"

/*END OF test_translate.c*/
