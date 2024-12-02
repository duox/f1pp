__VA_OPT__ and ##
 ## __VA_OPT__ (...)
 __VA_OPT__ ## (...)
 __VA_OPT__ (## ...)
 __VA_OPT__ (... ##)
 __VA_OPT__ (...) ##
__VA_OPT__ and #
 # __VA_OPT__ (...)
 __VA_OPT__ # (...)
 __VA_OPT__ (# ...)
 __VA_OPT__ (... #)
 __VA_OPT__ (...) #
__VA_OPT__ and spaces
 __VA_OPT__ (...)
 leading and trailing spaces within __VA_OPT__(...)
 __VA_OPT__(...)  (trailing space)
redefinitions
 space between __VA_OPT__ and (
#define foo(...) __VA_OPT__(fred)
#define foo(...) __VA_OPT__ (fred)
 space within __VA_OPT__()
#define foo(...) __VA_OPT__( a)
#define foo(...) __VA_OPT__(a )

#define bar(...) __VA_OPT__ (+a)
#define bar(...) __VA_OPT__ (+ a)
 consecutive __VA_OPT__s
#define foo(...) __VA_OPT__ (a b)
#define foo(...) __VA_OPT__(a) __VA_OPT__(b)
avoid token paste
 __VA_OPT__(foo)bar
 __VA_OPT__(foo)__VA_OPT__(bar)
 

// ERROR: __VA_OPT__ cannot be nested
#define foo1(...) __VA_OPT__(__VA_OPT__)
#define foo2(...) start __VA_OPT__(__VA_OPT__()) end
#define foo3(...) start __VA_OPT__(start __VA_OPT__(foo) end) end

// ERROR: required left argument
#define foo4(...) ## __VA_OPT__(test)
#define foo8(...) __VA_OPT__(##test)
#define foo9(...) test __VA_OPT__(##) test
#define foo10(...) test __VA_OPT__( ## test) test
// ERROR: required right argument
#define foo11(...) __VA_OPT__(test ##)
#define foo12(...) test __VA_OPT__(test## ) test
#define foo13(...) __VA_OPT__(test)##
#define foo14(...) __VA_OPT__(test) ##
#define foo15(...) test __VA_OPT__(test) ##
// OK, generates testtest
#define foo5(...) test /**/ ## /**/ __VA_OPT__ /**/ (test)

// ERROR: __VA_OPT__requires arguments
#define foo6(...) __VA_OPT__ ## (test)
#define foo7(...) __VA_OPT__##(test)
#define foo18(...) __VA_OPT__#()
#define foo19(a, ...) __VA_OPT__ # (a)

// ERROR in msvc (# requires argument) and no warning in GCC/clang
#define foo16(...) # __VA_OPT__()
#define foo17(...) #__VA_OPT__(test)

#define foo23(...) __VA_OPT__(test #)
#define foo24(...) __VA_OPT__(test # ) test
#define foo25(a, ...) __VA_OPT__(test # ) a

// ERROR: # requires an argument
#define foo20(...) __VA_OPT__(#)
#define foo21(a, ...) __VA_OPT__(#) a
// OK
#define foo22(a, ...) __VA_OPT__(#a)
foo22(1)	// OK, __VA_OPT__ returns nothing
foo22(1, 1)	// OK, __VA_OPT__ returns "1"


#define foo26(...) __VA_OPT__ (test)
#define foo27(...) __VA_OPT__ (    test    )
foo27(test)
foo27()
#define foo28(...) __VA_OPT__(test)fred
foo28()
foo28(test)
#define foo29(...) __VA_OPT__(test) fred
foo29()
foo29(test)
#define foo30(...) __VA_OPT__(test)__VA_OPT__(fred)
foo30()
foo30(test)

#define foo31(__VA_OPT__) #__VA_OPT__
foo31(foo)
#define foo32(__VA_OPT__, ...) #__VA_OPT__()
foo32(foo)

#define foo33(...) __VA_OPT__(fred)
#define foo33(...) __VA_OPT__ (fred)

#define foo34(...) __VA_OPT__ (a )
#define foo34(...) __VA_OPT__ ( a)

#define foo35(...) __VA_OPT__ (+a)
#define foo35(...) __VA_OPT__ (+ a)

#define foo36(...) __VA_OPT__ (a b)
#define foo36(...) __VA_OPT__(a) __VA_OPT__(b)

#define foo37(a, ...) start __VA_OPT__(,) end
foo37(foo)
foo37(foo,)
foo37(foo,bar)

#define foo22(...) __VA_OPT__(#__VA_ARGS__)

/* https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83063 */
#define ice(...) b##__VA_OPT__ ()
ice ()
ice (1)

#define f1(...) int b##__VA_OPT__(c)
#define f2(...) int __VA_OPT__(c)##d
#define f3(...) int e##__VA_OPT__()
#define f4(...) int __VA_OPT__()##f
#define f5(...) int g##__VA_OPT__(h)##i
#define f6(...) int j##__VA_OPT__()##k
#define f7(...) int l##__VA_OPT__()
#define f8(...) int __VA_OPT__()##m
#define f9(...) int n##__VA_OPT__()##o
f1 (1, 2, 3);
f1 ();
f2 (1, 2);
f2 ();
f3 (1);
f4 (2);
f5 (6, 7);
f5 ();
f6 (8);
f7 ();
f8 ();
f9 ();

int
main ()
{
  return bc + b + cd + d + f + ghi + gi + jk + l + m + no;
}
