// Usage:
// H("foobar") ===> replaced by uint during compilation


// macro hashing function by http://chrissavoie.com/index.php?option=com_content&task=view&id=14&Itemid=1


#define HASH_CONSTANT 5381

// The following is the guts of the compile-time hasher
#define HASH_RECURSE_00(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_01(string+1, *(string+1))), value)
#define HASH_RECURSE_01(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_02(string+1, *(string+1))), value)
#define HASH_RECURSE_02(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_03(string+1, *(string+1))), value)
#define HASH_RECURSE_03(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_04(string+1, *(string+1))), value)
#define HASH_RECURSE_04(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_05(string+1, *(string+1))), value)
#define HASH_RECURSE_05(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_06(string+1, *(string+1))), value)
#define HASH_RECURSE_06(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_07(string+1, *(string+1))), value)
#define HASH_RECURSE_07(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_08(string+1, *(string+1))), value)
#define HASH_RECURSE_08(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_09(string+1, *(string+1))), value)
#define HASH_RECURSE_09(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_10(string+1, *(string+1))), value)
#define HASH_RECURSE_10(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_11(string+1, *(string+1))), value)
#define HASH_RECURSE_11(string, value) HASH_FUNCTION((*(string+1) == 0 ? HASH_CONSTANT : HASH_RECURSE_12(string+1, *(string+1))), value)
#define HASH_RECURSE_12(string, value) HASH_CONSTANT

// The following is the function used for hashing
// Do NOT use NEXTHASH more than once, it will cause
// N-Squared expansion and make compilation very slow
// If not impossible
#define HASH_FUNCTION(NEXTHASH, VALUE) VALUE + (NEXTHASH << 2)

// finally the macro used to generate the hash
#define H(string) (uint)(*string == 0 ? 0 : HASH_RECURSE_00(string, *string))

H("1234567890")
