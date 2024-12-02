@@TODO.md@@

+ --macro:__TIMESTAMP__:ignore
+ --output-filename:file
+ $ f1pp --output-format:f1pp.pure text ..\..\src\main.asm
  *** fatal error: cannot create output file: "\src\main.asm.ii".
+ display error message if file not found
+ fix help output discrepancy (-s is used but help printed as /s /switch)
? environment variables
x warn if some option changes behavior of some previous option
+ --write-bom
+ --directive:ident:passthrough,deprecated,ignored
  --directive:ident:ignore,deprecated

- implement switches
- for command line frontend, allow use of -bKx combined switches

- switch --message:0001,0002:fatal|error|warning|suppress
		 --message:0001-0010:...
		 --message:all-fatals|errors|warnings:...
- --comment:start[:end]

- --paththrough-missing-includes
- --paththrough-includes:mask

- --timing:includes|main|all

- --include-binary
- --include-system-binary

- --include:parse-trigraphs:/file.h
- --implicit-fallthrough
- --disable-message-output		// disable output of ordinary message directives such as #message
- --shebang-passthrough
- --replace-nonprint[:radix]	// replace non-printing characters with escape codes
- --backup						// back up existing output files instead of overwriting
- --append						// append data to existing output files instead of overwriting
- --hook:before-file:cmd		// invoke command cmd dst-file src-file; use its output as input; also after
- --deprecate-<kind>:name		// deprecate entity (directive, macro, pragma, file)
- --disable-output				// do not perform output
- --directives-in-args			// allow directives in the macro arguments (c_flags_t::fl_disable_directives_in_args)

- use setjmp somewhere?
