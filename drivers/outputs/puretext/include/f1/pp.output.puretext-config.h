/*pp.output.puretext-config.h*/
/** @file
 *
 * @brief PP default configuration file.
 *
 */

#ifndef F1_PP_OUTPUT_PURETEXT_CONFIG_H_B0B19D19367D4D8F9BB6DA8A83A541C3_INCLUDED
#define F1_PP_OUTPUT_PURETEXT_CONFIG_H_B0B19D19367D4D8F9BB6DA8A83A541C3_INCLUDED


// Detect correct interaction
#ifdef F1_PP_OUTPUT_PURETEXT_DYNAMIC

# define F1_PP_OUTPUT_PURETEXT_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.OUTPUT.PURETEXT: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_OUTPUT_PURETEXT_STATIC

# define F1_PP_OUTPUT_PURETEXT_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.OUTPUT.PURETEXT: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_OUTPUT_PURETEXT_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.OUTPUT.PURETEXT: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_OUTPUT_PURETEXT__CONFIG__DUMP__
# define __F1_PP_OUTPUT_PURETEXT__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_OUTPUT_PURETEXT__CONFIG__DUMP__

#endif /* ndef F1_PP_OUTPUT_PURETEXT_CONFIG_H_B0B19D19367D4D8F9BB6DA8A83A541C3_INCLUDED */
/*END OF pp.output.puretext-config.h*/
