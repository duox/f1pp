/*pp.syntax.f1pp.h*/
/** @file
 *
 * @brief Advanced syntax for universal preprocessor.
 *
 */
#ifndef F1_SYNTAX_F1PP_H_884821E7F4E54A65880A050A889C01AE_INCLUDED
#define F1_SYNTAX_F1PP_H_884821E7F4E54A65880A050A889C01AE_INCLUDED
#pragma once


// Include dependencies
#include <f1/pp.h>
#include <f1/pp.standard.c99.h>
#include <f1/pp.standard.c23.h>


// Service includes
#include "pp.syntax.f1pp-config.h"		// library configuration
#include "pp.syntax.f1pp-forward.h"		// required forward definitions


#include "pp.syntax.f1pp/frames/Idefinition_frame.hxx"
#include "pp.syntax.f1pp/frames/Iscoped_frame.hxx"
#include "pp.syntax.f1pp/frames/function_frame.hxx"
#include "pp.syntax.f1pp/frames/macro_frame.hxx"

#include "pp.syntax.f1pp/parser.hxx"

// Directives
#include "pp.syntax.f1pp/directives/Iassignment.hxx"
#include "pp.syntax.f1pp/directives/abort.hxx"
#include "pp.syntax.f1pp/directives/alias.hxx"
#include "pp.syntax.f1pp/directives/assert.hxx"
#include "pp.syntax.f1pp/directives/assignment.hxx"
#include "pp.syntax.f1pp/directives/block.hxx"
#include "pp.syntax.f1pp/directives/break.hxx"
#include "pp.syntax.f1pp/directives/case.hxx"
#include "pp.syntax.f1pp/directives/channel.hxx"
#include "pp.syntax.f1pp/directives/comment.hxx"
#include "pp.syntax.f1pp/directives/concat.hxx"
#include "pp.syntax.f1pp/directives/continue.hxx"
#include "pp.syntax.f1pp/directives/default.hxx"
#include "pp.syntax.f1pp/directives/define.hxx"
#include "pp.syntax.f1pp/directives/do.hxx"
#include "pp.syntax.f1pp/directives/dump.hxx"
#include "pp.syntax.f1pp/directives/echo.hxx"
#include "pp.syntax.f1pp/directives/elifdef.hxx"
#include "pp.syntax.f1pp/directives/elifndef.hxx"
#include "pp.syntax.f1pp/directives/end.hxx"
#include "pp.syntax.f1pp/directives/endblock.hxx"
#include "pp.syntax.f1pp/directives/endcomment.hxx"
#include "pp.syntax.f1pp/directives/endenum.hxx"
#include "pp.syntax.f1pp/directives/endexec.hxx"
#include "pp.syntax.f1pp/directives/endfor.hxx"
#include "pp.syntax.f1pp/directives/endforeach.hxx"
#include "pp.syntax.f1pp/directives/endfunction.hxx"
#include "pp.syntax.f1pp/directives/endmacro.hxx"
#include "pp.syntax.f1pp/directives/endnamespace.hxx"
#include "pp.syntax.f1pp/directives/endregion.hxx"
#include "pp.syntax.f1pp/directives/endraw.hxx"
#include "pp.syntax.f1pp/directives/endrepeat.hxx"
#include "pp.syntax.f1pp/directives/endshell.hxx"
#include "pp.syntax.f1pp/directives/endswitch.hxx"
#include "pp.syntax.f1pp/directives/endswitchdef.hxx"
#include "pp.syntax.f1pp/directives/endswitchndef.hxx"
#include "pp.syntax.f1pp/directives/endwhile.hxx"
#include "pp.syntax.f1pp/directives/enum.hxx"
#include "pp.syntax.f1pp/directives/env.hxx"
#include "pp.syntax.f1pp/directives/exec.hxx"
#include "pp.syntax.f1pp/directives/exit.hxx"
#include "pp.syntax.f1pp/directives/export.hxx"
#include "pp.syntax.f1pp/directives/fallthrough.hxx"
#include "pp.syntax.f1pp/directives/for.hxx"
#include "pp.syntax.f1pp/directives/foreach.hxx"
#include "pp.syntax.f1pp/directives/format.hxx"
#include "pp.syntax.f1pp/directives/function.hxx"
#include "pp.syntax.f1pp/directives/goto.hxx"
#include "pp.syntax.f1pp/directives/ifdef.hxx"
#include "pp.syntax.f1pp/directives/ifndef.hxx"
#include "pp.syntax.f1pp/directives/includes/embed.hxx"
#include "pp.syntax.f1pp/directives/includes/import.hxx"
#include "pp.syntax.f1pp/directives/includes/include.hxx"
#include "pp.syntax.f1pp/directives/includes/includeonce.hxx"
#include "pp.syntax.f1pp/directives/includes/param.hxx"
#include "pp.syntax.f1pp/directives/includes/tryinclude.hxx"
#include "pp.syntax.f1pp/directives/local.hxx"
#include "pp.syntax.f1pp/directives/macro.hxx"
#include "pp.syntax.f1pp/directives/messages/debugout.hxx"
#include "pp.syntax.f1pp/directives/messages/message.hxx"
#include "pp.syntax.f1pp/directives/messages/fatalerror.hxx"
#include "pp.syntax.f1pp/directives/messages/print.hxx"
#include "pp.syntax.f1pp/directives/messages/status.hxx"
#include "pp.syntax.f1pp/directives/namespace.hxx"
#include "pp.syntax.f1pp/directives/option.hxx"
#include "pp.syntax.f1pp/directives/raw.hxx"
#include "pp.syntax.f1pp/directives/redefine.hxx"
#include "pp.syntax.f1pp/directives/region.hxx"
#include "pp.syntax.f1pp/directives/repeat.hxx"
#include "pp.syntax.f1pp/directives/return.hxx"
#include "pp.syntax.f1pp/directives/shell.hxx"
#include "pp.syntax.f1pp/directives/set.hxx"
#include "pp.syntax.f1pp/directives/switch.hxx"
#include "pp.syntax.f1pp/directives/switchdef.hxx"
#include "pp.syntax.f1pp/directives/switchndef.hxx"
#include "pp.syntax.f1pp/directives/undef.hxx"
#include "pp.syntax.f1pp/directives/until.hxx"
#include "pp.syntax.f1pp/directives/using.hxx"
#include "pp.syntax.f1pp/directives/version.hxx"
#include "pp.syntax.f1pp/directives/while.hxx"

