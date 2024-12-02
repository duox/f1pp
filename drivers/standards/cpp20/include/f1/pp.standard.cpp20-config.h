/*pp.syntax.cpp20-config.h*/
/** @file
 *
 * @brief C++20 preprocessor additions default configuration file.
 *
 */

#ifndef F1_PP_STANDARD_CPP20_CONFIG_H_5859BF4668DD974D92F18F2FDE37EE01_INCLUDED
#define F1_PP_STANDARD_CPP20_CONFIG_H_5859BF4668DD974D92F18F2FDE37EE01_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_STANDARD_CPP20_DYNAMIC

# define F1_PP_STANDARD_CPP20_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP20: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STANDARD_CPP20_STATIC

# define F1_PP_STANDARD_CPP20_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP20: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_STANDARD_CPP20_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP20: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_STANDARD_CPP20__CONFIG__DUMP__
# define __F1_PP_STANDARD_CPP20__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_STANDARD_CPP20__CONFIG__DUMP__

#endif /* ndef F1_PP_STANDARD_CPP20_CONFIG_H_5859BF4668DD974D92F18F2FDE37EE01_INCLUDED */
/*END OF pp.syntax.cpp20-config.h*/
