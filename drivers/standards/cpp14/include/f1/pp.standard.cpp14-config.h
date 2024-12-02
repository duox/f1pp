/*pp.syntax.cpp14-config.h*/
/** @file
 *
 * @brief C++17 preprocessor additions default configuration file.
 *
 */

#ifndef F1_PP_STANDARD_CPP14_CONFIG_H_2EB720D7798E493A85B3D8E217AE3F55_INCLUDED
#define F1_PP_STANDARD_CPP14_CONFIG_H_2EB720D7798E493A85B3D8E217AE3F55_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_STANDARD_CPP14_DYNAMIC

# define F1_PP_STANDARD_CPP14_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP14: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STANDARD_CPP14_STATIC

# define F1_PP_STANDARD_CPP14_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP14: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_STANDARD_CPP14_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP14: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_STANDARD_CPP14__CONFIG__DUMP__
# define __F1_PP_STANDARD_CPP14__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_STANDARD_CPP14__CONFIG__DUMP__

#endif /* ndef F1_PP_STANDARD_CPP14_CONFIG_H_2EB720D7798E493A85B3D8E217AE3F55_INCLUDED */
/*END OF pp.syntax.cpp14-config.h*/
