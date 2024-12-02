/*pch.h*/

#include <f1/pp.h>
#include <f1/pp.syntax.cpp.h>

#include <f1/pp.standard.c99.h>
#include <f1/pp.standard.clang.h>
#include <f1/pp.standard.gcc.h>
#include <f1/pp.standard.msvc.h>

#ifndef _DEV_PCH
# include <f1/pp.syntax.f1pp.h>
#endif	/* ndef _DEV_PCH */

#define STR_LEN_PAIR(s)		s, countof(s) - 1

/*END OF pch.h*/
