/*parse_identifier.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::f1pp::parser::parse_identifier()
{
	if( m_in_function && !m_capturing_line && /*m_new_line && */!(m_c_flags & c_flags_t::fl_disable_directives) )
		return on_esc_char();
	
	if( !m_ignore_enum_ident &&
		m_f1pp_flags & f1pp_flags_t::fl_enum_mode &&
		m_enum_parenthesis_stack.empty() &&
		is_exec_allowed()
		)
	{
		if( m_was_enum_ident )
			;//message( syntax::IMSG_ERROR_UNEXPECTED_IDENTIFIER );
		else
		{
			m_was_enum_ident = true;

			pp::string ident;
			parse_definition_identifier( ident );
			if( nullptr == m_active_enum_callback )
				output( ident );
			else
				m_active_enum_callback->set_identifier( *this, ident );
	
			return STATUS_SUCCESS;
		}
	}

	return super::parse_identifier();
}
bool f1::f1pp::parser::parse_directive_identifier( pp::string & buf, const char *& start, const char * end_s, f1::parser::Iparser * parser )
{
	return super::parse_directive_identifier( buf, start, end_s, parser );
}
bool f1::f1pp::parser::parse_definition_identifier( pp::string & buf, const char *& start, const char * end_s, f1::parser::Iparser * parser )
{
	return super::parse_definition_identifier( buf, start, end_s, parser );
}
bool f1::f1pp::parser::parse_macro_identifier( pp::string & buf, const char *& start, const char * end_s, f1::parser::Iparser * parser )
{
	status_t			status;
	core::Tref<ns::name_space>	ns = get_ns_context().get_current_namespace();
	const char * s = start;

	if( nullptr == parser )
		parser = get_parser();

	buf.clear();
	for(;;)
	{
		// Parse identifier
		pp::string ident;
		super::parse_definition_identifier( ident, s, end_s, parser );
		if( ident.empty() )
			break;

		// OK, name found, let's add it
		if( !buf.empty() )
			buf.add_part( "." );
		buf.add_part( ident );
		start = s;

		// Find it in the current namespace
		core::Tref<ns::name> nm;
		if( ns->find_subname( nm, ident.data(), ident.length() ).failed() )
			break;

		// Check whether it's a namespace
		ns = type_cast<ns::name_space *>( nm->get_object() );
		if( nullptr == ns )
			break;

		// Try to skip to dot
		skip_real_spaces_and_comments( s, end_s );
		if( s >= end_s || '.' != *s )
			break;
		++ s;
		skip_real_spaces_and_comments( s, end_s );
	}

	return !buf.empty();
}

const status_t f1::f1pp::parser::parse_identifier_list( core::Tarray<pp::string> & res )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source

	status_t status = parse_identifier_list( res, s, end_s );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}
