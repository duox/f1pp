/*pp.dbg-config.h*/
/** @file
 *
 * @brief F1.PP.DBG default configuration file.
 *
 */

#ifndef F1_PP_DBG_CONFIG_H_A658E8817FB64BE3AC073B58C9229152_INCLUDED
#define F1_PP_DBG_CONFIG_H_A658E8817FB64BE3AC073B58C9229152_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_DBG_DYNAMIC

# define F1_PP_DBG_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.DBG: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_DBG_STATIC

# define F1_PP_DBG_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.DBG: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_DBG_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.DBG: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_DBG__CONFIG__DUMP__
# define __F1_PP_DBG__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_DBG__CONFIG__DUMP__

#endif /* ndef F1_PP_DBG_CONFIG_H_A658E8817FB64BE3AC073B58C9229152_INCLUDED */
/*END OF pp.dbg-config.h*/
