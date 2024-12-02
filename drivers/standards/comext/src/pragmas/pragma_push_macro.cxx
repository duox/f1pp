/*pragma_push_macro.cxx*/
/** @file
 *
 * @brief Pragma push_macro implementation.
 *
 *	Note that it's correct to have non-empty macro stack at the end of the unit processing.
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::comext::pragma_push_macro::pragma_push_macro( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::c_pp::comext::pragma_push_macro::get_name() const
{
	return "push_macro";
}
// {41A3C881-7895-497d-9FEF-5CB240386B3D}
const f1::guid_t f1::c_pp::comext::pragma_push_macro::m_guid =
{ 0x41a3c881, 0x7895, 0x497d, { 0x9f, 0xef, 0x5c, 0xb2, 0x40, 0x38, 0x6b, 0x3d } };
const f1::guid_t & f1::c_pp::comext::pragma_push_macro::get_guid() const
{
	return m_guid;
}

// Operations
const status_t f1::c_pp::comext::pragma_push_macro::parse( pp::Iparser & parser, const pp::string & param_str ) const
{
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Get macro name
	const char * s = param_str.get_stringslice().data();
	const char * end_s = param_str.get_stringslice().data_end();
	if( nullptr == s )
		return parser.message( syntax::IMSG_WARNING_EXPECTED_S, "'('" );

	if( '(' != *s )
		parser.message( syntax::IMSG_WARNING_EXPECTED_S, "'('" );
	else
		++ s;
	c_pp_par->skip_real_spaces_and_comments( s, end_s );

	pp::string macro_name;
	c_pp_par->skip_string( s, end_s, &macro_name );
	c_pp_par->skip_real_spaces_and_comments( s, end_s );

	if( ')' != *s )
		parser.message( syntax::IMSG_WARNING_EXPECTED_S, "')'" );
	else
		++ s;

	if( s < end_s )
		return parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );

	// Find macro and push its value
	core::Tref<pp::Imacro> mac;
	parser.find_macro( mac, macro_name );
	if( nullptr != mac )
		mac->push_value( par->get_current_source(), par->get_line() );
	else
		parser.create_macro( mac, macro_name, pp::string(), pp::Imacro::macro_flags_t::fl_undefined );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_push_macro.cxx*/
