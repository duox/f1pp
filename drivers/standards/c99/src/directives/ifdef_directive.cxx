/*ifdef_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::c99::ifdef_directive::ifdef_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::c99::ifdef_directive::get_name() const
{
	return "ifdef";
}
// {1CF50BAE-310F-4640-99CC-263116B00C1B}
const f1::guid_t f1::c_pp::c99::ifdef_directive::m_guid =
{ 0x1cf50bae, 0x310f, 0x4640, { 0x99, 0xcc, 0x26, 0x31, 0x16, 0xb0, 0x0c, 0x1b } };
const f1::guid_t & f1::c_pp::c99::ifdef_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::c99::ifdef_directive::get_caps( unsigned mask ) const __noexcept
{
	return cap_invariant & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::c99::ifdef_directive::parse( pp::Iparser & parser )
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
		res = nullptr != mac;
		//res = parser.exists_macro( ident, 0, pp::Imacro::macro_flags_t::fl_expression_only );
		///res = parser.exists_macro( ident );

		if( nullptr != mac )
			parser.on_macro_expanded( *mac );
	}

	// Begin directive even if there were some errors
	parser.enter_directive( *new(std::nothrow) if_directive::frame( *c_pp_par, res ) );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF ifdef_directive.cxx*/
