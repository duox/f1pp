//test.c

///////////////////////////////////////////////////////////////////////////
// General tests

// Multiple arguments
#format N = "X[%X:%d]" ( 0x40 + 0x20, 16 )
#print "N =" N

// Uncode
// TODO
//#format N = "%C" ( 0x402 )
//#print "N =" N

///////////////////////////////////////////////////////////////////////////
// Format specifiers

// %c - narrow character
// %hC - narrow character
#format N = "\"%c\"" ( 0x4A )
#print "%c(0x4a) =\""N"\""
#format N = "\"%hC\"" ( 0x4A )
#print "%hC(0x4a) =\""N"\""

// %C - wide character
// %wc - wide character
#format N = "\"%C\"" ( 0x4A )
#print "%C(0x4A) =\""N"\""
#format N = "\"%wc\"" ( 0x4A )
#print "%wc(0x4A) =\""N"\""

// %s - narrow string
// %hS - narrow string
#format N = "\"%S\"" ( "abcde" )
#print "%S(\"abcde\") =\""N"\""
// %S - wide string
// %ws - wide string
#format N = "\"%S\"" ( L"abcde" )
#print "%S(L\"abcde\") =\""N"\""

// %d - signed decimal integer
// %i - signed decimal integer
#format N = "\"%d\"" ( 0x4A )
#print "%d( 0x4A) =\""N"\""
#format N = "\"%i\"" ( -0x4A )
#print "%i(-0x4A) =\""N"\""

// %u - unsigned decimal integer
#format N = "\"%u\"" ( 0x4A )
#print "%u( 0x4A) =\""N"\""
#format N = "\"%u\"" ( -0x4A )
#print "%u(-0x4A) =\""N"\""

// %o - octal integer
#format N = "\"%o\"" ( 0x4A )
#print "%o(0x4A) =\""N"\""

// %X - hexadecimal uppercase integer
#format N = "\"%X\"" ( 0x4A )
#print "%X(0x4A) =\""N"\""

// %x - hexadecimal uppercase integer
#format N = "\"%x\"" ( 0x4A )
#print "%x(0x4A) =\""N"\""

///////////////////////////////////////////////////////////////////////////
// Format flags

#format N = "\"%10d\"" (50)
#print " %10d(50) =\""N"\""

#format N = "\"%-10d\"" (50)
#print "%-10d(50) =\""N"\""

#format N = "\"%+10d\"" (50)
#print "%+10d(50) =\""N"\""

#format N = "\"% 10d\"" (50)
#print "% 10d(50) =\""N"\""

#format N = "\"%#10d\"" (50)
#print "%#10d(50) =\""N"\""

