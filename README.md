Universal preprocessor
==============================================

Copyright 2012-2023 duox (github.com/duox). All rights reserved.

This project, including all nested subprojects, is distributed under the terms of the LICENSE file license.

pp is a development component that performs text preprocessing. Several syntaxes are available, for example C/C++ standard preprocessor, f1 c pp extended syntax and so forth.

Currently the pp project is organized into following folder tree:
- /pp/ - contains generic logics of preprocessor, such as macro and directive management.
- /pp/docs/ - preprocessor core documentation; documentation for specific drivers is located in the drivers's doc/ directory.
- /pp/drivers/ - this folder contains extensions to the pp library, for example, syntax drivers or output drivers.
- /pp/drivers/frontends/ - frontends implementations.
- /pp/drivers/outputs/ - output format drivers (see f1.frontend.f1pp --output-format switch description).
- /pp/drivers/standards/ - preprocessor macros, directives, limits, etc. introduced by corresponding standards.
- /pp/drivers/syntaxes/ - syntax parsers that the standard drivers rely upon. Also, an external utility can be used.
- /pp/include/ - preprocessor core include directory. Clients must include `pp/include/f1/pp.h` or `pp/include/f1/pp-forward.h`.
- /pp/samples/ - basic examples (TODO).
- /pp/scripts/ - source code build scripts.
- /pp/src/ - preprocessor core source code.
- /pp/tests/ - tests related to the preprocessor core. Specific tests are located in drivers' root directories.
- /pp/tools/ - preprocessor tools, e.g. debugger.
