/*pragma_GCC_error.cxx*/
/** @file
 *
 * @brief Pragma `GCC error()' implementation.
 *
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::gcc::pragma_GCC_error::pragma_GCC_error( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::c_pp::gcc::pragma_GCC_error::get_name() const
{
	return "error";
}
// {29744988-3228-46C5-B1DF-414432A0D270}
const f1::guid_t f1::c_pp::gcc::pragma_GCC_error::m_guid =
{ 0x29744988, 0x3228, 0x46c5, { 0xb1, 0xdf, 0x41, 0x44, 0x32, 0xa0, 0xd2, 0x70 } };
const f1::guid_t & f1::c_pp::gcc::pragma_GCC_error::get_guid() const
{
	return m_guid;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::gcc::pragma_GCC_error::parse( pp::Iparser & parser, const pp::string & param_str ) const
{
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
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
			0 != c_pp_par->get_c_flags( c_pp::parser::c_flags_t::fl_multiple_error_directives ) ?
				syntax::IMSG_ERROR_USER_N:
				syntax::IMSG_FATAL_ERROR_USER_N,
			text.length(), text.data()
			);
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_GCC_error.cxx*/
