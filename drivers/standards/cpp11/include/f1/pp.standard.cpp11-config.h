/*pp.syntax.cpp11-config.h*/
/** @file
 *
 * @brief PP common extensions default configuration file.
 *
 */

#ifndef F1_PP_STANDARD_CPP11_CONFIG_H_3E48944004984ED5B8A7E50725A33D98_INCLUDED
#define F1_PP_STANDARD_CPP11_CONFIG_H_3E48944004984ED5B8A7E50725A33D98_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_STANDARD_CPP11_DYNAMIC

# define F1_PP_STANDARD_CPP11_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP11: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STANDARD_CPP11_STATIC

# define F1_PP_STANDARD_CPP11_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP11: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_STANDARD_CPP11_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP11: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_STANDARD_CPP11__CONFIG__DUMP__
# define __F1_PP_STANDARD_CPP11__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_STANDARD_CPP11__CONFIG__DUMP__

#endif /* ndef F1_PP_STANDARD_CPP11_CONFIG_H_3E48944004984ED5B8A7E50725A33D98_INCLUDED */
/*END OF pp.syntax.cpp11-config.h*/
