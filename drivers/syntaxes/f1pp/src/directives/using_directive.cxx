/*using_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::using_directive::using_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::using_directive::get_name() const
{
	return "using";
}
// {99B6BE34-7809-40D1-B35E-44FCBB56F43D}
const f1::guid_t f1::f1pp::using_directive::m_guid =
{ 0x99b6be34, 0x7809, 0x40d1, { 0xb3, 0x5e, 0x44, 0xfc, 0xbb, 0x56, 0xf4, 0x3d } };
const f1::guid_t & f1::f1pp::using_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::using_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

// Operations
const status_t f1::f1pp::using_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::ns::name_space> current_ns = c_pp_par->get_ns_context().get_current_namespace();
	__debugbreak_if( nullptr == current_ns )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch all arguments
	for(;;)
	{
		// Try to fetch "namespace" string
		bool is_ns = false;
		c_pp_par->skip_real_spaces_and_comments();
		if( par->skip_string( "namespace" ) )
		{
			c_pp_par->skip_real_spaces_and_comments();
			is_ns = true;
		}

		// Fetch identifier
		pp::string	ident;

		c_pp_par->skip_real_spaces_and_comments();
		if( c_pp_par->parse_macro_identifier( ident ).failed_status( status ) )
			return status;
		if( ident.is_empty() )
		{
			parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
			return STATUS_SUCCESS;
		}

		// Use namespace or an identifier
		if( is_ns )
		{
			// Find namespace
			core::Tref<f1::ns::name_space> nsp;
			if( current_ns->find_subnamespace( nsp, ident ).failed_status( status ) )
			{
				parser.message( c_pp::syntax::IMSG_ERROR_NOT_FOUND_S, ident.c_str() );
				return STATUS_SUCCESS;
			}

			// Check that no name from the new namespace is present in the current namespace
			array_foreach( ns::name_list_t::const_iterator, name_it, nsp->get_subname_list() )
			{
				core::Tref<ns::name> nm = *name_it;
				if( current_ns->find_subname( nm, nm->get_string() ).succeeded() )
				{
					parser.message( f1pp::syntax::IMSG_ERROR_NAME_ALREADY_PRESENT_S, ident.c_str() );
					return STATUS_SUCCESS;
				}
			}

			// Use namespace
			current_ns->use_namespace( *nsp );
		}
		else
		{
			// Find name
			core::Tref<f1::ns::name> nmp;
			if( current_ns->find_subname( nmp, ident ).failed_status( status ) )
			{
				parser.message( c_pp::syntax::IMSG_ERROR_NOT_FOUND_S, ident.c_str() );
				return STATUS_SUCCESS;
			}

			// Check that the name is not present in the current namespace
			core::Tref<ns::name> nm = nullptr;
			if( current_ns->find_subname( nm, nmp->get_string() ).succeeded() )
			{
				parser.message( f1pp::syntax::IMSG_ERROR_NAME_ALREADY_PRESENT_S, ident.c_str() );
				return STATUS_SUCCESS;
			}

			// Use name
			current_ns->use_name( *nmp );
		}

		// Iterate
		c_pp_par->skip_real_spaces_and_comments();
		if( par->is_nl() )
			break;
		if( !par->skip_char( ',' ) )
		{
			parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, "," );
			parser.get_parser()->skip_to_nl();
			break;
		}
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF using_directive.cxx*/
