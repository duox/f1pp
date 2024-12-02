Comments parsing test.

(1) comments.c

Most of comment testing, the rest of the list is group special cases.

(2) nested_comments.c

Dedicated test file intended to test a nesting comments behaviour.

Due to different approaches in parsing source text, several cases are specified.

(3) merge_comment.c

This is a very special test that shows difference in preprocessor behaviour.

* GCC x86-64 12.2 outputs (correctly):
```
<source>:1:13: error: pasting "/" and "/" does not give a valid preprocessing token
    1 | #define A   /##/
      |             ^
<source>:2:1: note: in expansion of macro 'A'
    2 | A text
      | ^
<source>:4:13: error: pasting "/" and "*" does not give a valid preprocessing token
    4 | #define B   /##*
      |             ^
<source>:5:1: note: in expansion of macro 'B'
    5 | B text
      | ^
<source>:1:13: error: expected unqualified-id before '/' token
    1 | #define A   /##/
      |             ^
<source>:2:1: note: in expansion of macro 'A'
    2 | A text
      | ^
```

* CLang outputs the following (correctly):
```
<source>:2:1: error: pasting formed '//', an invalid preprocessing token
A text
^
<source>:1:14: note: expanded from macro 'A'
#define A   /##/
             ^
<source>:2:1: error: expected unqualified-id
<source>:1:13: note: expanded from macro 'A'
#define A   /##/
            ^
<source>:1:13: note: expanded from macro '/'
<source>:5:1: error: pasting formed '/*', an invalid preprocessing token
B text
^
<source>:4:14: note: expanded from macro 'B'
#define B   /##*
             ^
```

* MSVC 19.0 outputs the following:
```
<source>(6): fatal error C1071: unexpected end of file found in comment
Internal Compiler Error in <path>\cl.exe.  You will be prompted to send an error report to Microsoft later.
INTERNAL COMPILER ERROR in '<path>\cl.exe'
    Please choose the Technical Support command on the Visual C++
    Help menu, or open the Technical Support help file for more information
Compiler returned: 2
```
Did you ever think that you can break a compiler so easily?

* Freehand f1pp completes preprocessing successfully (because it's a general preprocessor and it's the client who
must check merge result for validness) and outputs following:
```
#1 "test.c":
#3:// text
#6:/* text
```
