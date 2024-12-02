@@TODO.md

+ treat #!-directives as comments (i.e. #!/bin/f1pp is treated as a comment)
+ #set += ...
+ #using directive must check that there's no symbol override
+ #line directive doesn't change file name in the output
x #lang "language"	// FreeBASIC
+ #set ++ i
+ multiline macros: global macros (placed to the root namespace), public macros (placed in the
  namespace of enclosing macro)
+ #goto out of loops and inside the loops
+ #set NS.X = 10
+ #goto NS.Label
+ #pragma ident (see SunOS C++)
+ deprecated_macro(), deprecated_include(), ...
x # -- i (INCREMENT/DECREMENT NOT SUPPORTED, PREFIX OPERATIONS ARE RECOGNIZED)
x get rid of repetitious errors on the same line
+ #foreach e: enum
+ #foreach n: namespace
+ #define A 10 + #enum E A=1 #end // error because A=1 turns into 10=1
+ is_item_supported
+ [warn] attribute that displays a message when the macro is used

* take attributes into account
* take limits into account (such as maximum identifier length)

- #mute/#endmute	// disable output (Fypp)
- stringizing operators in the #macro/#function directives
- instead of local char[] buffers and result.add_part_alloc, use stack allocator for buffers
- macro parameter semantics (IDENT, EXPR, etc.)
- store files hashes and dates and process them only they are changed
- #bininclude (include files as is), #stringinclude (include file as a string): #[format(bin|string|hex)] include <file>
- inline form: #{ ... }#include...
- is_a( item, class ) : is_a( n, namespace )

SUPPORT FOR MACRO-LIKE FUNCTIONS
- I/O functions
	- F1PP.LOAD_FILE( file, [offset], [size] )
- string functions
	- F1PP.SUBSTR( str, offset, length )
- additional functions:
	- F1PP.RANDOM( min-bound, max-bound )
	- F1PP.EVAL( expr )

