/*export_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::export_directive::export_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::export_directive::get_name() const
{
	return "export";
}
// {5A72AFDA-27A3-4680-9F9C-64491A2A57C3}
const f1::guid_t f1::f1pp::export_directive::m_guid =
{ 0x5a72afda, 0x27a3, 0x4680, { 0x9f, 0x9c, 0x64, 0x49, 0x1a, 0x2a, 0x57, 0xc3 } };
const f1::guid_t & f1::f1pp::export_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::export_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::export_directive::parse( pp::Iparser & parser )
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

	// Parse all exports
	for(;;)
	{
		// Parse name of export
		pp::string export_name;

		f1pp_par->skip_real_spaces_and_comments();
		if( f1pp_par->parse_macro_identifier( export_name ).failed_status( status ) )
		{
			par->skip_to_nl();
			return parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		}
		__debugbreak_if( export_name.empty() )
			return STATUS_ERROR_INTERNAL;

		// Find export name
		core::Tref<ns::name>	export_nm;
		if( ns_ctx.get_current_namespace()->find_subname( export_nm, export_name.data(), export_name.length() ).failed() )
		{
			par->skip_to_nl();
			return parser.message( c_pp::syntax::IMSG_ERROR_NOT_FOUND_S, export_name.c_str() );
		}
		__debugbreak_if( nullptr == export_nm )
			return STATUS_ERROR_INTERNAL;

		// Get named object
		core::Tref<ns::named_object> export_obj = export_nm->get_object();
		__debugbreak_if( nullptr == export_obj )
			return STATUS_ERROR_INTERNAL;

		// Get object name
		const core::string & name = export_nm->get_string();

		// Create new name for the export
		core::Tref<ns::name>	nm;
		if( ns_ctx.get_root_namespace().create_subname( nm, export_obj, 0, name.c_str(), name.length() ).failed() )
		{
			par->skip_to_nl();
			return parser.message( c_pp::syntax::IMSG_ERROR_NOT_FOUND_S, export_name.c_str() );
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

	// Exit
	return STATUS_SUCCESS;
}

/*END OF export_directive.cxx*/
