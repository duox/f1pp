/*Iassignment_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::Iassignment_directive::Iassignment_directive( unsigned flags ):
	super( flags ),
	m_finish_callback( nullptr ), m_assignment( nullptr ), m_type_id( pp::variant::ty_none )
{
}

// Properties
const status_t f1::f1pp::Iassignment_directive::set_finish_callback( f1::parser::Iparser::Ibreakpoint_callback * finish_callback )
{
	m_finish_callback = finish_callback;
	return STATUS_SUCCESS;
}

unsigned f1::f1pp::Iassignment_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

// Operations
const status_t f1::f1pp::Iassignment_directive::parse( pp::Iparser & parser )
{
	return parse( parser, 0 );
}
const status_t f1::f1pp::Iassignment_directive::parse( pp::Iparser & parser, int end_ch )
{
	status_t status;
	pp::string ident;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	m_can_modify_target = true;

	// Check for `push' and `pop' forms first
	f1par->skip_real_spaces_and_comments();
	if( par->skip_char( '.' ) )
	{
		f1par->skip_real_spaces_and_comments();

		// Parse identifier
		status = parse_target_name( *f1par, ident );
		if( ident.is_empty() )
		{
			par->skip_to_nl();
			return f1par->message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		}

		// Execute known forms and exit
		return execute_subcommand( *f1par, *par, ident );
	}

	// Parse all unary assignments in place
	for(;;)
	{
		// Fetch prefix unary assignment
		f1par->skip_real_spaces_and_comments();
		core::Tconst_thin_ptr<Iunary_assignment>	prefix_assignment;
		prefix_assignment = parse_unary_assignment( *par );

		// Fetch target identifier
		f1par->skip_real_spaces_and_comments();
		parse_target_name( *f1par, ident );
		if( ident.is_empty() )
		{
			parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
			operator_complete( *f1par );//call_finish_callback();
			return STATUS_SUCCESS;
		}

		if( !parser.check_limit_value_min( pp::limit_t::li_maximum_identifier_length, ident.length() ) )
			parser.message( syntax::IMSG_WARNING_TOO_LONG_IDENTIFIER );

		// Find the target
		bool target_exists = exists_target( *f1par, ident );

		// Parse type specification
		f1par->skip_real_spaces_and_comments();
		if( par->skip_char(':') )
		{
			f1par->skip_real_spaces_and_comments();
			m_type_id = f1::pp::variant::ty_none;
			pp::string type_name;
			f1par->parse_type( m_type_id, &type_name );
			if( f1::pp::variant::ty_none == m_type_id )
			{
				parser.message( f1pp::syntax::IMSG_ERROR_UNKNOWN_TYPE_NAME_S, type_name.c_str() );
				operator_complete( *f1par );//call_finish_callback();
				return STATUS_SUCCESS;
			}
		}

		// Parse postfix unary assignment
		f1par->skip_real_spaces_and_comments();
		m_unary_assignment = parse_unary_assignment( *par );

		// Skip assignment operator
		f1par->skip_real_spaces_and_comments();
		m_assignment = parse_assignment( *par );	// optional assignment operator

		// Create target only if assignment statement is not `?='
		if( !target_exists )
		{
			if( &m_conditional_assignment != m_assignment )
			{
				pp::variant::ty_none != m_type_id ?
					create_target( *f1par, ident,pp::variant( m_type_id ) ):
					create_target( *f1par, ident,pp::variant( intmax_t(0) ) );
			}
		}
		else
		{
			m_can_modify_target = can_modify_target();
			if( !m_can_modify_target )
			{
				parser.message( syntax::IMSG_ERROR_MACRO_IS_IMMUTABLE );
			}
			else
			{
				if( &m_conditional_assignment == m_assignment )
					m_assignment = &m_simple_assignment;
			}
		}

		// Execute prefix assignment
		if( target_exists )
		{
			if( nullptr != prefix_assignment )
			{
				pp::variant value;
				get_target_value( value, m_type_id );
				prefix_assignment->evaluate( parser, value );
				set_target_value( value );
			}
		}

		// Go to macro expansion
		break;
	}

	// Fetch expression string and expand macros
	if( !par->is_nl() )
	{
		size_t len = 0;

		// Skip to new-line or comma
		f1par->skip_real_spaces_and_comments();
		f1par->find_separator( len, ';', end_ch );

		// Go to expanding macros
		f1par->modify_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode );
		f1par->capture_line( line_capture_callback, this, len );
	}
	else
	{
		if( nullptr != m_unary_assignment )
		{
			pp::variant value;
			get_target_value( value, m_type_id );
			m_unary_assignment->evaluate( parser, value );
			m_unary_assignment = nullptr;
			set_target_value( value );
		}
		operator_complete( *f1par );//call_finish_callback();
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::Iassignment_directive::execute_subcommand( f1::f1pp::parser & f1par, f1::parser::Iparser & par, const pp::string & ident )
{
	status_t status;

	if( ident.equal( "push" ) )
	{
		// Check for push `all'
		f1par.skip_real_spaces_and_comments();
		if( par.is_nl() )
			status = push_all_items( f1par );
		else
		{
			core::Tarray<pp::string> lst;
			if( f1par.parse_identifier_and_string_list( lst ).failed_status( status ) )
				return status;

			status = push_items( f1par, lst );
		}
	}
	else if( ident.equal( "pop" ) )
		status = pop_items( f1par );
	else if( ident.equal( "delete" ) )
	{
		// Parse the list of all items
		core::Tarray<pp::string> lst;
		if( f1par.parse_identifier_list( lst ).failed_status( status ) )
			return status;
		if( lst.empty() )
			return f1par.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER_LIST );

		// Execute command
		status = delete_items( f1par, lst );
	}
	else
		// Execute unknown form
		status = execute_unknown_subcommand( f1par, par, ident );
	if( status.failed() )
		par.skip_to_nl();

	// Exit
	return status;
}
const status_t f1::f1pp::Iassignment_directive::execute_unknown_subcommand( f1::f1pp::parser & f1par, f1::parser::Iparser & par, const pp::string & ident )
{
	__unreferenced_parameter( f1par );
	__unreferenced_parameter( par );
	return f1par.message( syntax::IMSG_ERROR_UNKNOWN_COMMAND_S, ident.c_str() );
}
const status_t f1::f1pp::Iassignment_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
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
		operator_complete( *f1par );//call_finish_callback();
		return STATUS_SUCCESS;
	}

	// Parse expression
	// Using direct parsing from buffer since the string is read and glued, all macros are substituted
	const char *	 start = (const char *) out_buf.get_ptr();
	const char * const end = (const char *) out_buf.get_end_ptr();
	pp::variant result( m_type_id );
	if( f1par->parse_expression( result, start, end, 0 ).failed_status( status ) )
	{
		operator_complete( *f1par );//call_finish_callback();
		return status;
	}

	// Process result
	f1par->skip_real_spaces_and_comments( start, end );
	if( start < end )
		parser.message( c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "set" );

	// Evaluate the result and store result to the target
	if( m_can_modify_target && target_created() )
	{
		pp::variant value;
		get_target_value( value, m_type_id );

		if( nullptr != m_assignment )
		{
			status = m_assignment->evaluate( *f1par, value, result );
			if( status.failed() )
			{
				operator_complete( *f1par );//call_finish_callback();
				return STATUS_SUCCESS;
			}
		}
		else
			value.move( result );

		if( nullptr != m_unary_assignment )
			m_unary_assignment->evaluate( parser, value );

		set_target_value( value );
	}

	// Check for exit
	/*if( parser.get_parser()->skip_char( ';' ) )
		parse( parser );
	else
		call_finish_callback();*/
	operator_complete( *f1par );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::Iassignment_directive::operator_complete( f1pp::parser & parser )
{
	if( nullptr != m_finish_callback )
	{
		m_finish_callback->on_breakpoint_reached();
		m_finish_callback = nullptr;
	}

	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}
