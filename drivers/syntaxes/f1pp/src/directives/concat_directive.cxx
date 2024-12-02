/*concat_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::concat_directive::concat_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::concat_directive::get_name() const
{
	return "concat";
}

// {52CEDCA0-E55A-4f36-A2CD-CE9962FF0AC4}
const f1::guid_t f1::f1pp::concat_directive::m_guid =
{ 0x52cedca0, 0xe55a, 0x4f36, { 0xa2, 0xcd, 0xce, 0x99, 0x62, 0xff, 0xa, 0xc4 } };
const f1::guid_t & f1::f1pp::concat_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::concat_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

// Operations
const status_t f1::f1pp::concat_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Parse flags
	f1pp::macro::ex_flags_t ex_flags = null;
	if( f1pp_par->exists_directive_attribute( "global" ) )
		ex_flags |= f1pp::macro::ex_flags_t::fl_global;
	//if( f1pp_par->exists_directive_attribute( "local" ) )
	//	ex_flags |= f1pp::macro::ex_flags_t::fl_local;
	if( f1pp_par->exists_directive_attribute( "public" ) )
		ex_flags |= f1pp::macro::ex_flags_t::fl_public;

	pp::Imacro::macro_flags_t flags = null;
	if( f1pp_par->exists_directive_attribute( "protected" ) )
		flags |= pp::Imacro::macro_flags_t::fl_immutable;
	if( f1pp_par->exists_directive_attribute( "weak" ) )
		flags |= pp::Imacro::macro_flags_t::fl_weak;

	// Parse label name
	pp::string ident;
	f1pp_par->skip_real_spaces_and_comments();
	if( f1pp_par->parse_macro_identifier( ident ).failed_status( status ) )
		return status;
	if( ident.empty() )
	{
		parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		return STATUS_SUCCESS;
	}

	// Find macro
	m_macro = nullptr;
	parser.find_macro( m_macro, ident.get_stringslice() );
	if( nullptr != m_macro && 0 != m_macro->get_flags( pp::Imacro::macro_flags_t::fl_weak ) )
	{
		f1pp_par->remove_macro( *m_macro );
		m_macro = nullptr;
	}
	if( nullptr == m_macro )
		f1pp_par->create_macro( m_macro, ident, pp::string(), flags );

	// Set extended flags
	core::Tref<f1pp::macro> f1mac = m_macro.dynamic_pointer_cast<f1pp::macro>();
	if( nullptr != f1mac )
		f1mac->modify_ex_flags( ex_flags );

	// Skip optional '='
	f1pp_par->skip_real_spaces_and_comments();
	if( f1pp_par->get_parser()->skip_char( '=' ) )
		f1pp_par->skip_real_spaces_and_comments();

	// Fetch expression string and expand macros
	f1pp_par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::concat_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;
	pp::string ident;
	pp::string	result( m_macro->get_value() );

	// Parse buffer
	if( 0 == out_buf.get_data_size() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_VALUE );
		return STATUS_SUCCESS;
	}

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	const bool	was_empty = m_macro->get_value().empty();
	const char *	start = (const char *) out_buf.get_ptr();
	const char * const end_s = (const char *) out_buf.get_end_ptr();
	const char *	s = start;
	const char *	word_start;
	size_t			word_length;
	char			sep;

	// Parse and merge all components
	pp::string	part;
	while( s < end_s )
	{
		sep = 0;
		f1pp_par->skip_real_spaces_and_comments( s, end_s );

		// Check for optional argument marker character
		bool optional = false;
		if( '?' == *s )
		{
			optional = true;
			++ s;
			f1pp_par->skip_real_spaces_and_comments( s, end_s );
		}
		if( s >= end_s )
			return parser.message( f1pp::syntax::IMSG_ERROR_UNEXPECTED_DIRECTIVE_END );

		// Check for screening character
		if( '\\' == *s )
			++ s;
		if( s >= end_s )
			return parser.message( f1pp::syntax::IMSG_ERROR_UNEXPECTED_DIRECTIVE_END );

		// Parse argument
		if( '\"' == *s || '\'' == *s || '`' == *s )
		{
			if( '`' != *s )
				sep = *s;

			f1pp_par->skip_string( s, end_s, &part );
			word_start	= part.data();
			word_length	= part.length();
		}
		//else if( par->peek_char( '(' ) )
		//	f1pp_par->parse_expression( part );
		else if( f1pp_par->is_ident_first_char( *s ) )
		{
			f1pp_par->parse_macro_identifier( part, s, end_s );
			word_start	= part.data();
			word_length	= part.length();
		}
		else
		{
			word_start = s;
			for( ; s < end_s; ++ s )
			{
				if( '\"' == *s || '\'' == *s || f1pp_par->is_ident_first_char( *s ) || isspace( *s ) )
					break;
			}
			word_length = s - word_start;
		}

		// Add result to the string
		if( !optional || !was_empty )
		{
			if( 0 != sep )
				result.add_part_alloc( &sep, 1 );

			result.add_part_alloc( word_start, word_length );

			if( 0 != sep )
				result.add_part_alloc( &sep, 1 );
		}
		part.clear();

		// Iterate
		f1pp_par->skip_real_spaces_and_comments();
	}

	// Set the resulting value
	m_macro->set_value( result );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::concat_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
	parser.modify_parser_flags( pp::Iparser::parser_flags_t::null, pp::Iparser::parser_flags_t::fl_expression_mode );
#ifdef _DEBUG
	core::Tref<f1::f1pp::concat_directive> drc = type_cast<f1::f1pp::concat_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::f1pp::concat_directive *>( param )->execute( parser, buf );
#endif
}

/*END OF concat_directive.cxx*/
