/*pp.syntax.comext-config.h*/
/** @file
 *
 * @brief PP common extensions default configuration file.
 *
 */
#ifndef F1_PP_STANDARD_COMEXT_CONFIG_H_09AE081049B34D5CB9B538CA891399BD_INCLUDED
#define F1_PP_STANDARD_COMEXT_CONFIG_H_09AE081049B34D5CB9B538CA891399BD_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_STANDARD_COMEXT_DYNAMIC

# define F1_PP_STANDARD_COMEXT_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.COMEXT: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STANDARD_COMEXT_STATIC

# define F1_PP_STANDARD_COMEXT_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.COMEXT: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_STANDARD_COMEXT_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.COMEXT: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_STANDARD_COMEXT__CONFIG__DUMP__
# define __F1_PP_STANDARD_COMEXT__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_STANDARD_COMEXT__CONFIG__DUMP__

#endif /* ndef F1_PP_STANDARD_COMEXT_CONFIG_H_09AE081049B34D5CB9B538CA891399BD_INCLUDED */
/*END OF pp.syntax.comext-config.h*/