/*const status_t f1::f1pp::Iassignment_directive::call_finish_callback()
{
	if( nullptr != m_finish_callback )
	{
		m_finish_callback->on_breakpoint_reached();
		m_finish_callback = nullptr;
	}
	return STATUS_SUCCESS;
}*/
const status_t f1::f1pp::Iassignment_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
	parser.modify_parser_flags( pp::Iparser::parser_flags_t::null, pp::Iparser::parser_flags_t::fl_expression_mode );
#ifdef _DEBUG
	core::Tref<f1::f1pp::Iassignment_directive> drc = type_cast<f1::f1pp::Iassignment_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::f1pp::Iassignment_directive *>( param )->execute( parser, buf );
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
const status_t f1::f1pp::Iassignment_directive::get_macro_value( pp::Imacro & mac, pp::variant & value, pp::variant::type_t ty )
{
	value = mac.get_value_variant();
	if( pp::variant::ty_none != ty && value.get_type() != ty )
		value.set_type( ty );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::Iassignment_directive::set_macro_value( pp::Imacro & mac, const pp::variant & value )
{
	core::string	str;
	switch( value.get_type() )
	{
	case pp::variant::ty_none:
		break;	// leave str empty
	case pp::variant::ty_bool:
	case pp::variant::ty_int:
		str.format( "%d", value.as_int() );
		break;
	case pp::variant::ty_uint:
		str.format( "%u", value.as_uint() );
		break;
	case pp::variant::ty_float:
		str.format( "%f", value.as_float() );
		break;
	case pp::variant::ty_string:
		str.format( "\"%s\"", value.as_string() );
		break;
	default:
		__assert( false );
		return STATUS_ERROR_INVALID_TYPE;
	}

	// Set the value
	pp::string	str_value( str );
	mac.set_value_variant( value, str_value );

	// Exit
	return STATUS_SUCCESS;
}*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const f1::f1pp::Iassignment_directive::unary_assignment_operator f1::f1pp::Iassignment_directive::m_intrinsic_unary_assignment_operators[] = {
	{ STR_LEN_PAIR( "++" ),	&m_increment_unary_assignment },
	{ STR_LEN_PAIR( "--" ),	&m_decrement_unary_assignment },
};
const f1::f1pp::Iassignment_directive::Iunary_assignment * f1::f1pp::Iassignment_directive::parse_unary_assignment( f1::parser::Iparser & par ) const
{
	const char * s = par.get_ptr();
	const size_t left = par.data_left();

	for( size_t i = 0; i < _countof(m_intrinsic_unary_assignment_operators); ++ i )
	{
		const unary_assignment_operator & op = m_intrinsic_unary_assignment_operators[i];
		if( left < op.m_length )
			continue;
		if( !__small_memcmp( s, op.m_string, op.m_length ) )
		{
			par.inc_pos( op.m_length );
			return op.m_assignment;
		}
	}
	return nullptr;
}

f1::f1pp::Iassignment_directive::increment_unary_assignment	f1::f1pp::Iassignment_directive::m_increment_unary_assignment;
const status_t f1::f1pp::Iassignment_directive::increment_unary_assignment::evaluate( pp::Iparser & par, pp::variant & val ) const
{
	switch( val.get_type() )
	{
	case pp::variant::ty_int:
		val.set_int( val.as_int() + 1 );
		break;
	case pp::variant::ty_uint:
		val.set_uint( val.as_uint() + 1 );
		break;
	case pp::variant::ty_float:
		val.set_float( val.as_float() + 1 );
		break;
	default:
		par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
		break;
	}
		
	return STATUS_SUCCESS;
}
f1::f1pp::Iassignment_directive::decrement_unary_assignment	f1::f1pp::Iassignment_directive::m_decrement_unary_assignment;
const status_t f1::f1pp::Iassignment_directive::decrement_unary_assignment::evaluate( pp::Iparser & par, pp::variant & val ) const
{
	switch( val.get_type() )
	{
	case pp::variant::ty_int:
		val.set_int( val.as_int() - 1 );
		break;
	case pp::variant::ty_uint:
		val.set_uint( val.as_uint() - 1 );
		break;
	case pp::variant::ty_float:
		val.set_float( val.as_float() - 1 );
		break;
	default:
		par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
		break;
	}

	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const f1::f1pp::Iassignment_directive::assignment_operator f1::f1pp::Iassignment_directive::m_intrinsic_assignment_operators[] = {
	{ STR_LEN_PAIR( "=" ),	&m_simple_assignment },
	{ STR_LEN_PAIR( "+=" ),	&m_addition_assignment },
	{ STR_LEN_PAIR( "-=" ),	&m_subtraction_assignment },
	{ STR_LEN_PAIR( "*=" ),	&m_multiplication_assignment },
	{ STR_LEN_PAIR( "/=" ),	&m_division_assignment },
	{ STR_LEN_PAIR( "%=" ),	&m_modulo_assignment },
	{ STR_LEN_PAIR( "&&=" ),&m_land_assignment },
	{ STR_LEN_PAIR( "||=" ),&m_lor_assignment },
	{ STR_LEN_PAIR( "^^=" ),&m_lxor_assignment },
	{ STR_LEN_PAIR( "&=" ),	&m_bitand_assignment },
	{ STR_LEN_PAIR( "|=" ),	&m_bitor_assignment },
	{ STR_LEN_PAIR( "^=" ),	&m_bitxor_assignment },
	{ STR_LEN_PAIR( "<<=" ),&m_lshift_assignment },
	{ STR_LEN_PAIR( ">>=" ),&m_rshift_assignment },
	{ STR_LEN_PAIR( "?=" ), &m_conditional_assignment },
};
const f1::f1pp::Iassignment_directive::Iassignment * f1::f1pp::Iassignment_directive::parse_assignment( f1::parser::Iparser & par ) const
{
	const char * s = par.get_ptr();
	const size_t left = par.data_left();

	for( size_t i = 0; i < _countof(m_intrinsic_assignment_operators); ++ i )
	{
		const assignment_operator & op = m_intrinsic_assignment_operators[i];
		if( left < op.m_length )
			continue;
		if( !__small_memcmp( s, op.m_string, op.m_length ) )
		{
			par.inc_pos( op.m_length );
			return op.m_assignment;
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

f1::f1pp::Iassignment_directive::simple_assignment f1::f1pp::Iassignment_directive::m_simple_assignment;
const status_t f1::f1pp::Iassignment_directive::simple_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	dst = src;

	__unreferenced_parameter( par );
	return STATUS_SUCCESS;
}
f1::f1pp::Iassignment_directive::addition_assignment	f1::f1pp::Iassignment_directive::m_addition_assignment;
const status_t f1::f1pp::Iassignment_directive::addition_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	if( pp::variant::ty_string == dst.get_type() && pp::variant::ty_string == src.get_type() )
	{
		return dst.set_string_move( _strcata( dst.as_string(), src.as_string() ) );
	}
	if( dst.is_numeric() && src.is_numeric() )
	{
		if( pp::variant::ty_float == dst.get_type() || pp::variant::ty_float == src.get_type() )
			return dst.set_float( dst.as_float() + src.as_float() );
		if( pp::variant::ty_int == dst.get_type() || pp::variant::ty_int == src.get_type() )
			return dst.set_int( dst.as_int() + src.as_int() );
		if( pp::variant::ty_uint == dst.get_type() || pp::variant::ty_uint == src.get_type() )
			return dst.set_uint( dst.as_uint() + src.as_uint() );
	}
	par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
	return STATUS_ERROR_INVALID_TYPE;
}
f1::f1pp::Iassignment_directive::subtraction_assignment f1::f1pp::Iassignment_directive::m_subtraction_assignment;
const status_t f1::f1pp::Iassignment_directive::subtraction_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	if( dst.is_numeric() && src.is_numeric() )
	{
		if( pp::variant::ty_float == dst.get_type() || pp::variant::ty_float == src.get_type() )
			return dst.set_float( dst.as_float() - src.as_float() );
		if( pp::variant::ty_int == dst.get_type() || pp::variant::ty_int == src.get_type() )
			return dst.set_int( dst.as_int() - src.as_int() );
		if( pp::variant::ty_uint == dst.get_type() || pp::variant::ty_uint == src.get_type() )
			return dst.set_uint( dst.as_uint() - src.as_uint() );
	}
	par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
	return STATUS_ERROR_INVALID_TYPE;
}
f1::f1pp::Iassignment_directive::multiplication_assignment f1::f1pp::Iassignment_directive::m_multiplication_assignment;
const status_t f1::f1pp::Iassignment_directive::multiplication_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	if( dst.is_numeric() && src.is_numeric() )
	{
		if( pp::variant::ty_float == dst.get_type() || pp::variant::ty_float == src.get_type() )
			return dst.set_float( dst.as_float() * src.as_float() );
		if( pp::variant::ty_int == dst.get_type() || pp::variant::ty_int == src.get_type() )
			return dst.set_int( dst.as_int() * src.as_int() );
		if( pp::variant::ty_uint == dst.get_type() || pp::variant::ty_uint == src.get_type() )
			return dst.set_uint( dst.as_uint() * src.as_uint() );
	}
	par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
	return STATUS_ERROR_INVALID_TYPE;
}
f1::f1pp::Iassignment_directive::division_assignment f1::f1pp::Iassignment_directive::m_division_assignment;
const status_t f1::f1pp::Iassignment_directive::division_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	if( dst.is_numeric() && src.is_numeric() )
	{
		if( pp::variant::ty_float == dst.get_type() || pp::variant::ty_float == src.get_type() )
		{
			if( src.as_float() < 2e-15 )
				return par.message( c_pp::syntax::IMSG_ERROR_DIVISION_BY_ZERO ), STATUS_ERROR_INVALID_VALUE;
			return dst.set_float( dst.as_float() / src.as_float() );
		}
		if( pp::variant::ty_int == dst.get_type() || pp::variant::ty_int == src.get_type() )
		{
			if( 0 == src.as_int() )
				return par.message( c_pp::syntax::IMSG_ERROR_DIVISION_BY_ZERO ), STATUS_ERROR_INVALID_VALUE;
			return dst.set_int( dst.as_int() / src.as_int() );
		}
		if( pp::variant::ty_uint == dst.get_type() || pp::variant::ty_uint == src.get_type() )
		{
			if( 0 == src.as_uint() )
				return par.message( c_pp::syntax::IMSG_ERROR_DIVISION_BY_ZERO ), STATUS_ERROR_INVALID_VALUE;
			return dst.set_uint( dst.as_uint() / src.as_uint() );
		}
	}
	par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
	return STATUS_ERROR_INVALID_TYPE;
}
f1::f1pp::Iassignment_directive::modulo_assignment f1::f1pp::Iassignment_directive::m_modulo_assignment;
const status_t f1::f1pp::Iassignment_directive::modulo_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	if( dst.is_numeric() && src.is_numeric() )
	{
		if( pp::variant::ty_float == dst.get_type() || pp::variant::ty_float == src.get_type() )
		{
			if( src.as_float() < 2e-15 )
				return par.message( c_pp::syntax::IMSG_ERROR_DIVISION_BY_ZERO ), STATUS_ERROR_INVALID_VALUE;
			return dst.set_float( fmod( dst.as_float(), src.as_float() ) );
		}
		if( pp::variant::ty_int == dst.get_type() || pp::variant::ty_int == src.get_type() )
		{
			if( 0 == src.as_int() )
				return par.message( c_pp::syntax::IMSG_ERROR_DIVISION_BY_ZERO ), STATUS_ERROR_INVALID_VALUE;
			return dst.set_int( dst.as_int() % src.as_int() );
		}
		if( pp::variant::ty_uint == dst.get_type() || pp::variant::ty_uint == src.get_type() )
		{
			if( 0 == src.as_uint() )
				return par.message( c_pp::syntax::IMSG_ERROR_DIVISION_BY_ZERO ), STATUS_ERROR_INVALID_VALUE;
			return dst.set_uint( dst.as_uint() % src.as_uint() );
		}
	}
	par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
	return STATUS_ERROR_INVALID_TYPE;
}

