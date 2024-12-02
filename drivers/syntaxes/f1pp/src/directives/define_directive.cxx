/*define_directive.cxx*/
/** @file
 * @brief Extension to the C99 standard #define directive.
 */
#include "../pch.h"
#include "../main.h"

f1::f1pp::define_directive::define_directive( unsigned flags ):
	super( flags )
{
}

// Properties

unsigned f1::f1pp::define_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return (super::get_caps() | Idirective::cap_override) & mask;
}

// Hooks
const status_t f1::f1pp::define_directive::fetch_macro_flags(
	c_pp::parser & parser, pp::Imacro::macro_flags_t & flags,
	const pp::string & ident, const pp::param_list * params, const pp::string & value )
{
	flags = null;

	core::Tref<f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_ifnot( nullptr != f1par )
	{
		if( f1par->exists_directive_attribute( "weak" ) )
			flags |= pp::Imacro::macro_flags_t::fl_weak;
	}

	__unreferenced_parameter( ident );
	__unreferenced_parameter( params );
	__unreferenced_parameter( value );
	return STATUS_SUCCESS;
}

const status_t f1::f1pp::define_directive::create_macro( core::Tref<pp::Imacro> & res,
	c_pp::parser & parser,
	const pp::string & ident, const pp::attribute_list * attrs, const pp::param_list * params, const pp::string & value, pp::Imacro::macro_flags_t flags )
{
	status_t status;

	core::Tref<f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_ifnot( nullptr != f1par )
	{
		attrs = f1par->get_directive_attribute_list();
	}
	ns::context & nsc = parser.get_ns_context();

	// Gather attributes
	f1pp::macro::ex_flags_t ex_flags = null;
	if( f1par->exists_directive_attribute( "global" ) )
		ex_flags |= f1pp::macro::ex_flags_t::fl_global;
	//if( f1par->exists_directive_attribute( "local" ) )
	//	ex_flags |= f1pp::macro::ex_flags_t::fl_local;
	if( f1par->exists_directive_attribute( "public" ) )
		ex_flags |= f1pp::macro::ex_flags_t::fl_public;

	if( f1par->exists_directive_attribute( "protected" ) )
		flags |= pp::Imacro::macro_flags_t::fl_immutable;
	if( f1par->exists_directive_attribute( "weak" ) )
		flags |= pp::Imacro::macro_flags_t::fl_weak;

	// Get correct namespace
	core::Tref<ns::name_space> previous_ns = nsc.get_current_namespace();
	if( ex_flags & f1pp::macro::ex_flags_t::fl_global )
		nsc.set_current_namespace( &nsc.get_root_namespace() );
	else if( ex_flags & f1pp::macro::ex_flags_t::fl_public )
		nsc.move_to_parent_namespace();

	// Create macro object
	status = super::create_macro( res, parser, ident, attrs, params, value, flags );

	if( previous_ns != nsc.get_current_namespace() )
		nsc.set_current_namespace( previous_ns );

	// Set extended flags
	core::Tref<f1pp::macro> f1mac = res.dynamic_pointer_cast<f1pp::macro>();
	if( nullptr != f1mac )
		f1mac->modify_ex_flags( ex_flags );

	// Exit
	return status;
}

/*END OF define_directive.cxx*/
