/*pragma_GCC_warning.cxx*/
/** @file
 *
 * @brief Pragma `GCC warning()' implementation.
 *
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::gcc::pragma_GCC_warning::pragma_GCC_warning( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::c_pp::gcc::pragma_GCC_warning::get_name() const
{
	return "warning";
}
// {2B85E71D-4A13-4888-AB19-1F6D8C11AE4C}
const f1::guid_t f1::c_pp::gcc::pragma_GCC_warning::m_guid =
{ 0x2b85e71d, 0x4a13, 0x4888, { 0xab, 0x19, 0x1f, 0x6d, 0x8c, 0x11, 0xae, 0x4c } };
const f1::guid_t & f1::c_pp::gcc::pragma_GCC_warning::get_guid() const
{
	return m_guid;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::gcc::pragma_GCC_warning::parse( pp::Iparser & parser, const pp::string & param_str ) const
{
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Get macro name
	const char * s = param_str.get_stringslice().data();
	const char * end_s = param_str.get_stringslice().data_end();
	bool parenthesis;

	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	if( '(' == *s )
		++ s,
		c_pp_par->skip_real_spaces_and_comments( s, end_s ),
		parenthesis = true;
	else
		parenthesis = false;

	pp::string text;
	c_pp_par->skip_string( s, end_s, &text );
	if( text.empty() )
		parser.message( syntax::IMSG_ERROR_EXPECTED_STRING );

	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	if( parenthesis )
	{
		if( ')' != *s )
			parser.message( syntax::IMSG_WARNING_EXPECTED_S, "')'" );
		else
			++ s;
	}

	if( s < end_s )
		parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );

	// Find macro and push its value
	if( !text.empty() )
	{
		parser.message( 
			syntax::IMSG_WARNING_USER_N,
			text.length(), text.data()
			);
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_GCC_warning.cxx*/