f1::f1pp::Iassignment_directive::land_assignment f1::f1pp::Iassignment_directive::m_land_assignment;
const status_t f1::f1pp::Iassignment_directive::land_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	if( (dst.is_numeric() || pp::variant::ty_bool == dst.get_type()) &&
		(src.is_numeric() || pp::variant::ty_bool == src.get_type()) )
	{
		return dst.set_int( dst.as_bool() && src.as_bool() );
	}
	par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
	return STATUS_ERROR_INVALID_TYPE;
}
f1::f1pp::Iassignment_directive::lor_assignment f1::f1pp::Iassignment_directive::m_lor_assignment;
const status_t f1::f1pp::Iassignment_directive::lor_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	if( (dst.is_numeric() || pp::variant::ty_bool == dst.get_type()) &&
		(src.is_numeric() || pp::variant::ty_bool == src.get_type()) )
	{
		return dst.set_int( dst.as_bool() || src.as_bool() );
	}
	par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
	return STATUS_ERROR_INVALID_TYPE;
}
f1::f1pp::Iassignment_directive::lxor_assignment	f1::f1pp::Iassignment_directive::m_lxor_assignment;
const status_t f1::f1pp::Iassignment_directive::lxor_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	if( (dst.is_numeric() || pp::variant::ty_bool == dst.get_type()) &&
		(src.is_numeric() || pp::variant::ty_bool == src.get_type()) )
	{
		return dst.set_int( dst.as_bool() != src.as_bool() );
	}
	par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
	return STATUS_ERROR_INVALID_TYPE;
}

