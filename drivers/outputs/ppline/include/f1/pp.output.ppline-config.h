/*pp.output.ppline-config.h*/
/** @file
 *
 * @brief PP default configuration file.
 *
 */

#ifndef F1_PP_OUTPUT_PPLINE_CONFIG_H_ED076E9B25CE410892B16DB2C2E14DDB_INCLUDED
#define F1_PP_OUTPUT_PPLINE_CONFIG_H_ED076E9B25CE410892B16DB2C2E14DDB_INCLUDED


// Detect correct interaction
#ifdef F1_PP_OUTPUT_PPLINE_DYNAMIC

# define F1_PP_OUTPUT_PPLINE_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.OUTPUT.PPLINE: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_OUTPUT_PPLINE_STATIC

# define F1_PP_OUTPUT_PPLINE_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.OUTPUT.PPLINE: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_OUTPUT_PPLINE_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.OUTPUT.PPLINE: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_OUTPUT_PPLINE__CONFIG__DUMP__
# define __F1_PP_OUTPUT_PPLINE__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_OUTPUT_PPLINE__CONFIG__DUMP__

#endif /* ndef F1_PP_OUTPUT_PPLINE_CONFIG_H_ED076E9B25CE410892B16DB2C2E14DDB_INCLUDED */
/*END OF pp.output.ppline-config.h*/
