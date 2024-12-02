
#macro print_varargs( ... )
	#print "varargs:" __VA_ARGS__
	X
#end
print_varargs( A, 500, 600 )
#print "-----------"

#macro print_varargs_outer
	#macro print_varargs_nested(...)
		#print "varargs:" __VA_ARGS__
		X
	#end
	print_varargs_nested( A, 500, 600 )
#end
print_varargs_outer
print_varargs_nested( B, 500, 600 )	// should not be seen as a macro, output as is
#print "-----------"

#macro print_varargs_global
	#[global]macro print_varargs_nested_global(...)
		#print "varargs:" __VA_ARGS__
		X
	#end
	print_varargs_nested_global( A, 500, 600 )
#end
print_varargs_global
print_varargs_nested_global( B, 500, 600 )
#print "-----------"

#namespace LS
	#macro print_varargs_ns
		#macro print_varargs_nested_ns(...)
			#print "varargs:" __VA_ARGS__
			X
		#end
		print_varargs_nested_ns( A, 500, 600 )
	#end
	print_varargs_ns						// launch macro expansion
#end
print_varargs_nested_ns( B, 500, 600 )		// should not be seen as a macro, output as is
LS.print_varargs_nested_ns( C, 500, 600 )	// should not be seen as well
#print "-----------"

#namespace LS
	#macro print_varargs_public
		#[public]macro print_varargs_nested_public(...)
			#print "varargs:" __VA_ARGS__
			X
		#end
		print_varargs_nested_public( A, 500, 600 )
	#end
	print_varargs_public						// launch macro expansion
#end
print_varargs_nested_public( B, 500, 600 )		// should not be seen as a macro, output as is
LS.print_varargs_nested_public( C, 500, 600 )
#print "-----------"

#macro print_varargs_namespace2( ... )
	#namespace NS
		#macro print_varargs2( ... )
			#print "print_varargs =" __VA_ARGS__
			X2
		#endmacro
	#endnamespace
	NS.print_varargs2( A, 100, 200, 300 )
	#print "print_varargs_namespace = " __VA_ARGS__
	X
#end
print_varargs_namespace2( B, 500, 600 )
NS.print_varargs2( C, 600, 700, 800 )

#macro atomic_add	mem, reg
	lock
	add		mem, reg
	#mem
#endmacro
atomic_add [(eax + ebx) + 12], ecx

#macro AAA(x)
	x
#end
#print "A = \""AAA(A)"\""
#redefine AAA(x)	Z
#print "A = \""AAA(A)"\""

