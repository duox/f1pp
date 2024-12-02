
// Digraphs
<:	// [
:>	// ]
<%	// {
%>	// }
%:	// #	(empty preprocessor operator, shouldn't get to output)

//// Trigraphs ////

// #	(empty preprocessor operator, shouldn't get to output)
??=
// \	(new line screening, should be deleted at early stage of preprocessing)
??/
// ^
??'
// [
??(
// ]
??)
// |
??!
// {
??<
// }
??>
// ~
??-

// Sample directives
 /**/ ??= /**/ define /**/ BIT_OR( /**/ a /**/ , /**/ b /**/ ) /**/ (a) /**/ ??! /**/ (b) /**/
 /**/ ??= if 15 == BIT_OR( BIT_OR( 1, 2 ), BIT_OR( 4, 8 ) ) 
PASSED
 /**/ ??= else
NOT PASSED
 /**/ ??= endif