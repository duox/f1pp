/*local_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::local_directive::local_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::local_directive::get_name() const
{
	return "local";
}
// {0F00A58F-885C-4712-B63F-0FCADA5999A0}
const f1::guid_t f1::f1pp::local_directive::m_guid =
{ 0xf00a58f, 0x885c, 0x4712, { 0xb6, 0x3f, 0xf, 0xca, 0xda, 0x59, 0x99, 0xa0 } };
const f1::guid_t & f1::f1pp::local_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::local_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

// Operations
const status_t f1::f1pp::local_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Parse all unary assignments in place
	for(;;)
	{
		// Fetch macro identifier
		pp::string ident;
		f1par->skip_real_spaces_and_comments();
		f1par->parse_macro_identifier( ident );
		if( ident.is_empty() )
		{
			parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
			return STATUS_SUCCESS;
		}

		if( !parser.check_limit_value_min( pp::limit_t::li_maximum_identifier_length, ident.length() ) )
			parser.message( syntax::IMSG_WARNING_TOO_LONG_IDENTIFIER );

		// Find the macro
		core::Tref<pp::Imacro> mac;
		parser.find_macro( mac, ident.get_stringslice() );
		if( nullptr != mac && 0 == mac->get_flags( pp::Imacro::macro_flags_t::fl_weak ) )
		{
			parser.message( c_pp::syntax::IMSG_ERROR_MACRO_ALREADY_DEFINED_S, ident.c_str() );
			f1par->display_macro_location( *mac, syntax::IMSG_INFO_DEFINED_HERE );
		}
		else
		{
			// Undefine weak macro
			core::Tref<f1::ns::name> nm;
			parser.get_ns_context().get_current_namespace()->find_subname( nm, ident.data(), ident.length() );
			if( nullptr != nm )
			{
				__assert( nullptr != mac );
				parser.get_ns_context().get_current_namespace()->remove_subname( *nm );
			}

			// Define new macro
			core::string value;
			if( f1par->make_unique_identifier( value ).succeeded() )
				f1par->create_macro( mac, ident, pp::string(value) );
		}

		// Skip separator
		f1par->skip_real_spaces_and_comments();
		if( par->is_nl() )
			break;
		if( !par->skip_char( ',' ) )
			parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_S_AFTER_S, ",", ident.c_str() );
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF local_directive.cxx*/
