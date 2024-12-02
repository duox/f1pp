/*pp.syntax.clang-config.h*/
/** @file
 *
 * @brief clang default configuration file.
 *
 */

#ifndef F1_PP_STANDARD_CLANG_CONFIG_H_B971F328837DF241A0B32CEF45432EA1_INCLUDED
#define F1_PP_STANDARD_CLANG_CONFIG_H_B971F328837DF241A0B32CEF45432EA1_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_STANDARD_CLANG_DYNAMIC

# define F1_PP_STANDARD_CLANG_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CLANG: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STANDARD_CLANG_STATIC

# define F1_PP_STANDARD_CLANG_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CLANG: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_STANDARD_CLANG_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CLANG: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_STANDARD_CLANG__CONFIG__DUMP__
# define __F1_PP_STANDARD_CLANG__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_STANDARD_CLANG__CONFIG__DUMP__

#endif /* ndef F1_PP_STANDARD_CLANG_CONFIG_H_B971F328837DF241A0B32CEF45432EA1_INCLUDED */
/*END OF pp.syntax.clang-config.h*/
