/*macro_Embed.cxx*/
#include "../pch.h"
#include "../main.h"

const status_t f1::f1pp::macro_Embed::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Check current state
	core::Tref<pp::param_list> pars;
	if( nullptr == params )
	{
		pars.createT<pp::param_list>();
		pars->append_param( 0, "name" );
		params = pars;
	}

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined /*| macro_flags_t::fl_operator*/ ).failed_status( status ) )
		return status;

	// Initialize object

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::f1pp::macro_Embed::get_name() const
{
	return "_Embed";
}

const status_t f1::f1pp::macro_Embed::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	status_t status;

	// Check current state
	__debugbreak_if( nullptr == args || 1 != args->get_arg_list().size() )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "_Embed( filename [params ...] )" );
	const pp::arg_list::arg_desc_t & arg = *args->get_arg_list().begin();

	core::Tref<f1::c_pp::parser> cpp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == cpp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Get the #embed directive object
	if( nullptr == m_embed_directive || &parser != m_embed_directive_parser )
	{
		core::Tref<f1pp::embed_directive>	drct;
		if( parser.find_directive( drct, f1pp::embed_directive::m_guid ).failed() )
			return STATUS_ERROR_NOT_INITIALIZED;
		m_embed_directive = drct;
		m_embed_directive_parser = &parser;
	}

	// Execute #embed directive
	if( m_embed_directive->execute( parser, arg.m_expanded_value.is_empty() ? arg.m_value : arg.m_expanded_value ).failed_status( status ) )
		return status;
	buf.clear();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF macro_Embed.cxx*/
