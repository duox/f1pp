/*pp.frontend-config.h*/
/** @file
 *
 * @brief F1.PP.FRONTEND configuration.
 *
 */

// Detect correct interaction
#ifdef F1_PP_FRONTEND_DYNAMIC

# define F1_PP_FRONTEND_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.FRONTEND: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_FRONTEND_STATIC

# define F1_PP_FRONTEND_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.FRONTEND: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_FRONTEND_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("F1.PP.FRONTEND: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif

/**
 * @brief Availability of dump class member functions.
 */
#ifndef __F1_PP_FRONTEND__CONFIG__DUMP__
# define __F1_PP_FRONTEND__CONFIG__DUMP__			__F1_BASE_CORE__CONFIG__DUMP__
#endif // ndef __F1_PP_FRONTEND__CONFIG__DUMP__

/*END OF pp.frontend-config.h*/
