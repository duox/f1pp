/*pp.syntax.c11-config.h*/
/** @file
 *
 * @brief PP C11 extensions default configuration file.
 *
 */

#ifndef F1_PP_STANDARD_C11_CONFIG_H_DE2D5907DAD64781B4E29507C1911BD2_INCLUDED
#define F1_PP_STANDARD_C11_CONFIG_H_DE2D5907DAD64781B4E29507C1911BD2_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_STANDARD_C11_DYNAMIC

# define F1_PP_STANDARD_C11_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.C11: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STANDARD_C11_STATIC

# define F1_PP_STANDARD_C11_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.C11: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_STANDARD_C11_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.C11: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_STANDARD_C11__CONFIG__DUMP__
# define __F1_PP_STANDARD_C11__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_STANDARD_C11__CONFIG__DUMP__

#endif /* ndef F1_PP_STANDARD_C11_CONFIG_H_DE2D5907DAD64781B4E29507C1911BD2_INCLUDED */
/*END OF pp.syntax.c11-config.h*/