const status_t f1::f1pp::parser::parse_identifier_list( core::Tarray<pp::string> & res, const char *& s, const char * end_s )
{
	status_t status;

	// Parse all macro names
	for( ; s < end_s; )
	{
		// Parse macro name
		pp::string ident;
		skip_real_spaces_and_comments( s, end_s );
		parse_macro_identifier( ident, s, end_s );
		if( ident.empty() )
		{
			m_parser->skip_to_nl();
			return message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		}

		// Process macro
		res.append( ident );

		// Iterate
		skip_real_spaces_and_comments( s, end_s );
		if( f1::parser::Iparser::is_nl( s, end_s ) )
			break;
		if( ',' != *s )
			break;
		++ s;
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::parse_identifier_and_string_list( core::Tarray<pp::string> & res )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source

	status_t status = parse_identifier_and_string_list( res, s, end_s );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}
const status_t f1::f1pp::parser::parse_identifier_and_string_list( core::Tarray<pp::string> & res, const char *& s, const char * end_s )
{
	status_t status;

	// Parse all macro names
	for(;;)
	{
		// Parse macro name
		pp::string ident;
		skip_real_spaces_and_comments( s, end_s );
		parse_macro_identifier( ident, s, end_s );
		if( ident.empty() )
		{
			parse_string( ident, s, end_s );
			if( ident.empty() )
			{
				m_parser->skip_to_nl();
				return message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
			}
		}

		// Process macro
		res.append( ident );

		// Iterate
		skip_real_spaces_and_comments( s, end_s );
		if( f1::parser::Iparser::is_nl( s, end_s ) )
			break;
		if( ',' != *s )
		{
			m_parser->skip_to_nl();
			return message( syntax::IMSG_ERROR_EXPECTED_S, "," );
		}
		++ s;
	}

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::f1pp::parser::parse_identifier_and_type( pp::string & ident, pp::variant::type_t & type_id )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source

	status_t status = parse_identifier_and_type( ident, type_id, s, end_s );
//	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}
const status_t f1::f1pp::parser::parse_identifier_and_type( core::Tref<pp::Imacro> & res, bool * append )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source

	status_t status = parse_identifier_and_type( res, s, end_s, append );
//	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}
const status_t f1::f1pp::parser::parse_identifier_and_type( pp::string & ident, pp::variant::type_t & type_id, const char * & s, const char * end_s )
{
	skip_real_spaces_and_comments( s, end_s );
	const char * old_s = s;
	type_id = pp::variant::ty_none;

	// Fetch macro identifier
	parse_macro_identifier( ident, s, end_s );
	if( !ident.is_empty() )
	{
		// Parse type specification
		skip_real_spaces_and_comments( s, end_s );
		if( ':' == *s )
		{
			++ s;
			skip_real_spaces_and_comments( s, end_s );
			pp::string type_name;
			parse_type( type_id, &type_name, s, end_s );
			if( f1::pp::variant::ty_none == type_id )
				message( f1pp::syntax::IMSG_ERROR_UNKNOWN_TYPE_NAME_S, type_name.c_str() );
		}
	}
	else
		s = old_s;

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::parse_identifier_and_type( core::Tref<pp::Imacro> & res, const char * & s, const char * end_s, bool * append )
{
	pp::string ident;
	pp::variant::type_t	type_id = f1::pp::variant::ty_none;
	bool add_value = false, no_auto = false;

	skip_real_spaces_and_comments( s, end_s );
	const char * old_s = s;

	// Fetch macro identifier
	parse_macro_identifier( ident, s, end_s );
	if( !ident.is_empty() )
	{
		// Find the macro
		core::Tref<pp::Imacro> mac;
		find_macro( mac, ident.get_stringslice() );
		if( nullptr != mac )
		{
			// Check macro identifier length
			if( !check_limit_value_min( pp::limit_t::li_maximum_identifier_length, ident.length() ) )
				message( syntax::IMSG_WARNING_TOO_LONG_IDENTIFIER );
		}

		// Parse type specification
		skip_real_spaces_and_comments( s, end_s );
		if( ':' == *s )
		{
			++ s;
			skip_real_spaces_and_comments( s, end_s );
			pp::string type_name;
			parse_type( type_id, &type_name, s, end_s );
			if( f1::pp::variant::ty_none == type_id )
			{
				message( f1pp::syntax::IMSG_ERROR_UNKNOWN_TYPE_NAME_S, type_name.c_str() );
				return STATUS_SUCCESS;
			}
		}

		// Parse assignment operator
		skip_real_spaces_and_comments( s, end_s );
		const char * prep_s = s;
		if( '+' == *s )
		{
			++ s,
			add_value = true;
			if( '?' == *s )
				++ s,
				no_auto = true;
		}
		else if( '?' == *s )
		{
			++ s,
			no_auto = true;
			if( '+' == *s )
				++ s,
				add_value = true;
		}
		if( '=' != *s )
			s = prep_s;

		// Parse assignment
		skip_real_spaces_and_comments( s, end_s );
		if( '=' != *s )
			s = old_s;
		else
		{
			++ s;

			// Create macro only if assignment statement is not `?='
			if( nullptr == mac && !no_auto )
			{
				// Gather macro attributes
				pp::Imacro::macro_flags_t macro_flags = null;
				if( exists_directive_attribute( "deprecated" ) )
					macro_flags |= pp::Imacro::macro_flags_t::fl_deprecated;
				if( exists_directive_attribute( "disabled" ) )
					macro_flags |= pp::Imacro::macro_flags_t::fl_disabled;
				if( exists_directive_attribute( "protected" ) )
					macro_flags |= pp::Imacro::macro_flags_t::fl_immutable;
				if( exists_directive_attribute( "weak" ) )
					macro_flags |= pp::Imacro::macro_flags_t::fl_weak;

				unsigned def_flags = 0;
				if( exists_directive_attribute( "global" ) )
					def_flags |= Idefinition_frame::fl_def_global;
				if( exists_directive_attribute( "local" ) )
					def_flags |= Idefinition_frame::fl_def_local;
				if( exists_directive_attribute( "public" ) )
					def_flags |= Idefinition_frame::fl_def_public;

				// Get correct namespace
				core::Tref<ns::name_space> previous_ns = get_ns_context().get_current_namespace();
				if( def_flags & macro_directive::frame::fl_def_global )
					get_ns_context().set_current_namespace( &get_ns_context().get_root_namespace() );
				else if( def_flags & macro_directive::frame::fl_def_public )
					get_ns_context().move_to_parent_namespace();

				// Create macro
				create_macro( mac, ident, nullptr, nullptr, nullptr, macro_flags );
//				if( pp::variant::ty_none != type_id )
					mac->set_value_variant( pp::variant( type_id ) );
//				else
//					mac->set_value_variant( pp::variant( intmax_t(0) ) );

				if( previous_ns != get_ns_context().get_current_namespace() )
					get_ns_context().set_current_namespace( previous_ns );
			}
			else if( nullptr != mac )
			{
				if( mac->get_flags( pp::Imacro::macro_flags_t::fl_immutable ) )
				{
					message( syntax::IMSG_ERROR_MACRO_IS_IMMUTABLE );
					mac = nullptr;
				}
			}
			res = mac;
		}
	}
	else
		s = old_s;

	if( nullptr != append )
		*append = add_value;

	// Exit
	return STATUS_SUCCESS;
}

/*END OF parse_identifier.cxx*/
