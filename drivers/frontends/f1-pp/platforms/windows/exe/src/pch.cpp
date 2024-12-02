/*pch.cpp*/
#include "pch.h"

#pragma comment(exestr,"\n\n")
#pragma comment(exestr,"Description: F1 preprocessor front-end.\n")
#pragma comment(exestr,"Time&date stamp: "__DATE__" "__TIME__".\n")
#pragma comment(compiler)

#if defined (_DEBUG)
 #if defined (_UNICODE)
  // debug unicode
  #pragma comment(exestr,"Compile: Debug UNICODE\n")
 #else
  // debug multibyte
  #pragma comment(exestr,"Compile: Debug MBCS\n")
 #endif
#else
 #pragma comment(linker,"/MAPINFO:EXPORTS")
 #pragma comment(linker,"/MAPINFO:FIXUPS")
 #pragma comment(linker,"/MAPINFO:LINES")
 #pragma comment(linker,"/RELEASE")
 #pragma comment(linker,"/MERGE:.CRT=.text")
 #if defined (_DEBUG_SUPPORT)
  #if defined (_UNICODE)
   // debug support unicode
   #pragma comment(exestr,"Compile: DebugSupport UNICODE\n")
  #else
   // debug support multibyte
   #pragma comment(exestr,"Compile: DebugSupport MBCS\n")
  #endif
 #else
  #if defined (_UNICODE)
   // release unicode
   #pragma comment(exestr,"Compile: Release UNICODE\n")
  #else
   // release multibyte
   #pragma comment(exestr,"Compile: Release MBCS\n")
  #endif
 #endif
#endif

/*END OF pch.cpp*/
