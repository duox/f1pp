/*pp.syntax.f1pp-forward.h*/
/** @file
 *
 * @brief Advanced preprocessor syntax forward declarations.
 *
 */
#pragma once

namespace f1 { namespace f1pp
{

// Directives
interface Iassignment_directive;
class abort_directive;
class alias_directive;
class assert_directive;
class assignment_directive;
class block_directive;
class break_directive;
class case_directive;
class channel_directive;
class comment_directive;
class concat_directive;
class continue_directive;
class debugout_directive;
class default_directive;
class define_directive;
class do_directive;
class dump_directive;
class echo_directive;
class elifdef_directive;
class elifndef_directive;
class embed_directive;
class end_directive;
class endblock_directive;
class endcomment_directive;
class endenum_directive;
class endexec_directive;
class endfor_directive;
class endforeach_directive;
class endfunction_directive;
class endmacro_directive;
class endnamespace_directive;
class endraw_directive;
class endregion_directive;
class endrepeat_directive;
class endshell_directive;
class endswitch_directive;
class endswitchdef_directive;
class endswitchndef_directive;
class endwhile_directive;
class enum_directive;
class env_directive;
class exec_directive;
class exit_directive;
class export_directive;
class fatalerror_directive;
class for_directive;
class foreach_directive;
class format_directive;
class fallthrough_directive;
class function_directive;
class goto_directive;
class ifdef_directive;
class ifndef_directive;
class import_directive;
class include_directive;
class includeonce_directive;
class local_directive;
class macro_directive;
class message_directive;
class namespace_directive;
class option_directive;
class param_directive;
class print_directive;
class raw_directive;
class redefine_directive;
class region_directive;
class repeat_directive;
class return_directive;
class set_directive;
class shell_directive;
class status_directive;
class switch_directive;
class switchdef_directive;
class switchndef_directive;
class tryinclude_directive;
class undef_directive;
class until_directive;
class using_directive;
class version_directive;
class while_directive;

// Macros
class macro_countof;
class macro_deprecated;
class macro_deprecated_directive;
class macro_deprecated_include;
class macro_Embed;
class macro_empty;
class macro_env;
class macro_GLOBAL_LINE;
class macro_has_directive;
class macro_has_include;
class macro_has_option;
class macro_option;

// Pragmas
class pragma_debugout;
class pragma_error;
class pragma_fatalerror;
class pragma_includedir;
class pragma_info;
class pragma_option;
class pragma_status;
class pragma_warning;

// Base definitions
class label;
typedef core::Tobject_array<label>				  label_list_t;
typedef core::Tconst_object_array<label>	const_label_list_t;

class channel;
typedef core::Tobject_array<channel>			  channel_list_t;
typedef core::Tconst_object_array<channel>	const_channel_list_t;

class enumeration;
typedef core::Tobject_array<enumeration>			  enumeration_list_t;
typedef core::Tconst_object_array<enumeration>	const_enumeration_list_t;

class enum_macro;
class env_macro;
class macro;

interface Idefinition_frame;
interface Iscoped_frame;
class function_frame;
class macro_frame;

class parser;
class syntax;
}}

/*END OF pp.syntax.f1pp-forward.h*/