f1::f1pp::Iassignment_directive::bitand_assignment f1::f1pp::Iassignment_directive::m_bitand_assignment;
const status_t f1::f1pp::Iassignment_directive::bitand_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	if( pp::variant::ty_string == dst.get_type() && pp::variant::ty_string == src.get_type() )
	{
		return dst.set_string_move( _strcata( dst.as_string(), src.as_string() ) );
	}
	if( dst.is_numeric() && src.is_numeric() )
	{
		if( pp::variant::ty_int == dst.get_type() || pp::variant::ty_int == src.get_type() )
			return dst.set_int( dst.as_int() & src.as_int() );
		if( pp::variant::ty_uint == dst.get_type() || pp::variant::ty_uint == src.get_type() )
			return dst.set_uint( dst.as_uint() & src.as_uint() );
	}
	par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
	return STATUS_ERROR_INVALID_TYPE;
}
f1::f1pp::Iassignment_directive::bitor_assignment f1::f1pp::Iassignment_directive::m_bitor_assignment;
const status_t f1::f1pp::Iassignment_directive::bitor_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	if( dst.is_numeric() && src.is_numeric() )
	{
		if( pp::variant::ty_int == dst.get_type() || pp::variant::ty_int == src.get_type() )
			return dst.set_int( dst.as_int() | src.as_int() );
		if( pp::variant::ty_uint == dst.get_type() || pp::variant::ty_uint == src.get_type() )
			return dst.set_uint( dst.as_uint() | src.as_uint() );
	}
	par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
	return STATUS_ERROR_INVALID_TYPE;
}
f1::f1pp::Iassignment_directive::bitxor_assignment  f1::f1pp::Iassignment_directive::m_bitxor_assignment;
const status_t f1::f1pp::Iassignment_directive::bitxor_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	if( dst.is_numeric() && src.is_numeric() )
	{
		if( pp::variant::ty_int == dst.get_type() || pp::variant::ty_int == src.get_type() )
			return dst.set_int( dst.as_int() ^ src.as_int() );
		if( pp::variant::ty_uint == dst.get_type() || pp::variant::ty_uint == src.get_type() )
			return dst.set_uint( dst.as_uint() ^ src.as_uint() );
	}
	par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
	return STATUS_ERROR_INVALID_TYPE;
}

