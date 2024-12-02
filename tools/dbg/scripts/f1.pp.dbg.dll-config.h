/*f1.pp.dbg.dll-config.h*/

#define CONFIG_DUMP_LINK_MODE	1

#define F1_BASE_CORE_DYNAMIC
#define F1_BASE_IO_DYNAMIC
#define F1_BASE_FS_DYNAMIC
#define F1_BASE_NS_DYNAMIC
#define F1_BASE_UTIL_DYNAMIC
#define F1_BASE_UNICODE_DYNAMIC
#define F1_BASE_MSGOUT_DYNAMIC
#define F1_TS_DYNAMIC
#define F1_PP_DYNAMIC

/*#if defined F1_BASE_CORE_DLL || defined F1_BASE_CORE_LIB
# define F1_BASE_CORE_DYNAMIC
#else
# undef F1_BASE_CORE_DYNAMIC
# undef F1_BASE_CORE_STATIC
#endif

#if defined F1_BASE_IO_DLL || defined F1_BASE_IO_LIB
# define F1_BASE_IO_DYNAMIC
#else
# undef F1_BASE_IO_DYNAMIC
# undef F1_BASE_IO_STATIC
#endif

#if defined F1_BASE_FS_DLL || defined F1_BASE_FS_LIB
# define F1_BASE_FS_DYNAMIC
#else
# undef F1_BASE_FS_DYNAMIC
# undef F1_BASE_FS_STATIC
#endif

#if defined F1_BASE_NS_DLL || defined F1_BASE_NS_LIB
# define F1_BASE_NS_DYNAMIC
#else
# undef F1_BASE_NS_DYNAMIC
# undef F1_BASE_NS_STATIC
#endif

#if defined F1_BASE_UTIL_DLL || defined F1_BASE_UTIL_LIB
# define F1_BASE_UTIL_DYNAMIC
#else
# undef F1_BASE_UTIL_DYNAMIC
# undef F1_BASE_UTIL_STATIC
#endif

// f1.ade.dll
#if defined F1_ADE_DLL || defined F1_ADE_LIB || defined F1_ADE_EXE
# define F1_ADE_DYNAMIC
#else
# undef F1_ADE_DYNAMIC
# undef F1_ADE_STATIC
#endif

/*END OF f1.pp.dbg.dll-config.h*/
