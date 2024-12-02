/*pp.syntax.cpp98-config.h*/
/** @file
 *
 * @brief C++98 preprocessor standard default configuration file.
 *
 */

#ifndef F1_PP_STANDARD_CPP98_CONFIG_H_ADB88DFF406544ADBF6D9E52DC7C74C3_INCLUDED
#define F1_PP_STANDARD_CPP98_CONFIG_H_ADB88DFF406544ADBF6D9E52DC7C74C3_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_STANDARD_CPP98_DYNAMIC

# define F1_PP_STANDARD_CPP98_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP98: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STANDARD_CPP98_STATIC

# define F1_PP_STANDARD_CPP98_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP98: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_STANDARD_CPP98_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP98: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_STANDARD_CPP98__CONFIG__DUMP__
# define __F1_PP_STANDARD_CPP98__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_STANDARD_CPP98__CONFIG__DUMP__

#endif /* ndef F1_PP_STANDARD_CPP98_CONFIG_H_ADB88DFF406544ADBF6D9E52DC7C74C3_INCLUDED */
/*END OF pp.syntax.cpp98-config.h*/
