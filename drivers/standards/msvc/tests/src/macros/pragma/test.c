//test.c

// The most basic test
__pragma( message( "A message" ) )

// Check sub-pragmas
__pragma( pack push )

// Check that the __pragma gets only one argument
__pragma( pack( push, 1 ) )