f1::f1pp::Iassignment_directive::lshift_assignment f1::f1pp::Iassignment_directive::m_lshift_assignment;
const status_t f1::f1pp::Iassignment_directive::lshift_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	if( dst.is_numeric() && src.is_numeric() )
	{
		if( pp::variant::ty_int == dst.get_type() || pp::variant::ty_int == src.get_type() )
			return dst.set_int( dst.as_int() << src.as_uint() );
		if( pp::variant::ty_uint == dst.get_type() || pp::variant::ty_uint == src.get_type() )
			return dst.set_uint( dst.as_uint() << src.as_uint() );
	}
	par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
	return STATUS_ERROR_INVALID_TYPE;
}
f1::f1pp::Iassignment_directive::rshift_assignment f1::f1pp::Iassignment_directive::m_rshift_assignment;
const status_t f1::f1pp::Iassignment_directive::rshift_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	if( dst.is_numeric() && src.is_numeric() )
	{
		if( pp::variant::ty_int == dst.get_type() || pp::variant::ty_int == src.get_type() )
			return dst.set_int( dst.as_int() >> src.as_uint() );
		if( pp::variant::ty_uint == dst.get_type() || pp::variant::ty_uint == src.get_type() )
			return dst.set_uint( dst.as_uint() >> src.as_uint() );
	}
	par.message( f1pp::syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );
	return STATUS_ERROR_INVALID_TYPE;
}
f1::f1pp::Iassignment_directive::conditional_assignment f1::f1pp::Iassignment_directive::m_conditional_assignment;
const status_t f1::f1pp::Iassignment_directive::conditional_assignment::evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const
{
	DBG_FAIL();
	__unreferenced_parameter( par );
	__unreferenced_parameter( dst );
	__unreferenced_parameter( src );
	return STATUS_ERROR_INTERNAL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Target API

const status_t f1::f1pp::Iassignment_directive::parse_target_name( f1pp::parser & parser, pp::string & buf )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( buf );
	return STATUS_ERROR_PURE_CALL;
}
const status_t f1::f1pp::Iassignment_directive::create_target( f1pp::parser & parser, const pp::string & ident, const pp::variant & value )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( ident );
	__unreferenced_parameter( value );
	return STATUS_ERROR_PURE_CALL;
}
bool f1::f1pp::Iassignment_directive::exists_target( f1pp::parser & parser, const pp::string & ident )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( ident );
	return false;
}
bool f1::f1pp::Iassignment_directive::target_created() const
{
	DBG_FAIL_ON_PURE_CALL();
	return false;
}

