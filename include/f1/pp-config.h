/*pp-config.h*/
/** @file
 *
 * @brief PP default configuration file.
 *
 */
#pragma once

#ifndef F1_PP_CONFIG_H_31378DC0B0AC40BB882BA9BA1975C58D_INCLUDED
#define F1_PP_CONFIG_H_31378DC0B0AC40BB882BA9BA1975C58D_INCLUDED


// Detect correct interaction
#ifdef F1_PP_DYNAMIC

# define F1_PP_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_STATIC

# define F1_PP_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP__CONFIG__DUMP__
# define __F1_PP__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP__CONFIG__DUMP__

#define __F1_PP__CONFIG__USE_INLINES__		1

#endif /* ndef F1_PP_CONFIG_H_31378DC0B0AC40BB882BA9BA1975C58D_INCLUDED */
/*END OF pp-config.h*/
