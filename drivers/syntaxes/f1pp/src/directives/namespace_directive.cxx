/*namespace_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::namespace_directive::namespace_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::namespace_directive::get_name() const
{
	return "namespace";
}
// {17A31BEC-EB39-42cd-B72D-FFA4EC10933D}
const f1::guid_t f1::f1pp::namespace_directive::m_guid =
{ 0x17a31bec, 0xeb39, 0x42cd, { 0xb7, 0x2d, 0xff, 0xa4, 0xec, 0x10, 0x93, 0x3d } };
const f1::guid_t & f1::f1pp::namespace_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::namespace_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::namespace_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;
	f1::ns::context & ns_ctx = parser.get_ns_context();

	// Parse namespace name
	f1pp_par->skip_real_spaces_and_comments();

	pp::string name;
	if( f1pp_par->parse_macro_identifier( name ).failed_status( status ) )
		return status;
	if( name.empty() )
	{
		parser.get_parser()->skip_to_nl();
		return parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
	}

	// Check whether it's an alias creation
	f1pp_par->skip_real_spaces_and_comments();
	if( f1pp_par->get_parser()->skip_char( '=' ) )
	{
		// Parse name of alias target
		pp::string target_name;
		f1pp_par->skip_real_spaces_and_comments();
		if( f1pp_par->parse_macro_identifier( target_name ).failed_status( status ) )
			return status;
		__debugbreak_if( target_name.empty() )
			return STATUS_ERROR_INTERNAL;

		// Check that alias name is a single identifier, not a path
		if( nullptr != memchr( name.data(), '.', name.length() ) )
			return parser.message( syntax::IMSG_ERROR_EXPECTED_SINGLE_IDENTIFIER );

		// Find namespace
		core::Tref<ns::name_space>	target_ns;
		if( ns_ctx.get_current_namespace()->find_subnamespace( target_ns, target_name.data(), target_name.length() ).failed() )
			return parser.message( c_pp::syntax::IMSG_ERROR_NOT_FOUND_S, target_name.c_str() );
		__debugbreak_if( nullptr == target_ns )
			return STATUS_ERROR_INTERNAL;

		// Create new name for the namespace
		core::Tref<ns::name>	nm;
		target_ns->create_name( nm, ns_ctx, 0, name.data(), name.length() );

		// Exit
		return STATUS_SUCCESS;
	}

	// Create new namespace
	core::Tref<ns::name_space>	parent_ns = ns_ctx.get_current_namespace();
	core::Tref<ns::name_space>	ns;
	if( parent_ns->create_subnamespace_by_path( ns, name.data(), name.size() ).failed_status( status ) )
		parser.message( f1pp::syntax::IMSG_ERROR_INVALID_IDENTIFIER );

	// Create execution context and enter new nested block
	frame * frm = new(std::nothrow) frame( *f1pp_par );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;
	frm->m_name.move( name );
	frm->m_namespace = ns;
	frm->m_parent_namespace = ns_ctx.get_current_namespace();

	// Enter new namespace
	ns_ctx.set_current_namespace( ns );
	parser.enter_directive( *frm );

	// Exit
	return STATUS_SUCCESS;
}

f1::f1pp::namespace_directive::frame::frame( f1pp::parser & par ):
	super( par )
{
}
const char * f1::f1pp::namespace_directive::frame::get_openning_directive_name() const
{
	return "namespace";
}
const char * f1::f1pp::namespace_directive::frame::get_closing_directive_name() const
{
	return "endnamespace";
}
const f1::guid_t & f1::f1pp::namespace_directive::frame::get_openning_directive_guid() const
{
	return namespace_directive::m_guid;
}
const f1::guid_t & f1::f1pp::namespace_directive::frame::get_closing_directive_guid() const
{
	return endnamespace_directive::m_guid;
}

/*END OF namespace_directive.cxx*/
