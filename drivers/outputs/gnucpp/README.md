GNU cpp output driver
=================================================

Full description can be found here:
https://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html

In short, each line is in the following form:
`# lineno filename [flag ...]`

where `lineno` specifies number of the following line. `filename` part specifies current file name in the form specified by the --full-path flags. According to GNU specification, `filename` must not conatin any non-printing characters; they must be replaced with the corresponding octal escape.

`flag` is an optional component which can repeat as much as needed separated by a space. Following values are supported:
`1` : this is the first line of the `filename`
`2` : this is the first line after another file has been included and finished.
All other flags are currently not supported.
