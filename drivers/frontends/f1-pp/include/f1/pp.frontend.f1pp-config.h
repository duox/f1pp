/*pp.frontend.f1pp-config.h*/
/** @file
 *
 * @brief PP F1 front-end default configuration file.
 *
 */

#ifndef F1_PP_FRONTEND_F1PP_CONFIG_H_6BFC32C5C1164AD994538BAEB97E3E1F_INCLUDED
#define F1_PP_FRONTEND_F1PP_CONFIG_H_6BFC32C5C1164AD994538BAEB97E3E1F_INCLUDED


// Detect correct interaction
#ifdef F1_PP_FRONTEND_F1PP_DYNAMIC

# define F1_PP_FRONTEND_F1PP_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.FRONTEND.F1PP: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_FRONTEND_F1PP_STATIC

# define F1_PP_FRONTEND_F1PP_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.FRONTEND.F1PP: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_FRONTEND_F1PP_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP.FRONTEND.F1PP: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_FRONTEND_F1PP__CONFIG__DUMP__
# define __F1_PP_FRONTEND_F1PP__CONFIG__DUMP__		__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_FRONTEND_F1PP__CONFIG__DUMP__

#endif /* ndef F1_PP_FRONTEND_F1PP_CONFIG_H_6BFC32C5C1164AD994538BAEB97E3E1F_INCLUDED */
/*END OF pp.frontend.f1pp-config.h*/
