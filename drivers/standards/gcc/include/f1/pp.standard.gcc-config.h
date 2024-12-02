/*pp.syntax.gcc-config.h*/
/** @file
 *
 * @brief GCC default configuration file.
 *
 */

#ifndef F1_PP_STANDARD_GCC_CONFIG_H_F8B9B734D1134A4AAB03CF6932742C2B_INCLUDED
#define F1_PP_STANDARD_GCC_CONFIG_H_F8B9B734D1134A4AAB03CF6932742C2B_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_STANDARD_GCC_DYNAMIC

# define F1_PP_STANDARD_GCC_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.GCC: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STANDARD_GCC_STATIC

# define F1_PP_STANDARD_GCC_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.GCC: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_STANDARD_GCC_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.GCC: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_STANDARD_GCC__CONFIG__DUMP__
# define __F1_PP_STANDARD_GCC__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_STANDARD_GCC__CONFIG__DUMP__

#endif /* ndef F1_PP_STANDARD_GCC_CONFIG_H_F8B9B734D1134A4AAB03CF6932742C2B_INCLUDED */
/*END OF pp.syntax.gcc-config.h*/
