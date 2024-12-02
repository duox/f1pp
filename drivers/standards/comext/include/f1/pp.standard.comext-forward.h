/*pp.standard.comext-forward.h*/
/** @file
 *
 * @brief Common preprocessor extensions forward declarations.
 *
 */
#pragma once

namespace f1 { namespace c_pp { namespace comext {

	// Base classes
	class plugin;
	class standard;

	// Directives
	class file_directive;
	class ident_directive;
	class include_next_directive;
	class includepath_directive;
	class info_directive;
	class sccs_directive;
	class warning_directive;

	// Macros
	class macro_COUNTER;
	class macro_TIMESTAMP;

	// Pragmas
	class pragma_include_alias;
	class pragma_message;
	class pragma_once;
	class pragma_push_macro;
	class pragma_pop_macro;
}}}

/*END OF pp.standard.comext-forward.h*/
