/*format_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::format_directive::format_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::format_directive::get_name() const
{
	return "format";
}
// {F2FFAF37-1EF2-409a-9BAA-857B3871FFC9}
const f1::guid_t f1::f1pp::format_directive::m_guid =
{ 0xf2ffaf37, 0x1ef2, 0x409a, { 0x9b, 0xaa, 0x85, 0x7b, 0x38, 0x71, 0xff, 0xc9 } };
const f1::guid_t & f1::f1pp::format_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::format_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

// Operations
const status_t f1::f1pp::format_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch macro identifier
	f1par->skip_real_spaces_and_comments();

	f1par->parse_macro_identifier( m_ident );
	if( m_ident.is_empty() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		return STATUS_SUCCESS;
	}

	// Skip '='
	f1par->skip_real_spaces_and_comments();
	if( par->skip_char( '=' ) )
		f1par->skip_real_spaces_and_comments();

	// Fetch expression string and expand macros
	f1par->modify_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode );
	f1par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::format_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_INVALID_TYPE;

	// Parse buffer
	if( 0 == out_buf.get_data_size() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_EXPRESSION );
		return STATUS_SUCCESS;
	}
	const char *	 start = (const char *) out_buf.get_ptr();
	const char * const end = (const char *) out_buf.get_end_ptr();

	// Fetch format string
	m_format.clear();
	f1par->parse_string( m_format, start, end );
	if( m_format.empty() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_STRING );
		return STATUS_SUCCESS;
	}

	// Parse argument list
	// Using direct parsing from buffer since the string is read and glued, all macros are substituted
	pp::arg_list args;
	f1par->skip_real_spaces_and_comments( start, end );
	status = f1par->parse_arg_list( args, nullptr, start, end, '(' == *start ? 0 : '\n' );
	UNREFERENCED_VALUE(status);	// ignore invalid status in expression

	// Check for ending characters
	f1par->skip_real_spaces_and_comments( start, end );
	if( start < end )
		parser.message( c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "format" );

	// Format string
	core::string	value;
	format_string( *f1par, value, m_format, args );

	// Set the value
	core::Tref<pp::Imacro> mac;
	parser.find_macro( mac, m_ident.get_stringslice() );
	if( nullptr == mac )
	{
		// Gather macro attributes
		pp::Imacro::macro_flags_t macro_flags = null;
		if( f1par->exists_directive_attribute( "deprecated" ) )
			macro_flags |= pp::Imacro::macro_flags_t::fl_deprecated;
		if( f1par->exists_directive_attribute( "disabled" ) )
			macro_flags |= pp::Imacro::macro_flags_t::fl_disabled;
		if( f1par->exists_directive_attribute( "protected" ) )
			macro_flags |= pp::Imacro::macro_flags_t::fl_immutable;
		if( f1par->exists_directive_attribute( "weak" ) )
			macro_flags |= pp::Imacro::macro_flags_t::fl_weak;

		unsigned def_flags = 0;
		if( f1par->exists_directive_attribute( "global" ) )
			def_flags |= Idefinition_frame::fl_def_global;
		if( f1par->exists_directive_attribute( "local" ) )
			def_flags |= Idefinition_frame::fl_def_local;
		if( f1par->exists_directive_attribute( "public" ) )
			def_flags |= Idefinition_frame::fl_def_public;

		// Get correct namespace
		core::Tref<ns::name_space> previous_ns = parser.get_ns_context().get_current_namespace();
		if( def_flags & macro_directive::frame::fl_def_global )
			parser.get_ns_context().set_current_namespace( &parser.get_ns_context().get_root_namespace() );
		else if( def_flags & macro_directive::frame::fl_def_public )
			parser.get_ns_context().move_to_parent_namespace();

		// Create macro
		f1par->create_macro( mac, m_ident, pp::string(value), macro_flags );

		if( previous_ns != parser.get_ns_context().get_current_namespace() )
			parser.get_ns_context().set_current_namespace( previous_ns );
	}
	else
		mac->set_value( pp::string(value) );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::format_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
	parser.modify_parser_flags( pp::Iparser::parser_flags_t::null, pp::Iparser::parser_flags_t::fl_expression_mode );
#ifdef _DEBUG
	core::Tref<f1::f1pp::format_directive> drc = type_cast<f1::f1pp::format_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::f1pp::format_directive *>( param )->execute( parser, buf );
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool f1::f1pp::format_directive::get_next_arg( f1::c_pp::parser & f1par, pp::variant & buf, pp::arg_list::arg_desc_list_t::const_iterator & arg_it )
{
	status_t status;

	// Get argument
	if( !arg_it.valid() )
		return false;

	const pp::string *	arg;
	const pp::arg_list::arg_desc_t & ad = *arg_it;
	if( !ad.m_expanded_value.empty() )
		arg = &ad.m_expanded_value;
	else
		arg = &ad.m_value;

	// Evaluate expression
	const char *	 start = (const char *) arg->data();
	const char * const end = (const char *) arg->data_end();
	status = f1par.parse_expression( buf, start, end, 0 );

	// Check result
	if( start < end )
		f1par.message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, "," );

	// Exit
	return true;
}

const status_t f1::f1pp::format_directive::format_string( f1::c_pp::parser & f1par, core::string & buf, const pp::string & format, const pp::arg_list & args )
{
	const char * s = format.data();
	const char * end_s = format.data_end();
	const pp::arg_list::arg_desc_list_t & adl = args.get_arg_list();
	pp::arg_list::arg_desc_list_t::const_iterator arg_it( adl );
	core::string	tmp;
	format_context	fmt;
	const pp::variant *	arg;
	pp::variant		arg_buf;

	const char * start = s;
	for( ; ; ++ s )
	{
		if( s < end_s && '%' != *s )
			continue;

		if( s != start )
			buf.append( start, s - start );

		if( s >= end_s )
			break;
		++ s;
		if( s >= end_s )
		{
			buf.append( s - 1, 1 );
			break;
		}

		// Start of the argument
		arg = get_next_arg( f1par, arg_buf, arg_it ) ? &arg_buf : nullptr;
		if( nullptr == arg )
			return f1par.message( f1pp::syntax::IMSG_ERROR_INSUFFICIENT_ARGUMENTS );
		start = s;

		// Parse flags field
		fmt.m_flags = 0;
		switch( *s )
		{
		case '-':	fmt.m_flags |= fmt_left_align;		++ s;	break;
		case '+':	fmt.m_flags |= fmt_prefix_sign;		++ s;	break;
		case '0':	fmt.m_flags |= fmt_prefix_zeroes;	++ s;	break;
		case ' ':	fmt.m_flags |= fmt_prefix_blanks;	++ s;	break;
		case '#':	fmt.m_flags |= fmt_prefix_sharp;	++ s;	break;
		}

		// Parse width field
		fmt.m_width = 0;
		if( '*' == *s )
			fmt.m_width = -1,
			++ s;
		else if( isdigit( *s ) )
			fmt.m_width = strtol( s, (char **) &s, 10 );

		// Parse precision field
		fmt.m_precision = 0;
		if( '.' == *s )
		{
			++ s;
			if( '*' == *s )
			{
				fmt.m_precision = (long) fetch_int_arg( f1par, *arg );

				arg_it.next();
				arg = get_next_arg( f1par, arg_buf, arg_it ) ? &arg_buf : nullptr;

				++ s;
			}
			else
				fmt.m_precision = strtol( s, (char **) &s, 10 );
		}

		// Parse prefixes
		switch( *s )
		{
		case 'h':
			fmt.m_prefix = pref_half;
			++ s;
			break;
		case 'l':
			fmt.m_prefix = pref_long;
			++ s;
			break;
		case 'I':
			++ s;
			if( s + 1 < end_s && '3' == s[0] && '2' == s[1] )
				fmt.m_prefix = pref_int32,
				s += 2;
			else if( s + 1 < end_s && '6' == s[0] && '4' == s[1] )
				fmt.m_prefix = pref_int64,
				s += 2;
			else
				fmt.m_prefix = pref_int;
			break;
		case 'w':
			fmt.m_prefix = pref_wide;
			++ s;
			break;
		default:
			fmt.m_prefix = pref_none;
			break;
		}

		// Parse format field
		switch( *s )
		{
		case '%':
			tmp.assign_char( '%' );
			break;
		// Characters
		case 'c':
			store_char( f1par, fmt, tmp, *arg, fmt.m_prefix == pref_wide );
			break;
		case 'C':
			store_char( f1par, fmt, tmp, *arg, fmt.m_prefix != pref_half );
			break;
		case 's': case 'S':
			store_str( f1par, fmt, tmp, *arg );
			break;
		// Integer
		case 'd': case 'i':
			store_int( f1par, fmt, tmp, *arg, 10, true );
			break;
		case 'u':
			store_int( f1par, fmt, tmp, *arg, 10, false );
			break;
		case 'o':
			store_int( f1par, fmt, tmp, *arg, 8, false );
			break;
		case 'x':
			store_int( f1par, fmt, tmp, *arg, 16, false, true );
			break;
		case 'X':
			store_int( f1par, fmt, tmp, *arg, 16, false, false );
			break;
		// Floating point (currently unsupported)
		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
		// Other known but unsupported formats
		case 'a':
		case 'n':
		case 'p':
			tmp.assign( start, s - start );
			fmt.m_width = fmt.m_precision = fmt.m_flags = fmt.m_prefix = pref_none;	// clear format context
			f1par.message( c_pp::syntax::IMSG_ERROR_UNSUPPORTED_S, tmp.c_str() );
			break;
		// Unknown formats
		default:
			tmp.assign( start, s - start );
			fmt.m_width = fmt.m_precision = fmt.m_flags = fmt.m_prefix = pref_none;	// clear format context
			f1par.message( f1pp::syntax::IMSG_ERROR_INVALID_S, tmp.c_str() );
			break;
		}

		// Generate field string
		if( 0 != fmt.m_width && tmp.size() < size_t(fmt.m_width) )
		{
			if( !(fmt.m_flags & fmt_left_align) )
			{
				if( fmt.m_flags & fmt_prefix_zeroes )
					buf.append_chars( '0', fmt.m_width - tmp.size() );
				else if( fmt.m_flags & fmt_prefix_sharp )
					buf.append_chars( '#', fmt.m_width - tmp.size() );
				else
					buf.append_chars( '\x20', fmt.m_width - tmp.size() );
				fmt.m_width = long(tmp.size());
			}
		}
		buf.append( tmp.data(), tmp.size() );

		if( 0 != fmt.m_width && tmp.size() < size_t(fmt.m_width) )
			buf.append_chars( '\x20', fmt.m_width - tmp.size() );	// add right spaces

		// Done, iterate
		start = s + 1;
		arg_it.next();
	}

	// Exit
	return STATUS_SUCCESS;
}
// Convert PP-string argument to a signed integer, displaying errors if needed
intmax_t f1::f1pp::format_directive::fetch_int_arg( f1::c_pp::parser & f1par,
	const pp::variant & arg, bool * valid_num )
{
#if 1
	if( nullptr != valid_num )
		*valid_num = true;
	__unreferenced_parameter( f1par );
	return arg.as_int();
#else
	const char * ep = nullptr;
	intmax_t val = _strntoimax( str.data(), str.length(), &ep, 10 );
	if( ep != str.data_end() )
	{
		if( nullptr != valid_num )
			*valid_num = false;
		f1par.message( c_pp::syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );
		return 0;
	}

	if( nullptr != valid_num )
		*valid_num = true;
	return val;
#endif
}
// Convert PP-string argument to an unsigned integer, displaying errors if needed
uintmax_t f1::f1pp::format_directive::fetch_uint_arg( f1::c_pp::parser & f1par,
	const pp::variant & arg, bool * valid_num )
{
#if 1
	if( nullptr != valid_num )
		*valid_num = true;
	__unreferenced_parameter( f1par );
	return arg.as_uint();
#else
	const char * ep = nullptr;
	uintmax_t val = _strntoumax( str.data(), str.length(), &ep, 10 );
	if( ep != str.data_end() )
	{
		if( nullptr != valid_num )
			*valid_num = false;
		f1par.message( c_pp::syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );
		return 0;
	}

	if( nullptr != valid_num )
		*valid_num = true;
	return val;
#endif
}
// Store formatted integer
const status_t f1::f1pp::format_directive::store_int( f1::c_pp::parser & f1par,
	const format_context & fmt, core::string & buf,
	const pp::variant & arg, int radix, bool is_signed, bool is_uppercase )
{
	bool valid_num = false;
	char sbuf[1 + sizeof(intmax_t)*8 + 1];	// maximum needed buffer size (for binary radix)
	char * begin;
	char sign;

	if( is_signed )
	{
		// Fetch numeric value from the argument
		intmax_t val = fetch_int_arg( f1par, arg, &valid_num );
		if( !valid_num )
			return STATUS_ERROR_INVALID_FORMAT;

		// Now format the value
		begin = _imaxtostr( sbuf, _countof(sbuf), val, radix );
		sign = val < 0 ? '-' : '+';
	}
	else
	{
		// Fetch numeric value from the argument
		uintmax_t val = fetch_uint_arg( f1par, arg, &valid_num );
		if( !valid_num )
			return STATUS_ERROR_INVALID_FORMAT;

		// Now format the value
		begin = _umaxtostr( sbuf, _countof(sbuf), val, radix );
		sign = '+';
	}
	if( is_uppercase )
		_strupr( sbuf );
	if( fmt.m_flags & fmt_prefix_sign )
		*--begin = sign;
	buf = begin;

	// Exit
	return STATUS_SUCCESS;
}
// Store formatted character
const status_t f1::f1pp::format_directive::store_char( f1::c_pp::parser & f1par,
	const format_context & fmt, core::string & buf, const pp::variant & arg, bool wide_char )
{
	status_t status;

	// Fetch numeric value from the argument
	bool valid_num = false;
	uintmax_t val = fetch_uint_arg( f1par, arg, &valid_num );
	if( !valid_num )
		return STATUS_ERROR_INVALID_FORMAT;
	uintmax_t initial_val = val;

	// Store numeric value
	if( wide_char )
	{
		f1::unicode::uchar8_t buf8[8];
		size_t bytes_stored = 0, bytes_passed = 0;

		status = f1::unicode::utf8::convert(
			buf8, _countof(buf8),
			(const f1::unicode::uchar16le_t *) &val, val > 0xFFFFUL ? 4 : 2,
			&bytes_stored, &bytes_passed );
		if( status.failed() )
			return f1par.message( f1pp::syntax::IMSG_ERROR_INVALID_CHARACTER_CODE_X, val );

		buf.append( buf8, bytes_stored );
		val &= uintmax_t(-1) << bytes_passed*8;
	}
	else
	{
		buf.append_char( char(val & 0xFF) );
		val -= char(val & 0xFF);
	}
	if( 0 != val )
		return f1par.message( f1pp::syntax::IMSG_ERROR_INVALID_CHARACTER_CODE_X, initial_val );

	// Exit
	__unreferenced_parameter( fmt );
	return STATUS_SUCCESS;
}
// Store formatted string
const status_t f1::f1pp::format_directive::store_str( f1::c_pp::parser & f1par,
	const format_context & fmt, core::string & buf, const pp::variant & arg )
{
	if( pp::variant::ty_string != arg.get_type() )
		return f1par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );

	buf = arg.as_string();

	// Exit
	__unreferenced_parameter( fmt );
	return STATUS_SUCCESS;
}

/*END OF format_directive.cxx*/
