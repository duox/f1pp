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
#include <f1/pp.standard.c99.h>
#include <f1/pp.standard.clang.h>
#include <f1/pp.standard.gcc.h>
#include <f1/pp.standard.msvc.h>

#include <hash_set>

#ifndef _DEV_PCH
# include "../include/f1/pp.syntax.cpp.h"
#endif // ndef _DEV_PCH

/*#define	get_maximum_include_depth()					get_limit_value( f1::pp::limit_t::li_maximum_include_depth )
#define	get_maximum_identifier_length()				get_limit_value( f1::pp::limit_t::li_maximum_identifier_length )
#define	get_maximum_macro_count()					get_limit_value( f1::pp::limit_t::li_maximum_macro_count )
#define	get_maximum_macro_parameter_count()			get_limit_value( f1::pp::limit_t::li_maximum_macro_parameter_count )
#define	get_maximum_macro_argument_count()			get_limit_value( f1::pp::limit_t::li_maximum_macro_argument_count )
#define	get_maximum_charconst_length()				get_limit_value( f1::pp::limit_t::li_maximum_charconst_length )
#define	get_maximum_default_error_maximum_count()	get_limit_value( f1::pp::limit_t::li_default_error_maximum_count )
#define	get_maximum_default_warning_maximum_count()	get_limit_value( f1::pp::limit_t::li_default_warning_maximum_count )
*/
/*END OF pch.h*/
