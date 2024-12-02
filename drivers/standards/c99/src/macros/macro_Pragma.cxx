/*macro_Pragma.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::c_pp::c99::macro_Pragma::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined | macro_flags_t::fl_unique | macro_flags_t::fl_function ).failed_status( status ) )
		return status;

	// Initialize object
	m_params.append_param( 0, "text" );
	set_param_list( &m_params );

//	core::Tref<ns::name> nm;
//	core::Tref<ns::name_space> ns = &parser.get_ns_context()->get_root_namespace();
//	if( create_name( nm, *ns, 0, "_Pragma" ).failed_status( status ) )
//		return status;

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::c_pp::c99::macro_Pragma::get_name() const
{
	return "_Pragma";
}

const status_t f1::c_pp::c99::macro_Pragma::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	status_t status;
	const char * s;

	// Check current state
	__debugbreak_if( 1 != args->get_arg_list().size() )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "_Pragma(\"string\")" );
	const pp::arg_list::arg_desc_t & arg = *args->get_arg_list().begin();

	core::Tref<f1::c_pp::parser> c_par = type_cast<f1::c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == c_par )
		return STATUS_ERROR_INVALID_TYPE;

	core::Tref<pragma_directive> drct;
	c_par->find_directive( drct, pragma_directive::m_guid );
	__debugbreak_if( nullptr == drct )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Check that argument is a string
	const pp::string & str = arg.m_value;//arg.m_expanded_value.is_empty() ? arg.m_value : arg.m_expanded_value;
	s = str.data();
//	c_par->skip_spaces_and_comments( s, str.data_end() );
//	if( s == str.data_end() || '\"' != *s )
//		return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "_Pragma(\"string\")" );

	// Get macro value
	pp::string val;
	if( c_par->unstringify_chars( val, s, str.data_end() - s, false ).failed_status( status ) )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "_Pragma(\"string\")" );

	s = val.data();

#if 1
	const char * end_s = val.data_end();

	// Parse pragma header
	pp::string ident;
	core::Tref<c_pp::Ipragma> target_prg;
	status = drct->parse_pragma( *c_par, s, end_s, ident, target_prg );
	if( STATUS_INFORMATION_NOT_PROCESSED == status )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
	if( status.failed() )
		return status;

	// Process pragma
	status = drct->execute( parser, ident, pp::string( s, end_s ), target_prg );
#else
	// Extract pragma name
	const char * start = s;
	size_t len = val.length();

	c_par->skip_real_spaces_and_comments( s, s + len );

	pp::string ident;
	if( !c_par->parse_directive_identifier( ident, s, s + len ) )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
	pp::string name;
	name.add_part_alloc( ident );

	val.inc_ptr( s - start );

	// Process pragma
	drct->execute( parser, name, val );
#endif

	// Done
	buf.clear();	// this "macro" doesn't produce anything

	// Exit
	return STATUS_SUCCESS;
}

/*END OF macro_Pragma.cxx*/
