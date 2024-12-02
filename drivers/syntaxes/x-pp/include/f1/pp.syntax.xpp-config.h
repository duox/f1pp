/*pp.syntax.xpp-config.h*/
/** @file
 *
 * @brief PP default configuration file.
 *
 */

#ifndef F1_XPP_CONFIG_H_145AF04B0EE042949057943EA01284F8_INCLUDED
#define F1_XPP_CONFIG_H_145AF04B0EE042949057943EA01284F8_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_XPP_DYNAMIC

# define F1_XPP_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("XPP: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_XPP_STATIC

# define F1_XPP_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("XPP: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_XPP_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("XPP: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_XPP__CONFIG__DUMP__
# define __F1_XPP__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_XPP__CONFIG__DUMP__

#endif /* ndef F1_XPP_CONFIG_H_145AF04B0EE042949057943EA01284F8_INCLUDED */
/*END OF pp.syntax.xpp-config.h*/
