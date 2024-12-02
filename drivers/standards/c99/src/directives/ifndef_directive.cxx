/*ifndef_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::c99::ifndef_directive::ifndef_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::c99::ifndef_directive::get_name() const
{
	return "ifndef";
}
// {265341E3-953D-492c-9687-9D5395168860}
const f1::guid_t f1::c_pp::c99::ifndef_directive::m_guid =
{ 0x265341e3, 0x953d, 0x492c, { 0x96, 0x87, 0x9d, 0x53, 0x95, 0x16, 0x88, 0x60 } };
const f1::guid_t & f1::c_pp::c99::ifndef_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::c99::ifndef_directive::get_caps( unsigned mask ) const __noexcept
{
	return cap_invariant & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::c99::ifndef_directive::parse( pp::Iparser & parser )
{
	bool res;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch macro identifier
	c_pp_par->skip_real_spaces_and_comments();

	pp::string ident;
	c_pp_par->parse_macro_identifier( ident );
	if( ident.is_empty() )
	{
		parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		res = false;
	}
	else
	{
		// Find the macro
		core::Tref<pp::Imacro> mac = nullptr;
		parser.find_macro( mac, ident,
			pp::Iparser::find_macro_flags_t::fl_macro_search_test_mode );
		res = nullptr == mac;
		//res = !parser.exists_macro( ident, 0, pp::Imacro::macro_flags_t::fl_expression_only );

		if( nullptr != mac )
			parser.on_macro_expanded( *mac );
	}

	// Begin directive even if there were some errors
	parser.enter_directive( *new(std::nothrow) if_directive::frame( *c_pp_par, res ) );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF ifndef_directive.cxx*/