// Macros
#include "pp.syntax.f1pp/macros/macro_countof.hxx"
#include "pp.syntax.f1pp/macros/macro_deprecated.hxx"
#include "pp.syntax.f1pp/macros/macro_deprecated_directive.hxx"
#include "pp.syntax.f1pp/macros/macro_deprecated_include.hxx"
#include "pp.syntax.f1pp/macros/macro_Embed.hxx"
#include "pp.syntax.f1pp/macros/macro_empty.hxx"
#include "pp.syntax.f1pp/macros/macro_env.hxx"
#include "pp.syntax.f1pp/macros/macro_GLOBAL_LINE.hxx"
#include "pp.syntax.f1pp/macros/macro_has_directive.hxx"
#include "pp.syntax.f1pp/macros/macro_has_include.hxx"
#include "pp.syntax.f1pp/macros/macro_has_option.hxx"
#include "pp.syntax.f1pp/macros/macro_option.hxx"

// Pragmas
#include "pp.syntax.f1pp/pragmas/pragma_debugout.hxx"
#include "pp.syntax.f1pp/pragmas/pragma_error.hxx"
#include "pp.syntax.f1pp/pragmas/pragma_fatalerror.hxx"
#include "pp.syntax.f1pp/pragmas/pragma_includedir.hxx"
#include "pp.syntax.f1pp/pragmas/pragma_info.hxx"
#include "pp.syntax.f1pp/pragmas/pragma_option.hxx"
#include "pp.syntax.f1pp/pragmas/pragma_status.hxx"
#include "pp.syntax.f1pp/pragmas/pragma_warning.hxx"

// Base definitions
#include "pp.syntax.f1pp/channel.hxx"
#include "pp.syntax.f1pp/enumeration.hxx"
#include "pp.syntax.f1pp/macro.hxx"
#include	"pp.syntax.f1pp/enum_macro.hxx"
#include	"pp.syntax.f1pp/env_macro.hxx"
#include "pp.syntax.f1pp/label.hxx"

#include "pp.syntax.f1pp/syntax.hxx"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif /* ndef F1_SYNTAX_F1PP_H_884821E7F4E54A65880A050A889C01AE_INCLUDED */
/*END OF pp.syntax.f1pp.h*/
