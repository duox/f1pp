/*f1.pp.lib.config.h*/

#if _MSC_VER < 1900	// _MSC_VER_2015
# ifndef _WIN32_WINNT
#  define _WIN32_WINNT		0x0403
# elif _WIN32_WINNT < 0x0403
#  undef _WIN32_WINNT
#  define _WIN32_WINNT		0x0403
# endif
#else
# ifndef _WIN32_WINNT
#  define _WIN32_WINNT		0x0601
# elif _WIN32_WINNT < 0x0603
#  undef _WIN32_WINNT
#  define _WIN32_WINNT		0x0603
# endif
#endif

#define F1_BASE_CORE_STATIC
#define F1_BASE_IO_STATIC
#define F1_BASE_FS_STATIC
#define		F1_BASE_FS_WINDOWS_STATIC
#define F1_BASE_MT_STATIC
#define		F1_BASE_MT_WINDOWS_STATIC
#define		F1_BASE_MT_DEFAULT_THREADPOOL_STATIC
#define F1_BASE_NLS_STATIC
#define F1_BASE_NS_STATIC
#define F1_BASE_UTIL_STATIC
#define F1_BASE_MSGOUT_STATIC
#define F1_BASE_UNICODE_STATIC
#define F1_PARSER_STATIC
#define F1_AST_STATIC
#define F1_TS_STATIC
#define F1_PP_STATIC
#define		F1_PP_FRONTEND_STATIC
#define			F1_PP_FRONTEND_F1PP_STATIC
#define		F1_PP_OUTPUT_GNUCPP_STATIC
#define		F1_PP_OUTPUT_LINENUM_STATIC
#define		F1_PP_OUTPUT_NULLOUT_STATIC
#define		F1_PP_OUTPUT_PPLINE_STATIC
#define		F1_PP_OUTPUT_PURETEXT_STATIC
#define F1_DEVEL_MSG_STATIC
#define F1_LIBS_STANDARD_STATIC
#define F1_EXPR_STATIC
#define		F1_EXPR_AST_STATIC
#define		F1_EXPR_PARSER_STATIC
#define		F1_EXPR_TS_STATIC

#define F1_SYNTAX_F1PP_STATIC
#define F1_PP_STANDARD_C99_STATIC
#define F1_PP_STANDARD_C11_STATIC
#define F1_PP_STANDARD_CLANG_STATIC
#define F1_PP_STANDARD_CPP98_STATIC
#define F1_PP_STANDARD_CPP11_STATIC
#define F1_PP_STANDARD_CPP14_STATIC
#define F1_PP_STANDARD_CPP17_STATIC
#define F1_PP_STANDARD_CPP23_STATIC
#define F1_PP_STANDARD_COMEXT_STATIC
#define F1_PP_STANDARD_GCC_STATIC
#define F1_PP_STANDARD_MSVC_STATIC

#define F1_PP_DBG_STATIC
#define F1_PP_DBG_FRONTEND_STATIC
#define F1_PP_DBG_FRONTEND_CONSOLE_STATIC
#define F1_PP_DBG_FRONTEND_GUI_STATIC
#define F1_PP_DBG_FRONTEND_GUI_MFC5_STATIC
#define F1_PP_DBG_FRONTEND_GUI_MFC7_STATIC

#define F1_MSGOUT_CONFIG_DEFAULT_PLUGINS	1

/*END OF f1.pp.lib.config.h*/
