Test library for f1.pp.c-pp
================================================

This project contains test suite for the f1.pp preprocessor.

Test code sources:
- author's current functionality testing files;
- stackoverflow.com numerous articles on the matter;
- mcpp test suite
- clang preprocessor test suite

Running tests
--------------

Those tests use `contest` testing application that allows to intercept input and output streams, call the application of interest and then compare captured output or generated files with given template files. Equality (probably not exact, one might want to ignore whitespace and difference in new-line types) results in passing test(s).

You'll need to perform following steps to run the test suite:
- for Windows, download `contest.exe` from github page; other platforms are not currently supported.
- run `contest.exe` with command line specifying application and tests to run. There are several predefined options in choosing applications to run the tests over: f1.pp, mcpp and other ones. You'll might need to acquire corresponding tools and specify paths to the executables.
- also, you might want to use predefined MSVS projects that contain full list of tests, so building the project involves evaluation of all tests.
