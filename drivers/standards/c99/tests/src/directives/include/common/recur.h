//recur.h
.
#if RECURSIVE
# undef RECURSIVE
# define RECURSIVE	0
# include "recur.h"
#endif
