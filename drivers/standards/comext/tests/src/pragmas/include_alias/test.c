
// Intendent usage
#include "subfolder/print_msg.h"
#pragma /**/ include_alias /**/ ( /**/ "msg.h" /**/ , /**/ "subfolder/print_msg.h" /**/ ) /**/
#include "msg.h"
#pragma include_\
	alias("msg2.h",\
	"../include_alias/subfolder/print_msg.h"\
	)
#include <msg2.h>

// Invalid syntax
#pragma include_alias
#pragma include_alias()
#pragma include_alias(,)
#pragma include_alias("")
#pragma include_alias("subfolder/print_msg.h")
#pragma include_alias("subfolder/print_msg.h","")
#pragma include_alias(,"")
#pragma include_alias(,"subfolder/print_msg.h")
#pragma include_alias("","subfolder/print_msg.h")
