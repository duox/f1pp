/*pp.frontend.f1pp.h*/
#pragma once

#include <f1/pp.syntax.cpp.h>
#include <f1/pp.syntax.f1pp.h>

#include <f1/pp.frontend.h>

#include <f1/pp.standard.c99.h>
#include <f1/pp.standard.c11.h>
#include <f1/pp.standard.c23.h>
#include <f1/pp.standard.cpp98.h>
#include <f1/pp.standard.cpp11.h>
#include <f1/pp.standard.cpp14.h>
#include <f1/pp.standard.cpp17.h>
#include <f1/pp.standard.cpp20.h>
#include <f1/pp.standard.cpp23.h>
#include <f1/pp.standard.comext.h>
#include <f1/pp.standard.clang.h>
#include <f1/pp.standard.gcc.h>
#include <f1/pp.standard.msvc.h>

#include <f1/pp.output.gnucpp.h>
#include <f1/pp.output.linenum.h>
#include <f1/pp.output.nullout.h>
#include <f1/pp.output.ppline.h>
#include <f1/pp.output.puretext.h>


#include "pp.frontend.f1pp-config.h"
#include "pp.frontend.f1pp-forward.h"


/*namespace f1 { namespace pp { namespace frontend { namespace f1pp {
	class application;
	class message_sink;
}}}}*/

//#include "pp.f1pp/message_sink.hxx"

#include "pp.frontend.f1pp/switch_types/Iswitch_numeric_arg.hxx"

#include "pp.frontend.f1pp/switches/switch_atsigns_in_identifiers.hxx"
#include "pp.frontend.f1pp/switches/switch_auto_once_includes.hxx"
#include "pp.frontend.f1pp/switches/switch_base_dir_allow.hxx"
#include "pp.frontend.f1pp/switches/switch_base_dir_disallow.hxx"
#include "pp.frontend.f1pp/switches/switch_character_encoding.hxx"
#include "pp.frontend.f1pp/switches/switch_define.hxx"
#include "pp.frontend.f1pp/switches/switch_debug.hxx"
#include "pp.frontend.f1pp/switches/switch_deprecate_include.hxx"
#include "pp.frontend.f1pp/switches/switch_deprecate_system_include.hxx"
#include "pp.frontend.f1pp/switches/switch_digraphs.hxx"
#include "pp.frontend.f1pp/switches/switch_directive.hxx"
#include "pp.frontend.f1pp/switches/switch_disable_output.hxx"
#include "pp.frontend.f1pp/switches/switch_dollars_in_identifiers.hxx"
#include "pp.frontend.f1pp/switches/switch_dump.hxx"
#include "pp.frontend.f1pp/switches/switch_dump_stack_on_error.hxx"
#include "pp.frontend.f1pp/switches/switch_dump_stack_on_warning.hxx"
#include "pp.frontend.f1pp/switches/switch_dump_macros_on_error.hxx"
#include "pp.frontend.f1pp/switches/switch_dump_macros_on_warning.hxx"
#include "pp.frontend.f1pp/switches/switch_endianness.hxx"
#include "pp.frontend.f1pp/switches/switch_export_messages.hxx"
#include "pp.frontend.f1pp/switches/switch_full_paths.hxx"
#include "pp.frontend.f1pp/switches/switch_ignore_macros.hxx"
#include "pp.frontend.f1pp/switches/switch_import_messages.hxx"
#include "pp.frontend.f1pp/switches/switch_include.hxx"
#include "pp.frontend.f1pp/switches/switch_include_dir.hxx"
#include "pp.frontend.f1pp/switches/switch_input_encoding.hxx"
#include "pp.frontend.f1pp/switches/switch_keep_comments.hxx"
#include "pp.frontend.f1pp/switches/switch_keep_doxygen_comments.hxx"
#include "pp.frontend.f1pp/switches/switch_known_pragma.hxx"
#include "pp.frontend.f1pp/switches/switch_long_wide_char.hxx"
#include "pp.frontend.f1pp/switches/switch_macro.hxx"
#include "pp.frontend.f1pp/switches/switch_max_include_depth.hxx"
#include "pp.frontend.f1pp/switches/switch_merge_strings.hxx"
#include "pp.frontend.f1pp/switches/switch_message.hxx"
#include "pp.frontend.f1pp/switches/switch_message_filename_base.hxx"
#include "pp.frontend.f1pp/switches/switch_message_filename_only.hxx"
#include "pp.frontend.f1pp/switches/switch_message_severity.hxx"
#include "pp.frontend.f1pp/switches/switch_newline.hxx"
#include "pp.frontend.f1pp/switches/switch_output_format.hxx"
#include "pp.frontend.f1pp/switches/switch_output_encoding.hxx"
#include "pp.frontend.f1pp/switches/switch_output_filename.hxx"
#include "pp.frontend.f1pp/switches/switch_output_filename_base.hxx"
#include "pp.frontend.f1pp/switches/switch_output_filename_only.hxx"
#include "pp.frontend.f1pp/switches/switch_output_full_paths.hxx"
#include "pp.frontend.f1pp/switches/switch_print_filename.hxx"
#include "pp.frontend.f1pp/switches/switch_standard.hxx"
#include "pp.frontend.f1pp/switches/switch_syntax_check_only.hxx"
#include "pp.frontend.f1pp/switches/switch_system_include_base.hxx"
#include "pp.frontend.f1pp/switches/switch_system_include_dir.hxx"
#include "pp.frontend.f1pp/switches/switch_timings.hxx"
#include "pp.frontend.f1pp/switches/switch_trigraphs.hxx"
#include "pp.frontend.f1pp/switches/switch_undefine.hxx"
#include "pp.frontend.f1pp/switches/switch_unsigned_char.hxx"
#include "pp.frontend.f1pp/switches/switch_user_include_dir.hxx"
#include "pp.frontend.f1pp/switches/switch_write_bom.hxx"

#include "pp.frontend.f1pp/application.hxx"

/*END OF pp.frontend.f1pp.h*/
