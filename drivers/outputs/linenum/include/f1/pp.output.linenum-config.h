/*pp.output.linenum-config.h*/
/** @file
 *
 * @brief PP default configuration file.
 *
 */

#ifndef F1_PP_OUTPUT_LINENUM_CONFIG_H_FD255F86CA4C42288C83FCB0EB8926BB_INCLUDED
#define F1_PP_OUTPUT_LINENUM_CONFIG_H_FD255F86CA4C42288C83FCB0EB8926BB_INCLUDED


// Detect correct interaction
#ifdef F1_PP_OUTPUT_LINENUM_DYNAMIC

# define F1_PP_OUTPUT_LINENUM_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.OUTPUT.LINENUM: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_OUTPUT_LINENUM_STATIC

# define F1_PP_OUTPUT_LINENUM_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.OUTPUT.LINENUM: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_OUTPUT_LINENUM_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.OUTPUT.LINENUM: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_OUTPUT_LINENUM__CONFIG__DUMP__
# define __F1_PP_OUTPUT_LINENUM__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_OUTPUT_LINENUM__CONFIG__DUMP__

#endif /* ndef F1_PP_OUTPUT_LINENUM_CONFIG_H_FD255F86CA4C42288C83FCB0EB8926BB_INCLUDED */
/*END OF pp.output.linenum-config.h*/