bool f1::f1pp::Iassignment_directive::can_modify_target() const
{
	DBG_FAIL_ON_PURE_CALL();
	return false;
}
const status_t f1::f1pp::Iassignment_directive::get_target_value( pp::variant & var, pp::variant::type_t ty )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( var );
	__unreferenced_parameter( ty );
	return STATUS_ERROR_PURE_CALL;
}
const status_t f1::f1pp::Iassignment_directive::set_target_value( const pp::variant & var )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( var );
	return STATUS_ERROR_PURE_CALL;
}

const status_t f1::f1pp::Iassignment_directive::push_all_items( f1pp::parser & parser )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	return STATUS_ERROR_PURE_CALL;
}
const status_t f1::f1pp::Iassignment_directive::push_items( f1pp::parser & parser, const core::Tarray<pp::string> & ident_list )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( ident_list );
	return STATUS_ERROR_PURE_CALL;
}
const status_t f1::f1pp::Iassignment_directive::pop_items( f1pp::parser & parser )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	return STATUS_ERROR_PURE_CALL;
}
const status_t f1::f1pp::Iassignment_directive::delete_items( f1pp::parser & parser, const core::Tarray<pp::string> & ident_list )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( ident_list );
	return STATUS_ERROR_PURE_CALL;
}

/*END OF Iassignment_directive.cxx*/
