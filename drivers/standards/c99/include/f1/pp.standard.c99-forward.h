/*pp.standard.c99-forward.h*/
/** @file
 *
 * @brief C99 preprocessor extensions forward declarations.
 *
 */
#pragma once

namespace f1 { namespace c_pp { namespace c99 {

	// Base classes
	//class parser;
	class plugin;
	class standard;

	// Directives
	class	define_directive;
	class	else_directive;
	class	elif_directive;
	class	endif_directive;
	class	error_directive;
	class	file_directive;
	class	if_directive;
	class	ifdef_directive;
	class	ifndef_directive;
	class	include_directive;
	class	line_directive;
	class	pragma_directive;
	class	undef_directive;

	// Macros
	class	macro_DATE;
	class	macro_defined;
	class	macro_FILE;
	class	macro_LINE;
	class	macro_Pragma;
	class	macro_TIME;
	class	macro_VA_ARGS;

	// Pragmas
}}}

/*END OF pp.standard.c99-forward.h*/
