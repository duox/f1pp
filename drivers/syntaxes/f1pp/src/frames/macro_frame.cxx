/*macro_frame.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::macro_frame::macro_frame( f1pp::parser & parser ):
	super( parser )
{
}

// Return resulting value
const status_t f1::f1pp::macro_frame::enter()
{
	return super::enter();
}
const status_t f1::f1pp::macro_frame::leave()
{
	return super::leave();
}

/*END OF macro_frame.cxx*/
