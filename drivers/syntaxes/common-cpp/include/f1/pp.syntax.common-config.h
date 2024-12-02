/*pp.syntax.common-config.h*/
/** @file
 *
 * @brief PP common extensions default configuration file.
 *
 */

#ifndef F1_PP_SYNTAX_COMMON_CONFIG_H_B2ACEB385BB5423AA1E618C1D92DF036_INCLUDED
#define F1_PP_SYNTAX_COMMON_CONFIG_H_B2ACEB385BB5423AA1E618C1D92DF036_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_SYNTAX_COMMON_DYNAMIC

# define F1_PP_SYNTAX_COMMON_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.SYNTAX.COMMON: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_SYNTAX_COMMON_STATIC

# define F1_PP_SYNTAX_COMMON_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.SYNTAX.COMMON: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_SYNTAX_COMMON_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.SYNTAX.COMMON: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_SYNTAX_COMMON__CONFIG__DUMP__
# define __F1_PP_SYNTAX_COMMON__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_SYNTAX_COMMON__CONFIG__DUMP__

#endif /* ndef F1_PP_SYNTAX_COMMON_CONFIG_H_B2ACEB385BB5423AA1E618C1D92DF036_INCLUDED */
/*END OF pp.syntax.common-config.h*/
