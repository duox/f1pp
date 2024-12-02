/*pch.h*/
#define STRICT
#define _CRT_SECURE_NO_WARNINGS			0
#define _ALLOW_RTCc_IN_STL
#include <windows.h>
#include <shlwapi.h>


#define _DEFINE_DEPRECATED_HASH_CLASSES 0
#define _CRT_SECURE_NO_WARNINGS			0

#ifdef __cplusplus
# ifndef _CPPUNWIND
#  define _HAS_EXCEPTIONS  0
# endif
#endif

//#include <windows.h>

#include <stdint.h>

/*#include <exception>
#include <list>
#include <hash_map>
#include <fstream>
#include <string>

#include <malloc.h>
#include <stdarg.h>
#include <crtdbg.h>
*/
#include <libcext.h>
#include <f1/core.h>
#include <f1/util.h>

//#ifndef _FINAL
# include <f1/pp.h>
# include <f1/pp.mcpp.h>
//#endif	/* ndef _FINAL */

/*END OF pch.h*/
