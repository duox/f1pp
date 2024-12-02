/*pragma_pop_macro.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::comext::pragma_pop_macro::pragma_pop_macro( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::c_pp::comext::pragma_pop_macro::get_name() const
{
	return "pop_macro";
}
// {FD366F31-ACB5-47ad-9DEB-9AA526F0422A}
const f1::guid_t f1::c_pp::comext::pragma_pop_macro::m_guid =
{ 0xfd366f31, 0xacb5, 0x47ad, { 0x9d, 0xeb, 0x9a, 0xa5, 0x26, 0xf0, 0x42, 0x2a } };
const f1::guid_t & f1::c_pp::comext::pragma_pop_macro::get_guid() const
{
	return m_guid;
}

// Operations
const status_t f1::c_pp::comext::pragma_pop_macro::parse( pp::Iparser & parser, const pp::string & param_str ) const
{
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
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

	// Find macro and pop its value
	core::Tref<pp::Imacro> mac;
	parser.find_macro( mac, macro_name, pp::Iparser::find_macro_flags_t::fl_macro_search_force );
	if( nullptr != mac )
	{
		if( mac->get_definition_stack().size() == 1 )
			parser.message( syntax::IMSG_WARNING_EMPTY_MACRO_STACK );
		else
		{
			const macro::definition & desc = *mac->get_definition_stack().rbegin();
			if( desc.m_was_undefined )	// was virtually undefined initially?
				c_pp_par->remove_macro( *mac );
			mac->pop_value();
		}
	}
	else
		parser.message( syntax::IMSG_WARNING_EMPTY_MACRO_STACK );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_pop_macro.cxx*/
