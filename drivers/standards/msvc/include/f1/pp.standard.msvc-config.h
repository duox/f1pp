/*pp.syntax.msvc-config.h*/
/** @file
 *
 * @brief PP common extensions default configuration file.
 *
 */

#ifndef F1_PP_STANDARD_MSVC_CONFIG_H_80ACE64D0991455C8483C9B1DABFBE95_INCLUDED
#define F1_PP_STANDARD_MSVC_CONFIG_H_80ACE64D0991455C8483C9B1DABFBE95_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_STANDARD_MSVC_DYNAMIC

# define F1_PP_STANDARD_MSVC_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.MSVC: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STANDARD_MSVC_STATIC

# define F1_PP_STANDARD_MSVC_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.MSVC: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_STANDARD_MSVC_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.MSVC: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_STANDARD_MSVC__CONFIG__DUMP__
# define __F1_PP_STANDARD_MSVC__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_STANDARD_MSVC__CONFIG__DUMP__

#endif /* ndef F1_PP_STANDARD_MSVC_CONFIG_H_80ACE64D0991455C8483C9B1DABFBE95_INCLUDED */
/*END OF pp.syntax.msvc-config.h*/
