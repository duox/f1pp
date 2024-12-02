/*pragma_includedir.cxx*/
/** @file
 *
 * @brief Pragma includedir implementation.
 *
 * @TODO maybe rename to includepath (like directive)
 *
 */
#include "../pch.h"
#include "../main.h"

f1::f1pp::pragma_includedir::pragma_includedir( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::f1pp::pragma_includedir::get_name() const
{
	return "includedir";
}
// {796761ED-EBBF-4A0E-A979-027530396722}
const f1::guid_t f1::f1pp::pragma_includedir::m_guid =
{ 0x796761ed, 0xebbf, 0x4a0e, { 0xa9, 0x79, 0x02, 0x75, 0x30, 0x39, 0x67, 0x22 } };
const f1::guid_t & f1::f1pp::pragma_includedir::get_guid() const
{
	return m_guid;
}

// Operations
const status_t f1::f1pp::pragma_includedir::parse( pp::Iparser & parser, const pp::string & param_str ) const
{
	status_t status;

	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::pp::context> ctx = parser.get_context();
	__debugbreak_if( nullptr == ctx )
		return STATUS_ERROR_NOT_INITIALIZED;

	const char * s = param_str.get_stringslice().data();
	const char * end_s = param_str.get_stringslice().data_end();

	// Skip open parenthesis
	bool was_parenthesis = false;
	if( '(' == *s )
		was_parenthesis = true,
		++ s;
	else
		parser.message( syntax::IMSG_WARNING_EXPECTED_S, "'('" );
	c_pp_par->skip_real_spaces_and_comments( s, end_s );

	// Skip open quote
	int quote = 0;
	if( '\'' == *s || '\"' == *s )
		quote = *s,
		++ s;
	else if( '<' == *s )
		quote = '>',
		++ s;
	else
		parser.message( syntax::IMSG_WARNING_EXPECTED_S, "\"" );

	// Parse all include paths
	for(;;)
	{
		// Skip leading spaces
		while( s < end_s && isspace( *s ) )
			++ s;
		if( s >= end_s )
			break;

		// Get path separator
		const char * end_pos = _strnchr( s, end_s - s, ';' );
		if( nullcstr == end_pos )
		{
			end_pos = _strnchr( s, end_s - s, char(quote) );
			if( nullcstr == end_pos )
			{
				end_pos = _strnchr( s, end_s - s, ')' );
				if( nullcstr == end_pos )
					end_pos = end_s;
			}
		}

		// Skip trailing spaces
		const char * rpos = end_pos;
		while( s <= rpos && isspace( rpos[-1] ) )
			-- rpos;

		// Append search path
		if( rpos != s )
			parser.append_include_path( '>' == quote ?
				pp::Iparser::include_path_flags_t::fl_system : pp::Iparser::include_path_flags_t::fl_common,
				s, rpos - s );
		else
			parser.message( c_pp::syntax::IMSG_WARNING_EMPTY_PATHNAME );
		if( end_pos >= end_s )
			break;

		// Iterate
		s = end_pos;
		if( ')' == *s )
			break;
		if( quote == *s )
			break;
		s ++;
	}

	// Skip close quote
	if( 0 != quote )
	{
		if( quote != *s )
		{
			const char buf[2] = { char(quote), 0 };
			parser.message( syntax::IMSG_WARNING_EXPECTED_S, buf );
		}
		else
			++ s;
		c_pp_par->skip_real_spaces_and_comments( s, end_s );
	}

	// Skip close parenthesis
	if( ')' != *s )
		parser.message( syntax::IMSG_WARNING_EXPECTED_S, "')'" );
	else
		++ s;

	if( s < end_s )
		return parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_includedir.cxx*/
