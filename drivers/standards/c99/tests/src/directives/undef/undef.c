//undef.c

/////////////////////////////////////////////////////////////////
// Valid data zone


// Expected use case
#define OL_0	1
OL_0
#undef OL_0
OL_0


/////////////////////////////////////////////////////////////////
// Potentionally invalid data zone


#undef XM	// undefine non-existent macro (performance warning)


/////////////////////////////////////////////////////////////////
// Invalid data zone


#undef			// no macro name given
#undef X Y		// unexpected characters after macro name
#undef __LINE__	// protected macro