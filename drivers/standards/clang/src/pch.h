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

#include <hash_set>

#include <f1/pp.standard.c99.h>
#include <f1/pp.standard.c23.h>
#include <f1/pp.standard.cpp23.h>
#include <f1/pp.standard.comext.h>

#ifndef _DEV_PCH
# include "../include/f1/pp.standard.clang.h"
#endif // ndef _DEV_PCH

/*END OF pch.h*/
