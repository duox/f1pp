
#macro enclosing
# define internal	1
#end
enclosing			// define the `internal' macro
internal			// not recognized

#macro enclosing_public
# [public] define internal_public	2
#end
enclosing_public	// define the `internal_public' macro
internal_public		// recognized as a macro
