Macro merge operator test.

Merge operator is `##` operator that takes two parameters, strips leading and trailing spaces of each parameter and then merges two arguments into single entity whch in turn becomes a subject of macro expansion.

Each argument of the `##` operator can be either macro parameter or a space delimited sequence of characters. Merge operator effectively removes spaces between two operands.

C/C++ standards require that the resulting entity must represent a single pp-token (which is equal a single compiler token in this case). However, since the f1pp preprocessor is a universal preprocessor, it knows nothing about C/C++ tokens and thus cannot determine if the result of the `##` operator is valid. To facilitate this, f1pp simply calls a callback that is passed merge result; a client (a C/C++ compiler) must check the string and display a message if result of merg operator represents more than one compiler token.
