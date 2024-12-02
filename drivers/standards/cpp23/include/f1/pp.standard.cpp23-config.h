/*pp.syntax.cpp23-config.h*/
/** @file
 *
 * @brief PP common extensions default configuration file.
 *
 */

#ifndef F1_PP_STANDARD_CPP23_CONFIG_H_A8A9F274101C46EBB5F755BC3AEE5F22_INCLUDED
#define F1_PP_STANDARD_CPP23_CONFIG_H_A8A9F274101C46EBB5F755BC3AEE5F22_INCLUDED
#pragma once

// Detect correct interaction
#ifdef F1_PP_STANDARD_CPP23_DYNAMIC

# define F1_PP_STANDARD_CPP23_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP23: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STANDARD_CPP23_STATIC

# define F1_PP_STANDARD_CPP23_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP23: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_STANDARD_CPP23_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.STANDARD.CPP23: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_STANDARD_CPP23__CONFIG__DUMP__
# define __F1_PP_STANDARD_CPP23__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_STANDARD_CPP23__CONFIG__DUMP__

#endif /* ndef F1_PP_STANDARD_CPP23_CONFIG_H_A8A9F274101C46EBB5F755BC3AEE5F22_INCLUDED */
/*END OF pp.syntax.cpp23-config.h*/
