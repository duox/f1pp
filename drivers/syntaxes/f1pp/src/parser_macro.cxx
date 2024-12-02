/*parser_macro.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::f1pp::parser::create_macro( core::Tref<pp::Imacro> & res,
	const pp::string & ident, const f1::pp::attribute_list * attrs,
	const f1::pp::param_list * params, const pp::string * value, pp::Imacro::macro_flags_t flags
	)
{
	status_t status;

	ns::context & nsc = get_ns_context();

	// Gather attributes
	f1pp::macro::ex_flags_t ex_flags = null;
	if( exists_directive_attribute( "global" ) )
		ex_flags |= f1pp::macro::ex_flags_t::fl_global;
	//if( exists_directive_attribute( "local" ) )
	//	ex_flags |= f1pp::macro::ex_flags_t::fl_local;
	if( exists_directive_attribute( "public" ) )
		ex_flags |= f1pp::macro::ex_flags_t::fl_public;

	// Get correct namespace
	core::Tref<ns::name_space> previous_ns = nsc.get_current_namespace();
	if( ex_flags & f1pp::macro::ex_flags_t::fl_global )
		nsc.set_current_namespace( &nsc.get_root_namespace() );
	else if( ex_flags & f1pp::macro::ex_flags_t::fl_public )
		nsc.move_to_parent_namespace();

	// Create macro object
	status = super::create_macro( res, ident, attrs, params, value, flags );

	if( previous_ns != nsc.get_current_namespace() )
		nsc.set_current_namespace( previous_ns );

	// Exit
	return status;
}

/*END OF parser_function.cxx*/
