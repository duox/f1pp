/*pp.syntax.f1pp-config.h*/
/** @file
 *
 * @brief F1.PP.SYNTAX.F1PP default configuration file.
 *
 */

#ifndef F1_SYNTAX_F1PP_CONFIG_H_A658E8817FB64BE3AC073B58C9229152_INCLUDED
#define F1_SYNTAX_F1PP_CONFIG_H_A658E8817FB64BE3AC073B58C9229152_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_SYNTAX_F1PP_DYNAMIC

# define F1_SYNTAX_F1PP_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1PP: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_SYNTAX_F1PP_STATIC

# define F1_SYNTAX_F1PP_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1PP: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_SYNTAX_F1PP_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1PP: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_SYNTAX_F1PP__CONFIG__DUMP__
# define __F1_SYNTAX_F1PP__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_SYNTAX_F1PP__CONFIG__DUMP__

#endif /* ndef F1_SYNTAX_F1PP_CONFIG_H_A658E8817FB64BE3AC073B58C9229152_INCLUDED */
/*END OF pp.syntax.f1pp-config.h*/
