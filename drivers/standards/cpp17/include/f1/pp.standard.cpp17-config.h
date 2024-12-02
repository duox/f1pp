/*pp.syntax.cpp17-config.h*/
/** @file
 *
 * @brief C++17 preprocessor standard default configuration file.
 *
 */

#ifndef F1_PP_STANDARD_CPP17_CONFIG_H_3E48944004984ED5B8A7E50725A33D98_INCLUDED
#define F1_PP_STANDARD_CPP17_CONFIG_H_3E48944004984ED5B8A7E50725A33D98_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_STANDARD_CPP17_DYNAMIC

# define F1_PP_STANDARD_CPP17_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP17: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STANDARD_CPP17_STATIC

# define F1_PP_STANDARD_CPP17_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP17: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_STANDARD_CPP17_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP17: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_STANDARD_CPP17__CONFIG__DUMP__
# define __F1_PP_STANDARD_CPP17__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_STANDARD_CPP17__CONFIG__DUMP__

#endif /* ndef F1_PP_STANDARD_CPP17_CONFIG_H_3E48944004984ED5B8A7E50725A33D98_INCLUDED */
/*END OF pp.syntax.cpp17-config.h*/
