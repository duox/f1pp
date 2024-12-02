@@TODO.md@@

LEGEND:
-		to be implemented
+		done
-+		partially done
x		rejected (add explanation why)
o		rejected until someone asks for it (add explanation why)
?		unclear task, needs reasearch
*		in progress

PARSING:
+ use parser calls, not direct pointers to chars (because of accidential #include's)
+ _Pragma directive
+ parser must not move source text in memory; make a text map
+ #pragma once
+ ternary operator: ?:
+ div and mod: division by zero
+ illegal shift amount: 1 << -1
+ lazy evaluation for && and ||
+ '(' nested expressions
? token pasting operator ## must generate warning if the resulting token is not a pp-token
+ implicit type conversions in expressions (signed/unsigned)
+ suffixes to numbers LlUu
+ allow redefinition of object-like macro if they values are identical
+ allow redefinition of function-like macro if they values are identical and parameter list has
  identical number of parameters with identical spelling
+ store line and source where macro is defined
+ store line and source where compound directive starts
+ #pragma push/pop_macro
+ accept multiple inputs, they are concatenated into the output.
+ warn if #if*/#el*/#endif (and block start/end) are located in different files
+ fix skip_string ("'" is legal string)
+ warn if trying to recursively substitute macro
+ advanced parsing for single_or_tuple_args.c
+ warn if there are any trigraphs/digraphs (in a separate step because of performance issues)
+ allow '$' dollar sign in identifiers; warn however if they are used
+ UCNs are accepted as identifiers
+ maximum identifier length
o extended identifiers allow the characters specified in C11 (SOLUTION: use overloaded is_ident_[first_]char)
o extended identifiers allow the characters specified by Unicode XID_Start and XID_Continue properties (SOLUTION: use overloaded is_ident_[first_]char)
? C++ 2011 Standard user-defined literals
? warn when a string or character literal is followed by a ud-suffix which does not beging with an underscore
+ specify breack characters (when preprocessor calls flush_chars()) - needed for client's support of C++ import statement
+ syntax check only (no output)
+ flag to allow merging of sequential strings.
+ chain syntaxes (so they are not dependent exclusively to c_pp)
+ support several standards (in specific order, e.g. C99+GCC)
+ __FILE__: /FC (full path of source files in disgnostics)
+ check that \11 is parsed as an octal int
+ warn about differences in versions (between C90, C99, C11 and C2X, between C++98, C++11, C++17, C++20.)
+ timings for parsing includes and the main file
- merge m_capturing_line and m_args_parse_mode in the m_parse_mode

TRIGRAPHS AND DIGRAPHS
+ trigraphs
+ digraphs <% ... %> and %:
+ implement digraph and trigraph fetching

ENCODINGS
+ set_input_encoding: encoding is required for #if '0' == 0x30
+ set_output_encoding: encoding required by client tool
? L"..." strings (NO NEED: all charactes are correctly processed by the common string parser)
o bidirectional control characters checking is on. See enum gcc.libcpp.cpp_bidirectional_level
o warn about invalid UTF-8 characters in comments. 2 if it should be a pedwarn
- check encodings work
- UTF-8 tables

STRINGS
? mixed string literal concatenation (take the first one) (NO NEED: that's compiler job)
+ U/u strings and chars, L strings and chars
+ C++ R raw strings
+ C++23 delimited escape sequences \o{...}  \x{...} \u{...} \N{...}
+ \oXXX \o{XXX} escapes
+ mode when chars (wide chars, UTF-8 chars) are unsigned
? C#11 raw strings ("".."..."..."") (NO NEED: we do not support C#)
? C# verbatim strings (@"..." - no escapes and handles new lines), note that in verbatim strings "" = " (NO NEED: we do not support C#)
? C# interpolated strings ($"...") and interpolated raw strings ($...$"""...""") (NO NEED: we do not support C#)
+ warn about invalid forms of delimited or named escape sequences

EVENTS AND CALLBACKS
+ implement failure callbacks such as on_include_not_found, on_macro_not_found, on_unknown_direcitve etc.
? parsing events (macro defined, file included etc.)
? failure via events called from the corresponding virtual function
x on_error (NO NEED: client can add message output to msgout::context and get all errors)
+ check performance (introduce event masks probably)

COMMENTS
+ comments generate a space per comment
+ keep comments in output
x on_comment (NO NEED: context::parse_comment is a virtual function)
+ warn for nested comments
+ nested comments
+ */ outside of comment
+ /##/ => // comment (msvc extension)
+ keep comments (keep comments with deleted slash-nl screening, keep comments from macros)
+ comments parsing code will stop when source expires; when patch expires, comments should end because
  patch is the result of macro expansion and thus ending macro must be ignored only to the end of macro value.
+ detect doxygen comments and: ignore or pass through

WHITESPACE
+ correct line numbers
+ trim all left and right spaces from the macro value
+ compress whitespace in the output
+ output that emits #line directives in line numbering changes
? keep line continuations

MESSAGES:
+ line numbers in error messages
+ when macro error, show where the macro was defined
+ specify where unclosed directive has started
+ stop after N warnings
+ stop after N errors
+ allow base parser to display messages too
+ use error object/callback instead of simple message
+ make text message definitions more comfortable
+ rewrite message system so all messages has same severity which is defined by initial settings
+ fatal errors as just errors (for running tests) (--message-severity:id:severity)
+ message file export (--export-messages)
+ message file import (--import-messages)

EXPRESSIONS
+ some operators (e.g. =) must be specified as known but disallowed (for catching #if a ++ + b which current parser
  will parse as a + + + b without error)
+ #if with invalid expression must still be considered false for parsing #endif
? if an expression passed to #if (or its brethern) contains an unknown macro, expand the expression with known macros
  and pass through unexecuted, and then pass through the remaining block. Each #elif is evaluated in turn and if it
  does not contain unknown macros, it will be executed immediately. Finally, any #else clause is always passed through
  unexecuted. Note that include guards normally defeat this algorithm, so those are specially detected and ignored.
+ unary post-operators (by default no such operators are defined)
+ L and LL suffixes
+ C++23 size_t literals
+ C++ 2014 Standard binary constants
+ C++ 2014 Standard digit separators
+ handle C++ alternate operator names
+ warn about implicit sign changes owing to integer promotions
+ 'true' and 'false' in #if expressions
+ warn about long long numeric constants
+ warn about multi-character constants
+ warn about multicharacter and overflown multicharacter character constants
+ endianess support (for BE it's the most significant byte in a word has the lowest address in memory)
+ warn if undefined identifiers are evaluated in an #if/#elif
+ warn for numbers like 010 that can be confused with decimal numbers
x hexadecimal floats (NO NEED: non-integer constants are not supported)
x C2X decimal floating-point constants (NO NEED: non-integer constants are not supported)
x interpret imaginary, fixed-point, or other gnu extension literal number suffixes as user-defined literal number suffixes (NO NEED: non-integer constants are not supported)
? sizeof operator with support from compiler; if no types defined reject sizeof (COMPILER SHOULD ADD A MACRO)
? precision for chars, ints and wide chars (simply mask) (WHAT FOR?)
? prepare for using = and op= operators in advanced syntaxes. Lazy evaluation of operands in expression -
  value is evaluated only when needed. Such macros use fl_do_not_expand flag to prevent from expanding by expand()
  (WHAT FOR?)

INCLUDES
+ on_include_not_found
+ if an #include is not found, pass it through unmodified. This is very useful for passing through includes of system headers.
+ pass through specified includes without including them
x report includes either as as-is (in the way file name was specified), or relative to the specified path 
  (absolute path if paths are unrelated)
+ return flat list of files that were included during preprocessing (for build support)
+ build dependency graph (ech source must contain a list of sub-sources)
+ #pragma include_alias( alias_name, real_name ) => #include "alias_name" == #include "real_name"
+ warn if maximum depth of the nested #include is reached (set it as a parameter)
+ __has_include, #ifdef __has_include
+ header file name "0x/mylib.h" in #include directive shall not start with a digit
+ warn if user-supplied directory does not exist (specified for all and individual (must have) directories)
x tokenize C++20 module directives (compiler must call include_source() when it meets an import statement)
+ auto #pragma once
+ specify deprecated includes (--deprecate-include:mask and --deprecate-system-include:mask)
+ output new-line delimited list of include files (to source.ins)
+ output list of include files in form of "S|U include-file included-file"
+ #include_next

DIRECTIVES
+ disabling evaluation of particular directives (send them directly to the output)
+ on_unknown_direcitve
+ let #error to continue after printing error and just increments the exit code (so several #error's can be caught)
+ same pragmas in some compilers do not use parenthesis:
	+ pragma message (Digital Mars)
	+ pragma include_alias (IAR)
+ check that #define A 1 // comments are stripped
+ #error "..."
+ #warning
+ #includepath
+ #info
+ pass through unsupported but known directives: #ident, #sccs
+ ignore #! in the first line
+ warn if "defined" is encountered in place other than an #if/#elif (also check check that #ifdef defined is not an error)
+ in #if/#elif, ask for unknown macros
+ process directives only, without trying to expand macros
+ if more than one ## operator and/or # operator appears in the replacement list of a macro definition,
  the order of evaluation of the operators is undefined. So issue a warning for a ## b ## c expressions
? remove #ifdefs keeping all other lines unchanged
? in #if/#elif, if some macros failed to expand, pass entire #if/#endif unchanged
? keep #defines in output file

MACROS
+ warn if macro in expression is undefined
+ do not expand some macros.
+ do not allow to define certain macros
? make pp aware that macro is known but undefined (so evaluate them to 0 without warning)
+ callback on ## merging operator (so client may warn if result of the merge is an invalid pp-token)
+ on_unknown_macro_in_expression, x on_unknown_function_in_expression
+ __VA_ARGS__, __VA_OPT__, args...
+ do not expand macro argument if it's used in # operator
+ warn about __DATE__, __TIME__ and __TIMESTAMP__ usage (reproducing warning)
+ if there is no white space between object-like macro name and its replacement list in a macro definition,
  the C++11 compiler issues a warning message:
  #define A~X	// A ~X
+ variadic macros
  #define debug1(format, ...)		printf(format, ## __VA_ARGS__)
  #define debug2(format, args ...)  printf(format, ## args)
+ warn about builtin macros that are redefined or explicitly undefined
? C++ front-end should add true and false un-undefinable expression-only macros
+ predefined macros must refer to command line (their source/line is empty)
+ fix following code:
#define A	/*
*/"
+ __pragma which does not require a string literal as _Pragma
+ warn of unused macros in the main file
x Nonzero means we are tracking locations of tokens involved in macro expansion. 1 Means we track the location
  in degraded mode where we do not track locations of tokens resulting from the expansion of arguments
  of function-like macro.  2 Means we do track all macro expansions. This last option is the one
  that consumes the highest amount of memory.
+ macro expansion lines:
	#51.FOREACH.1.HASH.2:...
	#51.FOREACH.2:...
- prepare for macro overloading (search for macro considering argument list)

OUTPUT
+ pure text output; without space compression text located at exact lines. Leave #file though
+ --output-format:pplines
+ ignore all input text except preprocessor directives
x BorlandC style output:cfile line:space text (NO NEED: TOO WORDY)

FRONTEND:
+ dump includes
+ accept dollars, at-signs in identifiers
? tab stop size
+ override maximum include depth (for warnings) --max-include-depth
+ dump macros --dump-macros
+ dump directives --dump-directives
+ dump pragmas --dump-pragmas
x dump macros with their values --dump-macro-vals; also dumps usage count
x dump macro references --dump-macro-refs
+ dump known pragmas --dump-known-pragmas (MOVED TO --dump)
+ common dump switch: --dump:item,...[:filename]
- input characters narrow and wide encoding (used for normal and L characters) --exec-encoding and --exec-wide-encoding

EXTENSIONS
+ f1.pp.syntax.common.lib : common extensions such as __COUNTER__, #pragma once etc)
+ remove extensions and place them to f1.pp.syntax.common-cpp project.

VERSIONS
=== C23 ===
+ #warning since C23
=== C++20 ===
+ warn if C++20 __VA_OPT__ feature is not enabled by preprocessing mode
=== C++23 ===
+ GCC support for the '#elifdef' and '#elifndef' directives (since C++23).
=== GCC ===
+ #pragma GCC poison printf
+ #pragma GCC dependency "/usr/include/time.h" rerun fixincludes
- header.gcc files (see -remap in http://jklp.org/info/gnu/gcc/gcc_14.html)
=== TI compiler ===
- #warn

OPTIMIZATION
- carefully optimize pp::string::add_part
- do not expand macros inside #if 0/#endif (already done?)
- specify that the file is already preprocessed in some way: --preprocessed:macros,trigraphs
- use hash maps for searching: macros, unknown macros

ADVANCED
- "enclose arguments in parenthesis" warning
- automatic insertion of parenthesis around arguments
- remove extra parentheses if they are meaningless (only one term)
- disable some identifiers for defining as macros (e.g. true/false etc.)
- intellisence support - file schemes
- generate html report basing on pp information
- line per line execution for plugins/pp-dbg
- precompiled header support (serialization) (warn about precompiled header files we couldn't use)
- dependencies should be restored from a precompiled header
- sourcs common size quotas - use disk cache to temporarily store parsed sources
- show pragmas usage history (maybe for some GUID or pragma name)
- makefile usage

REFERENCES
- Preprocessor written in Python https://github.com/ned14/pcpp (https://pypi.org/project/pcpp/)

LAST CHECK:
+ __fastcall calling conventions (in final build)
- avoid empty functions (such as initialize() that only calls its base counterpart)
- add inline functions
- remove commented code
- add comments on code functionality
- clean up classes by removing:
  - unneeded methods and member variables (e.g. pp_file.find_macro( slice ))
  - commented out or #if 0'ed code
- check value of functions returning pointers for nullptr
- check value of functions returning status_t
- noexcept
- maximize use of const
- check for no leaks in all scenariosa
- pools and TLSF/buddy allocators for memory management
- clean and rebuild solution to find out errors
- internal_error() instead of STATUS_ERROR_INTERNAL
- use f1::pp::cpp::parser::skip_to_newline instead of f1::parser::Iparser::skip_to_nl

TESTING:
- place boost.preprocessor in the test path

REJECTED TODOS:
x multiple sequential strings must be merged:
	#include PATH "/file"	=> #include "basedir" "/file"
  REASON: merging strings is compiler's duty; specifically #include do not support
		  multiple strings; use string merge operator ##.
		  However, some option can be trigger string merge.


@@END OF TODO.md@@
