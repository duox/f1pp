/*pp-config.h*/
/** @file
 *
 * @brief PP default configuration file.
 *
 */

#ifndef F1_C_PP_CONFIG_H_EF36F44FF727405C83E5C90D3180D16D_INCLUDED
#define F1_C_PP_CONFIG_H_EF36F44FF727405C83E5C90D3180D16D_INCLUDED
#pragma once


// Detect correct interaction
#ifdef F1_C_PP_DYNAMIC

# define F1_C_PP_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("C-PP: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_C_PP_STATIC

# define F1_C_PP_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("C-PP: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_C_PP_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("C-PP: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_C_PP__CONFIG__DUMP__
# define __F1_C_PP__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_C_PP__CONFIG__DUMP__

/**
 * @brief Output macros with subsections (#12.FOREACH.1.DECOUPLE.2).
 */
#ifndef __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__
# define __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__		0
#endif // ndef __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__

#endif /* ndef F1_C_PP_CONFIG_H_EF36F44FF727405C83E5C90D3180D16D_INCLUDED */
/*END OF pp-config.h*/
