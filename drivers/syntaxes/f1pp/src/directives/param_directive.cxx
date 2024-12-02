/*param_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::param_directive::param_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::param_directive::get_name() const
{
	return "param";
}

// {19674D23-CFCF-458D-9AF9-A5E438D5AAE1}
const f1::guid_t f1::f1pp::param_directive::m_guid =
{ 0x19674d23, 0xcfcf, 0x458d, { 0x9a, 0xf9, 0xa5, 0xe4, 0x38, 0xd5, 0xaa, 0xe1 } };
const f1::guid_t & f1::f1pp::param_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::param_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

// Operations
const status_t f1::f1pp::param_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = type_cast<f1::parser::Iparser *>(parser.get_parser());
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	f1pp::parser::source_context * ctx = f1par->get_top_source_context();
	if( nullptr == ctx )
		return parser.message( syntax::IMSG_ERROR_ALREADY_USED_S );

	/*core::Tref<ns::name_space> ns_current = f1par->get_ns_context().get_current_namespace();
	core::Tref<ns::name_space> ns;
	if( ns_current->find_subnamespace( ns, "__params" ).failed() )
	{
		if( ns_current->create_subnamespace( ns, "__params" ).failed_status( status ) )
			return status;
	}
	__debugbreak_if( nullptr == ns )
		return STATUS_ERROR_INTERNAL;
	f1par->get_ns_context().set_current_namespace( ns );*/

	// Parse all parameters
	bool was_param = false;
	for( ;; )
	{
		// Skip initial spaces and comments
		f1par->skip_real_spaces_and_comments();
		if( par->is_nl() )
			break;

		// Parse macro name and type
		pp::string ident;
		pp::variant::type_t ty = pp::variant::type_t::ty_none;
		if( f1par->parse_identifier_and_type( ident, ty ).failed_status( status ) )
			break;

		if( ident.empty() )
		{
			status = parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
			break;
		}

		core::Tref<pp::Imacro> existing_macro;
		f1par->find_macro( existing_macro, ident );
		if( nullptr != existing_macro )
		{
			ctx->m_saved_param_macros.append( existing_macro );
			parser.remove_macro( *existing_macro );
		}

		// Parse parameter default value
		pp::string value;
		f1par->skip_real_spaces_and_comments();
		if( par->skip_char( '=' ) )
		{
			f1par->skip_real_spaces_and_comments();

			size_t len = 0;
			const char * arg = par->get_ptr();
			f1par->skip_to_separator( len, ',' );
			if( 0 != len )
				f1par->expand_macros( value, pp::string( arg, len ) );
		}

		// Get parameter value
		const pp::string * arg_val = &value;
		if( nullptr != ctx->m_args && ctx->m_next_arg_index < ctx->m_args->get_arg_list().size() )
		{
			const pp::arg_list::arg_desc_t * arg = ctx->m_args->get_arg_desc( ctx->m_next_arg_index );
			__debugbreak_if( nullptr == arg )
			{
				status = STATUS_ERROR_INTERNAL;
				break;
			}
			++ ctx->m_next_arg_index;

			arg_val = &arg->m_value;
		}

		// Create macro
		core::Tref<pp::Imacro> mac;
		if( parser.create_macro( mac, ident, *arg_val, null ).failed_status( status ) )
		{
			switch( status.get_value() )
			{
			case STATUS_ERROR_ALREADY_EXISTS:
				parser.message( c_pp::syntax::IMSG_ERROR_MACRO_ALREADY_DEFINED_S, ident.c_str() );
				break;
			}
			break;
		}
		__assert( nullptr != mac );
		was_param = true;

		ctx->m_params.append( mac );

		// Iterate
		f1par->skip_real_spaces_and_comments();
		if( !par->skip_char( ',' ) )
			break;
	}

	// Check for errors
	if( status.succeeded() && !was_param )
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_PARAM_LIST );

	//f1par->get_ns_context().set_current_namespace( ns_current );
	//ns_current->use_namespace( *ns );
	if( status.failed() )
		parser.append_macros( ctx->m_saved_param_macros );	// restore state

	// Exit
	return STATUS_SUCCESS;
}

/*END OF param_directive.cxx*/
