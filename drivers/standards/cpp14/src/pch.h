/*pch.h*/

#include <f1/core.h>
#include <f1/msgout.h>
#include <f1/ns.h>
//#include <f1/io.h>
//#include <f1/fs.h>
#include <f1/unicode.h>
//#include <f1/util.h>
#include <f1/parser.h>

#include <f1/pp.h>

#include <f1/pp.standard.c11.h>
#include <f1/pp.standard.cpp11.h>

#include <hash_set>

#ifndef _DEV_PCH
# include "../include/f1/pp.standard.cpp14.h"
#endif // ndef _DEV_PCH

/*END OF pch.h*/
