/*pp.output.nullout-config.h*/
/** @file
 *
 * @brief PP default configuration file.
 *
 */

#ifndef F1_PP_OUTPUT_NULLOUT_CONFIG_H_F5F470C5724ACB46B079FFC8ADC32C54_INCLUDED
#define F1_PP_OUTPUT_NULLOUT_CONFIG_H_F5F470C5724ACB46B079FFC8ADC32C54_INCLUDED


// Detect correct interaction
#ifdef F1_PP_OUTPUT_NULLOUT_DYNAMIC

# define F1_PP_OUTPUT_NULLOUT_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.OUTPUT.NULLOUT: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_OUTPUT_NULLOUT_STATIC

# define F1_PP_OUTPUT_NULLOUT_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.OUTPUT.NULLOUT: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_OUTPUT_NULLOUT_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.OUTPUT.NULLOUT: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_OUTPUT_NULLOUT__CONFIG__DUMP__
# define __F1_PP_OUTPUT_NULLOUT__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_OUTPUT_NULLOUT__CONFIG__DUMP__

#endif /* ndef F1_PP_OUTPUT_NULLOUT_CONFIG_H_F5F470C5724ACB46B079FFC8ADC32C54_INCLUDED */
/*END OF pp.output.nullout-config.h*/
