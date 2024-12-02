/*pch.h*/

#include <f1/core.h>
#include <f1/mt.h>
#include <f1/mt.windows.h>

#include <f1/pp.h>

#ifndef _DEV_PCH
# include <f1/pp.dbg.h>
#endif	/* ndef _DEV_PCH */

#define STR_LEN_PAIR(s)		s, countof(s) - 1

/*END OF pch.h*/
