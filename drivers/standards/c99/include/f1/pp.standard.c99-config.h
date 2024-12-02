/*pp.syntax.c99-config.h*/
/** @file
 *
 * @brief PP common extensions default configuration file.
 *
 */

#ifndef F1_PP_STANDARD_C99_CONFIG_H_9F4BC9B22AA54D2F91A5D8C01D468E80_INCLUDED
#define F1_PP_STANDARD_C99_CONFIG_H_9F4BC9B22AA54D2F91A5D8C01D468E80_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_STANDARD_C99_DYNAMIC

# define F1_PP_STANDARD_C99_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.C99: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STANDARD_C99_STATIC

# define F1_PP_STANDARD_C99_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.C99: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_STANDARD_C99_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.C99: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_STANDARD_C99__CONFIG__DUMP__
# define __F1_PP_STANDARD_C99__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_STANDARD_C99__CONFIG__DUMP__

#endif /* ndef F1_PP_STANDARD_C99_CONFIG_H_9F4BC9B22AA54D2F91A5D8C01D468E80_INCLUDED */
/*END OF pp.syntax.c99-config.h*/
