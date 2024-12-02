/*pragma_clang_deprecated.cxx*/
/** @file
 *
 * @brief Pragma `clang deprecated()' implementation.
 *
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::clang::pragma_clang_deprecated::pragma_clang_deprecated( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::c_pp::clang::pragma_clang_deprecated::get_name() const
{
	return "deprecated";
}
// {3C3143F0-0D9A-4D73-8A24-97AF0CC31CF9}
const f1::guid_t f1::c_pp::clang::pragma_clang_deprecated::m_guid =
{ 0x3c3143f0, 0x0d9a, 0x4d73, { 0x8a, 0x24, 0x97, 0xaf, 0x0c, 0xc3, 0x1c, 0xf9 } };
const f1::guid_t & f1::c_pp::clang::pragma_clang_deprecated::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::clang::pragma_clang_deprecated::get_caps( unsigned mask ) const
{
	return mask & (fl_self_parse_arglist);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::clang::pragma_clang_deprecated::parse( pp::Iparser & parser, const pp::string & param_str ) const
{
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Get macro name
	const char * s = param_str.get_stringslice().data();
	const char * end_s = param_str.get_stringslice().data_end();

	if( '(' != *s )
		parser.message( syntax::IMSG_WARNING_EXPECTED_S, "'('" );
	else
		++ s;
	c_pp_par->skip_real_spaces_and_comments( s, end_s );

	pp::string macro_name;
	c_pp_par->parse_macro_identifier( macro_name, s, end_s );
	if( macro_name.empty() )
		parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );

	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	if( ',' != *s )
		parser.message( syntax::IMSG_WARNING_EXPECTED_S, "','" );
	else
		++ s;

	pp::string text;
	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	c_pp_par->skip_string( s, end_s, &text );
	if( text.empty() )
		parser.message( syntax::IMSG_ERROR_EXPECTED_STRING );

	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	if( ')' != *s )
		parser.message( syntax::IMSG_WARNING_EXPECTED_S, "')'" );
	else
		++ s;

	if( s < end_s )
		return parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );

	// Find macro and push its value
	if( !macro_name.empty() )
	{
		core::Tref<pp::Imacro> mac;
		parser.find_macro( mac, macro_name );
		if( nullptr == mac )
			return parser.message( syntax::IMSG_ERROR_NOT_FOUND_S, macro_name.c_str() );

		mac->modify_flags( pp::Imacro::macro_flags_t::fl_deprecated );
		if( !text.empty() )
			mac->set_deprecation_text( text );
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_clang_deprecated.cxx*/
