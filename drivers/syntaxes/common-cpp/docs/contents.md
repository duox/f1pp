@@contents.md@@

COMMON PREPROCESSOR FETURES
===========================

This syntax driver contains miscellaneous syntax elements found in many preprocessors.
These features are placed in the single project and syntax drivers may refer to this driver.

Some extensions are specific to only one preprocessor but since number of extensions offered
by this preprocessor is quite low, it would be quite superfluous to implement separate driver
for the preprocessor support.

Each feature listed in documentation contains list of preprocessors that support this feature.


DIRECTIVES
----------

#include_next
#include once
#message
#warning


PREDEFINED MACROS
------------------

__COUNTER__
__pragma (parsed as a predefined macro)

PRAGMAS
-------

+ message (MSVC, GCC)
+ push_macro (MSVC, GCC)
+ pop_macro (MSVC, GCC)

- setlocale (MSVC)
- deprecated (MSVC)
- include_alias (MSVC)

@@END OF contents.md@@
