
#define A	3

///////////////////////////////////////////////////////////
// All switch elements

#pragma message( "All switch elements:" )
#switch A
#case 0:	// colon after the case expression
	#pragma message( "A = 0" )
#default
	#pragma message( "Default clause" )
	#fallthrough
#case 1		// no colon after the case expression
	#pragma message( "A = 1" )
#case 2 /**/ :
	#pragma message( "A = 2" )
	#fallthrough
#endswitch

////////////////////////////////////////////////////////////
// Nested switches

#pragma message( "Nested switches:" )
#switch A
#case 0
	#pragma message( "A = 0" )
	#switch A
	#case 0
		#pragma message( "A.0 = 0" )
	#case 1
		#pragma message( "A.0 = 1" )
	#case 2
		#pragma message( "A.0 = 2" )
	#case 3
		#pragma message( "A.0 = 3" )
	#endswitch
#case 1
	#pragma message( "A = 1" )
	#switch A
	#case 0
		#pragma message( "A.1 = 0" )
	#case 1
		#pragma message( "A.1 = 1" )
	#case 2
		#pragma message( "A.1 = 2" )
	#case 3
		#pragma message( "A.1 = 3" )
	#endswitch
#case 2
	#pragma message( "A = 2" )
	#switch A
	#case 0
		#pragma message( "A.2 = 0" )
	#case 1
		#pragma message( "A.2 = 1" )
	#case 2
		#pragma message( "A.2 = 2" )
	#case 3
		#pragma message( "A.2 = 3" )
	#endswitch
#case 3
	#pragma message( "A = 3" )
	#switch A
	#case 0
		#pragma message( "A.3 = 0" )
	#case 1
		#pragma message( "A.3 = 1" )
	#case 2
		#pragma message( "A.3 = 2" )
	#case 3
		#pragma message( "A.3 = 3" )
	#endswitch
#endswitch
