/*pragma_include_alias.cxx*/
/** @file
 *
 * @brief Pragma push_macro implementation.
 *
 *	Note that it's correct to have non-empty macro stack at the end of the unit processing.
 */
#include "../pch.h"
#include "../main.h"

f1::c_pp::comext::pragma_include_alias::pragma_include_alias( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::c_pp::comext::pragma_include_alias::get_name() const
{
	return "include_alias";
}
// {32C9ACFE-A015-4ac1-8EF9-25C54658ED85}
const f1::guid_t f1::c_pp::comext::pragma_include_alias::m_guid =
{ 0x32c9acfe, 0xa015, 0x4ac1, { 0x8e, 0xf9, 0x25, 0xc5, 0x46, 0x58, 0xed, 0x85 } };
const f1::guid_t & f1::c_pp::comext::pragma_include_alias::get_guid() const
{
	return m_guid;
}

// Operations
const status_t f1::c_pp::comext::pragma_include_alias::parse( pp::Iparser & parser, const pp::string & param_str ) const
{
	status_t status;

	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	const char * s = param_str.get_stringslice().data();
	const char * end_s = param_str.get_stringslice().data_end();
	if( nullptr == s )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "'('" );

	// Skip openning parenthesis
	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	bool parenthesis = false;
	if( '(' == *s )
	{
		parenthesis = true;
		++ s;
	}

	// Parse alias name
	// Alias name is not resolved because it can be non-existent
	c_pp::parser::include_flags_t alias_include_flags = null;
	pp::string alias_name;
	if( c_pp_par->parse_include_filename( alias_name, alias_include_flags, s, end_s ).failed() || alias_name.is_empty() )
		return status;	//parser.message( syntax::IMSG_ERROR_EXPECTED_ALIAS_NAME );

	// Skip comma
	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	if( ',' != *s )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "','" );
	++ s;

	// Parse target name
	c_pp::parser::include_flags_t target_include_flags = null;
	pp::string target_name;
	if( c_pp_par->parse_include_filename( target_name, target_include_flags, s, end_s ).failed() || target_name.is_empty() )
		return status;	//parser.message( syntax::IMSG_ERROR_EXPECTED_TARGET_NAME );

	core::string target_full_name;
	if( c_pp_par->resolve_include_filename( target_full_name, target_name.get_stringslice(), null ).failed_status( status ) )
		return status;

	// Skip closing parenthesis
	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	if( parenthesis )
	{
		if( ')' != *s )
			return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "')'" );
		++ s;
		c_pp_par->skip_real_spaces_and_comments( s, end_s );
	}

	if( s < end_s )
		parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );

	// Check for some errors
	if( alias_include_flags != target_include_flags )
		return parser.message( syntax::IMSG_WARNING_UNMATCHED_QUOTES );

	// Create include alias
	if( c_pp_par->register_alias( nullptr, core::static_string(alias_name.data(), alias_name.length()), core::static_string(target_full_name.data(), target_full_name.length()) ).failed_status( status ) )
		return status;

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_include_alias.cxx*/
