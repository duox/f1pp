/*main.h*/

#ifdef _DEV_PCH
# include "../include/f1/pp.syntax.cpp.h"
#endif // def _DEV_PCH

#define STR_LEN_PAIR( str )		str, countof(str) - 1

#define DBG_INFO_argparse( fmt )		//DBG_PRINTF( fmt )
#define DBG_INFO_breakpoints( str )		//DBG_PRINT( "BREAKPOINTS: " str "\n" )
#define DBG_INFO_capture( fmt )			//DBG_PRINTF( fmt )
#define DBG_INFO_patch( fmt )			//DBG_PRINTF( fmt )
#define DBG_INFO_allow_unexpanded_output( str, val )	//DBG_PRINTF(( str ": m_allow_args_separators = %s\n", val ? "true" : "false" ))

/*END OF main.h*/
