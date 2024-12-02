/*pp.server-config.h*/

#define __F1_PP_SERVER__CONFIG__DUMP__	1

// Detect correct interaction
#ifdef F1_PP_SERVER_DYNAMIC

# define F1_PP_SERVER_API		__declspec(dllexport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP_SERVER: exports")
# endif // CONFIG_DUMP_LINK_MODE

#elif defined F1_PP_SERVER_STATIC

# define F1_PP_SERVER_API
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP_SERVER: static")
# endif // CONFIG_DUMP_LINK_MODE

#else

# define F1_PP_SERVER_API		__declspec(dllimport)
# if CONFIG_DUMP_LINK_MODE
#  pragma message("PP_SERVER: imports")
# endif // CONFIG_DUMP_LINK_MODE

#endif


/*END OF pp.server-config.h*/
