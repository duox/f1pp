/*pp.syntax.c23-config.h*/
/** @file
 *
 * @brief PP C23 extensions default configuration file.
 *
 */

#ifndef F1_PP_STANDARD_C23_CONFIG_H_83833F1BE375004992E79DF62D3E475C_INCLUDED
#define F1_PP_STANDARD_C23_CONFIG_H_83833F1BE375004992E79DF62D3E475C_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_STANDARD_C23_DYNAMIC

# define F1_PP_STANDARD_C23_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.C23: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STANDARD_C23_STATIC

# define F1_PP_STANDARD_C23_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.C23: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_STANDARD_C23_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.C23: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_STANDARD_C23__CONFIG__DUMP__
# define __F1_PP_STANDARD_C23__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_STANDARD_C23__CONFIG__DUMP__

#endif /* ndef F1_PP_STANDARD_C23_CONFIG_H_83833F1BE375004992E79DF62D3E475C_INCLUDED */
/*END OF pp.syntax.c23-config.h*/
