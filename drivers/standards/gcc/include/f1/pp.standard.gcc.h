/*pp.standard.gcc.h*/
/** @file
 *
 * @brief GCC preprocessor standard extensions.
 *
 */
#ifndef F1_STANDARD_GCC_H_38BCA6AF311248F68415CFEFE651220D_INCLUDED
#define F1_STANDARD_GCC_H_38BCA6AF311248F68415CFEFE651220D_INCLUDED
#pragma once


// Include dependencies
#include <f1/pp.h>

#include <f1/pp.syntax.cpp.h>


// Service includes
#include "pp.standard.gcc-config.h"		// library configuration
#include "pp.standard.gcc-forward.h"	// required forward definitions


// Directives

// Macros
#include "pp.standard.gcc/macros/macro_BASE_FILE.hxx"
#include "pp.standard.gcc/macros/macro_FILE_NAME.hxx"
#include "pp.standard.gcc/macros/macro_INCLUDE_LEVEL.hxx"

// Pragmas
#include "pp.standard.gcc/pragmas/pragma_GCC.hxx"
#include "pp.standard.gcc/pragmas/pragma_GCC_error.hxx"
#include "pp.standard.gcc/pragmas/pragma_GCC_warning.hxx"

// Base definitions
//#include "pp.standard.gcc/plugin.hxx"
#include "pp.standard.gcc/standard.hxx"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif /* ndef F1_STANDARD_GCC_H_38BCA6AF311248F68415CFEFE651220D_INCLUDED */
/*END OF pp.standard.gcc.h*/
