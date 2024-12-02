/*alias_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::alias_directive::alias_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::alias_directive::get_name() const
{
	return "alias";
}
// {EF42F8F5-4B2B-4993-84E4-6DFD3C069856}
const f1::guid_t f1::f1pp::alias_directive::m_guid =
{ 0xef42f8f5, 0x4b2b, 0x4993, { 0x84, 0xe4, 0x6d, 0xfd, 0x3c, 0x6, 0x98, 0x56 } };
const f1::guid_t & f1::f1pp::alias_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::alias_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::alias_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	f1::ns::context & ns_ctx = parser.get_ns_context();

	// Get correct namespace
	core::Tref<ns::name_space> previous_ns = ns_ctx.get_current_namespace();
	if( f1pp_par->exists_directive_attribute( "global" ) )
		ns_ctx.set_current_namespace( &ns_ctx.get_root_namespace() );
	else if( f1pp_par->exists_directive_attribute( "public" ) )
		ns_ctx.move_to_parent_namespace();

	// Parse all aliases
	for(;;)
	{
		// Parse alias name
		pp::string name;

		f1pp_par->skip_real_spaces_and_comments();
		if( f1pp_par->parse_macro_identifier( name ).failed_status( status ) )
			break;
		if( name.empty() )
		{
			par->skip_to_nl();
			status = parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
			break;
		}

		// Check that alias name is a single identifier, not a path
		if( nullptr != memchr( name.data(), '.', name.length() ) )
		{
			par->skip_to_nl();
			status = parser.message( syntax::IMSG_ERROR_EXPECTED_SINGLE_IDENTIFIER );
			break;
		}

		// Check whether it's an alias creation
		f1pp_par->skip_real_spaces_and_comments();
		if( !f1pp_par->get_parser()->skip_char( '=' ) )
		{
			par->skip_to_nl();
			status = parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, "=" );
			break;
		}

		// Parse name of alias target
		pp::string target_name;

		f1pp_par->skip_real_spaces_and_comments();
		if( f1pp_par->parse_macro_identifier( target_name ).failed_status( status ) )
		{
			par->skip_to_nl();
			status = parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
			break;
		}
		__debugbreak_if( target_name.empty() )
		{
			status = STATUS_ERROR_INTERNAL;
			break;
		}

		// Find target name
		core::Tref<ns::name>	target_nm;
		if( previous_ns->find_subname( target_nm, target_name.data(), target_name.length() ).failed() )
		{
			par->skip_to_nl();
			status = parser.message( c_pp::syntax::IMSG_ERROR_NOT_FOUND_S, target_name.c_str() );
		}
		__debugbreak_if( nullptr == target_nm )
		{
			status = STATUS_ERROR_INTERNAL;
			break;
		}

		// Get named object
		core::Tref<ns::named_object> target_obj = target_nm->get_object();
		__debugbreak_if( nullptr == target_obj )
		{
			status = STATUS_ERROR_INTERNAL;
			break;
		}

		// Create new name for the alias
		core::Tref<ns::name>	nm;
		if( target_obj->create_name( nm, ns_ctx, 0, name.data(), name.length() ).failed() )
		{
			par->skip_to_nl();
			status = parser.message( c_pp::syntax::IMSG_ERROR_NOT_FOUND_S, target_name.c_str() );
			break;
		}

		// Iterate
		f1pp_par->skip_real_spaces_and_comments();
		if( parser.get_parser()->is_eof() || parser.get_parser()->is_nl() )
			break;
		if( !f1pp_par->get_parser()->skip_char( ',' ) )
		{
			parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, "," );
			par->skip_to_nl();
			break;
		}
	}

	if( previous_ns != ns_ctx.get_current_namespace() )
		ns_ctx.set_current_namespace( previous_ns );

	// Exit
	return status;
}

/*END OF alias_directive.cxx*/
