/*parser_expression.cxx*/
/**
 *	Preprocessor expression evaluation.
 *
 *	- recursive, but recursion depth equals to the number of priority levels.
 *	- allocates no dynamic memory.
 *	- extensible, operators and functions can be added.
 *	- simple and easy to maintain.
 */
#include "pch.h"
#include "main.h"

const status_t f1::c_pp::parser::parse_expression(
	pp::variant & result,
	const pp::variant & left_value, const binary_operator_desc_t * left_op,
	const char * & s, const char * end, int end_ch )
{
	status_t status;

//	OutputDebugStringA( core::string( s, end - s ).c_str() );
//	OutputDebugStringA( "\n" );

	// Parse initial value
	skip_spaces_and_comments( s, end );
	pp::variant	right_value( result.get_type() );
	if( parse_operand( right_value, s, end ).failed_status( status ) )
		return status;
	if( c_flags_t::null == (m_c_flags & c_flags_t::fl_disable_expression) )
		result = right_value;

	// Parse all elements to the right
	for(;;)
	{
		// Parse operator
		skip_spaces_and_comments( s, end );
		const char * start_pos = s;
		if( 0 != end_ch && end_ch == *s )
			break;

		const binary_operator_desc_t * right_op = parse_binary_operator( s, end, right_value.get_type() );
		if( nullptr == right_op )
			break;
		if( right_op->m_flags & fl_bop_deprecated )
			message( syntax::IMSG_WARNING_DEPRECATED_OPERATOR_S, right_op->m_name );
		s += right_op->m_len;

		// Basing on prorities, parse right expression
		if( nullptr == left_op || left_op->m_priority < right_op->m_priority )
		{
			// Call operator's preexecute before the right operand is evaluated
			bool was_preexecute_called = false;
			if( c_flags_t::null == (m_c_flags & c_flags_t::fl_disable_expression) && nullptr != right_op->m_preexecute )
			{
				right_op->m_preexecute( *this, result, right_value, s, end );
				was_preexecute_called = true;
			}

			// Evaluate right operand
			if( parse_expression( result, right_value, right_op, s, end, end_ch ).failed_status( status ) )
				return status;
			if( c_flags_t::null == (m_c_flags & c_flags_t::fl_disable_expression) )
				right_value = result;

			// Call postexecute if operator's preexecute was called
			if( was_preexecute_called && nullptr != right_op->m_postexecute )
				right_op->m_postexecute( *this, result, right_value );
		}
		else
		{
			s = start_pos;
			if( c_flags_t::null == (m_c_flags & c_flags_t::fl_disable_expression) )
				left_op->m_execute( *this, result, left_value, right_value );
			return STATUS_SUCCESS;
		}
	}

	if( nullptr != left_op )
	{
		if( c_flags_t::null == (m_c_flags & c_flags_t::fl_disable_expression) )
			left_op->m_execute( *this, result, left_value, right_value );
	}
//	else
//		result = right_value;

	// Check for end of expression
/*	if( (0 == end_ch && s < end) || (0 != end_ch && end_ch != *s) )
	{
		skip_to_newline( s, end );	// skip to nl or eof
		message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );
	}*/	// client should do that

	// Exit
	return STATUS_SUCCESS;
}
/*

1 + 2 * 3 ^ 2 + 5 * 6
1 + 2 * 9 + 5 * 6
1 + 18 + 5 * 6

*/

const status_t f1::c_pp::parser::parse_expression( pp::variant & result, int end_ch )
{
	const char * s = m_parser->get_ptr();
	status_t status = parse_expression( result, s, m_parser->get_end_ptr(), end_ch );
	m_parser->set_ptr( s );
	return status;
}
const status_t f1::c_pp::parser::parse_expression( pp::variant & result, const char * & string, const char * end, int end_ch )
{
	// Parse, evaluate expression and exit
	return parse_expression( result, pp::variant::invalid, nullptr, string, end, end_ch );
}
const status_t f1::c_pp::parser::parse_integer_expression( uint64_t & result, const char * & string, const char * end, int end_ch )
{
	status_t status;

	// Parse and evaluate expression
	pp::variant expr_val;
	if( parse_expression( expr_val, pp::variant::invalid, nullptr, string, end, end_ch ).failed_status( status ) )
		return status;

	// Transform type to boolean
	if( expr_val.set_type( pp::variant::ty_int ).failed_status( status ) )
		return status;
	result = expr_val.as_int();

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::parse_boolean_expression( bool & result, const char * & string, const char * end, int end_ch )
{
	status_t status;

	// Parse and evaluate expression
	pp::variant expr_val;
	if( parse_expression( expr_val, pp::variant::invalid, nullptr, string, end, end_ch ).failed_status( status ) )
		return status;

	// Transform type to boolean
	if( expr_val.set_type( pp::variant::ty_bool ).failed_status( status ) )
		return status;
	result = expr_val.as_bool();

	// Exit
	return STATUS_SUCCESS;
}

/*

1 + 2 * 3 ^ 4 + 5 * 6

1 2 3 4 5 6
 + * ^ + *

*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::parser::parse_operand( pp::variant & result, const char * & s, const char * end )
{
	status_t	status;
	operand		opnd;
	core::Tarray<const unary_operator_desc_t *>	prefix_op_seq;

	// Check current state
	if( s >= end )
	{
		message( syntax::IMSG_ERROR_EXPECTED_OPERAND );
		return STATUS_ERROR_INVALID_SYNTAX;
	}
	opnd.m_value.set_type( result.get_type() );

	// Skip unary prefix operators
	prefix_op_seq.clear();
	for(;;)
	{
		skip_spaces_and_comments( s, end );
		if( s >= end )
			break;

		const unary_operator_desc_t * desc = parse_unary_operator( m_prefix_unary_operator_desc_list, s, end );
		if( nullptr == desc )
			break;
		if( desc->m_flags & fl_uop_deprecated )
			message( syntax::IMSG_WARNING_DEPRECATED_OPERATOR_S, desc->m_name.c_str() );

		s += desc->m_name.size();
		prefix_op_seq.push_back( desc );
	}

	// Fetch operand value
	if( parse_value( opnd, s, end ).failed_status( status ) )
		return status;

	// Process unary prefix operators
	array_foreach( core::Tarray<const unary_operator_desc_t *>::const_iterator, it, prefix_op_seq )
	{
		const unary_operator_desc_t * desc = *it;
		__debugbreak_if( nullptr == desc )
			return STATUS_ERROR_INTERNAL;
		if( desc->m_execute( *this, opnd ).failed_status( status ) )
			return status;
	}

	// Process unary postfix operators
	for(;;)
	{
		skip_spaces_and_comments( s, end );
		if( s >= end )
			break;

		const unary_operator_desc_t * desc = parse_unary_operator( m_postfix_unary_operator_desc_list, s, end );
		if( nullptr == desc )
			break;
		if( desc->m_flags & fl_uop_deprecated )
			message( syntax::IMSG_WARNING_DEPRECATED_OPERATOR_S, desc->m_name.c_str() );

		s += desc->m_name.size();
		if( desc->m_execute( *this, opnd ).failed_status( status ) )
			return status;
	}

	// Exit
	result = opnd.m_value;
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::parse_value( pp::variant & value, const char * & s, const char * end_s, unsigned flags )
{
	status_t status;

	// Check current state
	if( s >= end_s )
	{
		message( syntax::IMSG_ERROR_EXPECTED_VALUE );
		return STATUS_ERROR_INVALID_SYNTAX;
	}

	// Parse value
	int ch = *s;
	if( '\'' == ch || ('L' == ch && '\'' == s[1] ) )
	{
		intmax_t val;
		if( 'L' == *s )
			++ s, ++ s,
			status = parse_wchar( val, s, end_s );
		else
			++ s,
			status = parse_char( val, s, end_s );
		if( status.failed() )
			return status;

		if( '\'' != *s )
		{
			message( syntax::IMSG_ERROR_TOO_LONG_CHAR_CONSTANT );
			return STATUS_SUCCESS;
		}
		++ s;

		status = get_c_flags( c_flags_t::fl_unsigned_char ) ?
			value.set_uint( val ):
			value.set_int( val );
	}
	else if( isdigit( ch ) )
	{
		if( pp::variant::ty_float == value.get_type() )
		{
			parse_float( value, s, end_s );
		}
		else if( parse_integer( value, s, end_s ) )
		{
			if( s < end_s && ('.' == *s || 'e' == *s || 'E' == *s || 'p' == *s || 'P' == *s) )
			{
				pp::variant val;
				if( parse_float( val, s, end_s ) )
					value.set_float( value.as_int() + val.as_float() );
			}
		}
	}
	else if( is_ident_first_char( ch ) )
	{
		// Parse identifier
		const char * start_pos = s;
		pp::string ident;
		if( !parse_macro_identifier( ident, s, end_s ) )
		{
			s = start_pos;
			return STATUS_ERROR_INTERNAL;
		}
		skip_spaces_and_comments( s, end_s );

		// Parse optional list of parameters
		// Even if macro is not defined and expands to "0", leaving parameters would break expression syntax,
		// for example: "undefined_macro( 1, 2 )" => "0( 1, 2)", so I suggest it would be nice to ignore
		// parameters to turn entire invocation of undefined function-like macro to "0".
		core::Tref<pp::arg_list> args;
		if( s < end_s && '(' == *s )
		{
			if( parse_arg_list( args, nullptr, s, end_s ).failed_status( status ) )
				return status;
		}

		// Find macro
		core::Tref<pp::Imacro> mac;
		if( find_macro( mac, ident, *args ).succeeded() && __verify(nullptr != mac) )
		{
			if( nullptr == args && nullptr == mac->get_param_list() )
			{
				parse_integer( value, mac->get_value() );
			}
			else
			{
				pp::string buf;
				if( mac->expand( *this, buf, args ).failed_status( status ) )
					return status;
				parse_integer( value, buf );
			}
			//opnd.m_macro = mac;
		}
		else if( ident.equal( "true", 4 ) )
		{
			value.set_int( 1);
		}
		else if( ident.equal( "false", 5 ) )
		{
			value.set_int( 0 );
		}
		else
		{
			// Find function
			/*core::Tref<pp::Ifunction> func;
			if( find_function( func, ident.get_stringslice() ).succeeded() )
			{
				// Parse function
				if( func->parse( *this, value ).failed_status( status ) )
					return status;
			}
			else*/
			{
				// Try to hand processing of unknown macro to user
				value.set_type( pp::variant::ty_none );
				on_unknown_expression_macro( ident, value );
				if( value.is_empty() )
				{
					// If unknown identifiers must be ignored, just exit
					if( flags & fl_parse_value_ignore_unknown_idents )
					{
						s = start_pos;
						return STATUS_SUCCESS;
					}

					// Skip argument list if it is present (we do it only in expression mode)
					if( m_parser->peek_char( '(' ) )
					{
						core::Tref<pp::arg_list> temp_args;
						parse_arg_list( temp_args, nullptr, 0 );
					}

					// Display warning, macro is still not found
					//s = start_pos;
					if( nullptr == mac && !(m_c_flags & c_flags_t::fl_disable_expression) )
						message( syntax::IMSG_WARNING_UNDEFINED_MACRO_IN_EXPRESSION, ident.c_str() );

					// Exit
					value.set_int( 0 );	// unknown identifier is considered to be 0
					return STATUS_SUCCESS;
				}
			}
		}
	}
	else if( '(' == ch )
	{
		++ s;
		parse_expression( value, s, end_s, ')' );
		if( ')' != *s )
			message( syntax::IMSG_ERROR_EXPECTED_S, ")" );
		++ s;
	}
	else
	{
		message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );
		skip_to_newline( s, end_s );
		return STATUS_ERROR_INVALID_SYNTAX;
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::parse_value( operand & opnd, const char * & s, const char * end_s, unsigned flags )
{
	status_t status;

	// Parse value itself
//	opnd.m_type = operand_type_t::operand_type_null;
//	const char * start = s;
	if( parse_value( opnd.m_value, s, end_s, flags ).failed_status( status ) )
		return status;
//		opnd.m_type = operand_type_t::operand_type_numeric;

	// Done
//	opnd.m_range.set_range( 0, s - start );	//start, s - start );

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::on_unknown_expression_macro( const pp::string & ident, pp::variant & value )
{
	__unreferenced_parameter( ident );
	__unreferenced_parameter( value );
	return STATUS_INFORMATION_NOT_PROCESSED;	//message( syntax::IMSG_WARNING_UNDEFINED_IDENTIFIER_IN_EXPR, ident.c_str() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::parser::add_unary_operator( unary_operator_desc_list_t & op_list, execute_unary_operator_fn * execute, const char * name, size_t len, unsigned flags )
{
	// Check current state
	if( size_t(-1) == len )
		len = __small_strlen( name );

	// Check whether this operator already exists
	// TODO: can we just overload existing operator?

	// Add operator descriptor
	unary_operator_desc_t * desc = op_list.append_new();
	desc->m_execute = execute;
	desc->m_name.assign( name, len );
	desc->m_flags = flags;

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::remove_unary_operator( unary_operator_desc_list_t & op_list, execute_unary_operator_fn * execute )
{
	core::Tarray<unary_operator_desc_t>	desc_list;
	array_foreach( core::Tarray<unary_operator_desc_t>::const_iterator, it, op_list )
	{
		const unary_operator_desc_t & desc = *it;
		if( desc.m_execute == execute )
			continue;
		desc_list.append( desc );
	}
	op_list.move( desc_list );

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::remove_unary_operator( unary_operator_desc_list_t & op_list, const char * name, size_t len )
{
	if( size_t(-1) == len )
		len = __small_strlen( name );

	core::Tarray<unary_operator_desc_t>	desc_list;
	array_foreach( core::Tarray<unary_operator_desc_t>::const_iterator, it, op_list )
	{
		const unary_operator_desc_t & desc = *it;
		if( desc.m_name.size() != len )
			continue;
		if( !__small_memcmp( name, desc.m_name.data(), desc.m_name.size() ) )
			continue;
		desc_list.append( desc );
	}
	op_list.move( desc_list );

	return STATUS_SUCCESS;
}

const f1::c_pp::parser::unary_operator_desc_t * f1::c_pp::parser::parse_unary_operator( const unary_operator_desc_list_t & op_list, const char * s, const char * end )
{
	size_t len_left = end - s;
	array_foreach( core::Tarray<unary_operator_desc_t>::const_reverse_iterator, it, op_list )
	{
		const unary_operator_desc_t & desc = *it;
		if( len_left < desc.m_name.size() )
			continue;
		if( !__small_memcmp( s, desc.m_name.data(), desc.m_name.size() ) )
			return &desc;
	}
	return nullptr;
}
const f1::c_pp::parser::unary_operator_desc_t * f1::c_pp::parser::parse_prefix_unary_operator( const char * s, const char * end )
{
	return parse_unary_operator( m_prefix_unary_operator_desc_list, s, end );
}
const f1::c_pp::parser::unary_operator_desc_t * f1::c_pp::parser::parse_postfix_unary_operator( const char * s, const char * end )
{
	return parse_unary_operator( m_postfix_unary_operator_desc_list, s, end );
}
const f1::c_pp::parser::unary_operator_desc_t * f1::c_pp::parser::parse_prefix_unary_operator()
{
	return parse_unary_operator( m_prefix_unary_operator_desc_list, m_parser->get_ptr(), m_parser->get_end_ptr() );
}
const f1::c_pp::parser::unary_operator_desc_t * f1::c_pp::parser::parse_postfix_unary_operator()
{
	return parse_unary_operator( m_postfix_unary_operator_desc_list, m_parser->get_ptr(), m_parser->get_end_ptr() );
}

const status_t f1::c_pp::parser::unary_invalid_op( c_pp::parser & par, operand & opnd )
{
	par.message( syntax::IMSG_ERROR_OPERATOR_IS_NOT_SUPPORTED );
	__unreferenced_parameter( par );
	__unreferenced_parameter( opnd );
	return STATUS_ERROR_NOT_SUPPORTED;
}

const status_t f1::c_pp::parser::unary_lnot( c_pp::parser & par, operand & opnd )
{
	__unreferenced_parameter( par );

	// Perform type selection
	switch( opnd.m_value.get_type() )
	{
	case f1::pp::variant::ty_bool:
		return opnd.m_value.set_bool( ! opnd.m_value.as_bool() );
	case f1::pp::variant::ty_int:
		return opnd.m_value.set_int( ! opnd.m_value.as_int() );
	case f1::pp::variant::ty_uint:
		return opnd.m_value.set_uint( ! opnd.m_value.as_uint() );
	default:
		DBG_FAIL();
		return STATUS_ERROR_INVALID_TYPE;
	}
}
const status_t f1::c_pp::parser::unary_not( c_pp::parser & par, operand & opnd )
{
	__unreferenced_parameter( par );

	// Perform type selection
	switch( opnd.m_value.get_type() )
	{
	case f1::pp::variant::ty_int:
		return opnd.m_value.set_int( ~ opnd.m_value.as_int() );
	case f1::pp::variant::ty_uint:
		return opnd.m_value.set_uint( ~ opnd.m_value.as_uint() );
	default:
		DBG_FAIL();
		return STATUS_ERROR_INVALID_TYPE;
	}
}
const status_t f1::c_pp::parser::unary_minus( c_pp::parser & par, operand & opnd )
{
	// Perform type selection
	switch( opnd.m_value.get_type() )
	{
	case f1::pp::variant::ty_int:
		return opnd.m_value.set_int( - opnd.m_value.as_int() );
	case f1::pp::variant::ty_uint:
		par.message( syntax::IMSG_WARNING_UNARY_MINUS_APPLIED_TO_UINT );
		return opnd.m_value.set_uint( (uintmax_t) - (intmax_t) opnd.m_value.as_uint() );
	case f1::pp::variant::ty_float:
		return opnd.m_value.set_float( - opnd.m_value.as_float() );
	default:
		DBG_FAIL();
		return STATUS_ERROR_INVALID_TYPE;
	}
}
const status_t f1::c_pp::parser::unary_plus( c_pp::parser & par, operand & opnd )
{
	// Perform type selection
	switch( opnd.m_value.get_type() )
	{
	case f1::pp::variant::ty_int:
		return opnd.m_value.set_int( + opnd.m_value.as_int() );
	case f1::pp::variant::ty_uint:
		par.message( syntax::IMSG_WARNING_UNARY_PLUS_APPLIED_TO_UINT );
		return opnd.m_value.set_uint( + opnd.m_value.as_uint() );
	case f1::pp::variant::ty_float:
		return opnd.m_value.set_float( + opnd.m_value.as_float() );
	default:
		DBG_FAIL();
		return STATUS_ERROR_INVALID_TYPE;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning(push)
#pragma warning(disable:4510)	//: 'binary_operator_desc_t' : default constructor could not be generated
#pragma warning(disable:4512)	//: 'binary_operator_desc_t' : assignment operator could not be generated
#pragma warning(disable:4610)	//: struct 'binary_operator_desc_t' can never be instantiated - user defined constructor required
const f1::c_pp::parser::binary_operator_desc_t f1::c_pp::parser::m_default_binary_operator_desc[28] = {
	{ STR_LEN_PAIR( "*" ),		11,	f1::c_pp::parser::execute_mul },
	{ STR_LEN_PAIR( "/" ),		11,	f1::c_pp::parser::execute_div },
	{ STR_LEN_PAIR( "%" ),		11,	f1::c_pp::parser::execute_rem },

	{ STR_LEN_PAIR( "+" ),		10,	f1::c_pp::parser::execute_cat,	nullptr,							nullptr,							0,					pp::variant::ty_string,	pp::variant::ty_string },
	{ STR_LEN_PAIR( "+" ),		10,	f1::c_pp::parser::execute_add },
	{ STR_LEN_PAIR( "-" ),		10,	f1::c_pp::parser::execute_sub },

	{ STR_LEN_PAIR( "<<" ),		 9,	f1::c_pp::parser::execute_shl },
	{ STR_LEN_PAIR( ">>" ),		 9,	f1::c_pp::parser::execute_shr },

	{ STR_LEN_PAIR( ">" ),		 8,	f1::c_pp::parser::execute_greater },
	{ STR_LEN_PAIR( ">=" ),		 8,	f1::c_pp::parser::execute_greater_equ },
	{ STR_LEN_PAIR( "<" ),		 8,	f1::c_pp::parser::execute_less },
	{ STR_LEN_PAIR( "<=" ),		 8,	f1::c_pp::parser::execute_less_equ },

	{ STR_LEN_PAIR( "==" ),		 7,	f1::c_pp::parser::execute_equ },
	{ STR_LEN_PAIR( "!=" ),		 7,	f1::c_pp::parser::execute_not_equ },
	{ STR_LEN_PAIR( "not_eq" ),  7,	f1::c_pp::parser::execute_not_equ,	nullptr,						nullptr,							fl_bop_deprecated },

	{ STR_LEN_PAIR( "&" ),		 6,	f1::c_pp::parser::execute_cat,	nullptr,							nullptr,							0,					pp::variant::ty_string,	pp::variant::ty_string },
	{ STR_LEN_PAIR( "&&" ),		 3,	f1::c_pp::parser::execute_land,	f1::c_pp::parser::preexecute_land,	f1::c_pp::parser::postexecute_land },
	{ STR_LEN_PAIR( "and" ),	 3,	f1::c_pp::parser::execute_land,	f1::c_pp::parser::preexecute_land,	f1::c_pp::parser::postexecute_land,	fl_bop_deprecated },
	{ STR_LEN_PAIR( "&" ),		 6,	f1::c_pp::parser::execute_and },
	{ STR_LEN_PAIR( "bitand" ),	 6,	f1::c_pp::parser::execute_and,	nullptr,							nullptr,							fl_bop_deprecated },

	{ STR_LEN_PAIR( "^" ),		 5,	f1::c_pp::parser::execute_xor },
	{ STR_LEN_PAIR( "xor" ),	 5,	f1::c_pp::parser::execute_xor,	nullptr,							nullptr,							fl_bop_deprecated },

	{ STR_LEN_PAIR( "||" ),		 2,	f1::c_pp::parser::execute_lor,	f1::c_pp::parser::preexecute_lor,	f1::c_pp::parser::postexecute_lor },
	{ STR_LEN_PAIR( "or" ),		 2,	f1::c_pp::parser::execute_lor,	f1::c_pp::parser::preexecute_lor,	f1::c_pp::parser::postexecute_lor,	fl_bop_deprecated },
	{ STR_LEN_PAIR( "|" ),		 4,	f1::c_pp::parser::execute_or },
	{ STR_LEN_PAIR( "bitor" ),	 4,	f1::c_pp::parser::execute_or,	nullptr,							nullptr,							fl_bop_deprecated },

	{ STR_LEN_PAIR( "?" ),		 1,	f1::c_pp::parser::execute_select,	f1::c_pp::parser::preexecute_select,	f1::c_pp::parser::postexecute_select },

	{ STR_LEN_PAIR( "," ),		 0,	f1::c_pp::parser::execute_seq },
};
#pragma warning(pop)

const status_t f1::c_pp::parser::add_binary_operators( const binary_operator_desc_t * descs, size_t count )
{
	for( size_t i = 0; i < count; ++ i )
	{
		m_binary_operator_desc_list.push_back( descs[i] );
	}
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::remove_binary_operators( const binary_operator_desc_t * descs, size_t count )
{
	for( size_t i = 0; i < count; ++ i )
	{
		array_foreach( core::Tarray<binary_operator_desc_t>::iterator, it, m_binary_operator_desc_list )
		{
			const binary_operator_desc_t & desc = *it;
			if( descs[i].m_len != desc.m_len )
				continue;
			if( !__small_memcmp( descs[i].m_name, desc.m_name, desc.m_len ) )
			{
				m_binary_operator_desc_list.erase( it );
				break;
			}
		}
	}
	return STATUS_SUCCESS;
}

const f1::c_pp::parser::binary_operator_desc_t * f1::c_pp::parser::parse_binary_operator( const char * s, const char * end,
	f1::pp::variant::type_t left_operand_type )
{
	const size_t len_left = end - s;

#if 1
	size_t best_len = 0;
	const binary_operator_desc_t * best_op = nullptr;
	array_foreach( core::Tarray<binary_operator_desc_t>::const_iterator/*const_reverse_iterator*/, it, m_binary_operator_desc_list )
	{
		const binary_operator_desc_t & desc = *it;
		if( len_left < desc.m_len )
			continue;
		if( desc.m_len <= best_len )
			continue;
		if( !__small_memcmp( s, desc.m_name, desc.m_len ) )
		{
			if( f1::pp::variant::ty_none == desc.m_left_arg_type || desc.m_left_arg_type == left_operand_type )
			{
				best_len = desc.m_len;
				best_op = &desc;
			}
		}
	}
	return best_op;
#else
	for( size_t i = 0; i < countof(binary_operator_desc); ++ i )
	{
		if( len_left < binary_operator_desc[i].m_len )
			continue;
		if( !__small_memcmp( s, binary_operator_desc[i].m_name, binary_operator_desc[i].m_len ) )
			return &binary_operator_desc[i];
	}
	return nullptr;
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::parser::binary_invalid_op( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )
{
	par.message( syntax::IMSG_ERROR_OPERATOR_IS_NOT_SUPPORTED );
	__unreferenced_parameter( par );
	__unreferenced_parameter( result );
	__unreferenced_parameter( left_value );
	__unreferenced_parameter( right_value );
	return STATUS_ERROR_NOT_SUPPORTED;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// STRING OPERATORS

const status_t f1::c_pp::parser::execute_cat(
	c_pp::parser & par,
	pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )
{
	// Reject invalid types
	__debugbreak_if( f1::pp::variant::ty_string != left_value.get_type() && f1::pp::variant::ty_string != right_value.get_type() )
		return STATUS_ERROR_INVALID_TYPE;

	// Set the value
	result.set_string_move( _strcata( left_value.as_string(), right_value.as_string() ) );

	// Exit
	__unreferenced_parameter( par );
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OTHER OPERATORS

const status_t f1::c_pp::parser::execute_seq(
	c_pp::parser & par,
	pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )
{
	// Set the value
	result = right_value;

	// Exit
	__unreferenced_parameter( par );
	__unreferenced_parameter( left_value );
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ARITHMETIC OPERATORS

const status_t f1::c_pp::parser::execute_binary_arith_op(
	c_pp::parser & par,
	int_binary_arith_op_fn * int_fn, uint_binary_arith_op_fn * uint_fn, float_binary_arith_op_fn * float_fn, 
	pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )
{
	// Reject early some cases
	__debugbreak_if( f1::pp::variant::ty_bool == left_value.get_type() || f1::pp::variant::ty_bool == right_value.get_type() )
		return STATUS_ERROR_INVALID_TYPE;

	// Perform type selection
	switch( left_value.get_type() )
	{
	case f1::pp::variant::ty_uint:
		switch( right_value.get_type() )
		{
		case f1::pp::variant::ty_int:
		case f1::pp::variant::ty_uint:
			if( left_value.get_type() != right_value.get_type() )
				par.message( syntax::IMSG_WARNING_MIXED_SIGNEDNESS );
			return result.set_uint( uint_fn( left_value.as_uint(), right_value.as_uint() ) );
		default:
			;
		}
		// fallthrough
	case f1::pp::variant::ty_int:
		switch( right_value.get_type() )
		{
		case f1::pp::variant::ty_int:
			return result.set_int( int_fn( left_value.as_int(), right_value.as_int() ) );
		case f1::pp::variant::ty_uint:
			par.message( syntax::IMSG_WARNING_MIXED_SIGNEDNESS );
			return result.set_uint( uint_fn( left_value.as_int(), right_value.as_int() ) );
		case f1::pp::variant::ty_float:
			return result.set_float( float_fn( left_value.as_float(), right_value.as_float() ) );
		default:
			DBG_FAIL();
			return STATUS_ERROR_INVALID_TYPE;
		}
		break;
	case f1::pp::variant::ty_float:
		switch( right_value.get_type() )
		{
		case f1::pp::variant::ty_int:
		case f1::pp::variant::ty_uint:
		case f1::pp::variant::ty_float:
			return result.set_float( float_fn( left_value.as_float(), right_value.as_float() ) );
		default:
			DBG_FAIL();
			return STATUS_ERROR_INVALID_TYPE;
		}
		break;
	default:
		DBG_FAIL();
		return STATUS_ERROR_INVALID_TYPE;
	}
}

#define BINARY_ARITH_OP_IMPLEMENT_EX( name, op )	\
intmax_t f1::c_pp::parser::execute_##name##_int( intmax_t left_value, intmax_t right_value )\
{\
	return left_value op right_value;\
}\
uintmax_t f1::c_pp::parser::execute_##name##_uint( uintmax_t left_value, uintmax_t right_value )\
{\
	return left_value op right_value;\
}\
max_float_t f1::c_pp::parser::execute_##name##_float( max_float_t left_value, max_float_t right_value )\
{\
	return left_value op right_value;\
}
#define BINARY_ARITH_OP_IMPLEMENT( name, op )	\
const status_t f1::c_pp::parser::execute_##name( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )\
{\
	return execute_binary_arith_op( par,\
		execute_##name##_int, execute_##name##_uint, execute_##name##_float,\
		result, left_value,	right_value );\
}\
BINARY_ARITH_OP_IMPLEMENT_EX( name, op )

BINARY_ARITH_OP_IMPLEMENT( add, + )
BINARY_ARITH_OP_IMPLEMENT( sub, - )
BINARY_ARITH_OP_IMPLEMENT( mul, * )

const status_t f1::c_pp::parser::execute_div( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )
{
	if( 0 == right_value.as_int() )
		return par.message( syntax::IMSG_ERROR_DIVISION_BY_ZERO );
	return execute_binary_arith_op( par,
		execute_div_int, execute_div_uint, execute_div_float,
		result, left_value,	right_value );
}
BINARY_ARITH_OP_IMPLEMENT_EX( div, / )

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SHIFT OPERATORS

const status_t f1::c_pp::parser::execute_binary_bitwise_op(
	int_binary_bitwise_op_fn * int_fn, uint_binary_bitwise_op_fn * uint_fn,
	pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )
{
	// Concatenate strings
	if( pp::variant::ty_string == left_value.get_type() && pp::variant::ty_string == left_value.get_type() )
	{
		char * buf = _strcata( left_value.as_string(), right_value.as_string() );
		__assert( nullptr != buf );	// TODO
		return result.set_string_move( buf );
	}

	// Reject early some cases
	__debugbreak_if( 
		(!left_value .can_cast_to( f1::pp::variant::ty_int ) && !left_value .can_cast_to( f1::pp::variant::ty_uint )) ||
		(!right_value.can_cast_to( f1::pp::variant::ty_int ) && !right_value.can_cast_to( f1::pp::variant::ty_uint ))
		)
		return STATUS_ERROR_INVALID_TYPE;

	// Perform operation
	if( f1::pp::variant::ty_uint == left_value.get_type() || f1::pp::variant::ty_uint == right_value.get_type() )
		return result.set_uint( uint_fn( left_value.as_uint(), right_value.as_uint() ) );
	return result.set_int( int_fn( left_value.as_int(), right_value.as_int() ) );
}

#define BINARY_BITWISE_OP_IMPLEMENT_EX( name, op )	\
intmax_t f1::c_pp::parser::execute_##name##_int( intmax_t left_value, intmax_t right_value )\
{\
	return left_value op right_value;\
}\
uintmax_t f1::c_pp::parser::execute_##name##_uint( uintmax_t left_value, uintmax_t right_value )\
{\
	return left_value op right_value;\
}
#define BINARY_BITWISE_OP_IMPLEMENT( name, op )	\
const status_t f1::c_pp::parser::execute_##name( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )\
{\
	__unreferenced_parameter( par );\
	return execute_binary_bitwise_op(\
		execute_##name##_int, execute_##name##_uint,\
		result, left_value,	right_value );\
}\
BINARY_BITWISE_OP_IMPLEMENT_EX( name, op )

BINARY_BITWISE_OP_IMPLEMENT( and, & )
BINARY_BITWISE_OP_IMPLEMENT(  or, | )
BINARY_BITWISE_OP_IMPLEMENT( xor, ^ )

const status_t f1::c_pp::parser::execute_shl( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )
{
	if( right_value.as_int() < 0 )
		return par.message( syntax::IMSG_WARNING_NEGATIVE_SHIFT_AMOUNT );
	return execute_binary_bitwise_op(
		execute_shl_int, execute_shl_uint,
		result, left_value,	right_value );
}
BINARY_BITWISE_OP_IMPLEMENT_EX( shl, << )

const status_t f1::c_pp::parser::execute_shr( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )
{
	if( right_value.as_int() < 0 )
		return par.message( syntax::IMSG_WARNING_NEGATIVE_SHIFT_AMOUNT );
	return execute_binary_bitwise_op(
		execute_shr_int, execute_shr_uint,
		result, left_value,	right_value );
}
BINARY_BITWISE_OP_IMPLEMENT_EX( shr, >> )

const status_t f1::c_pp::parser::execute_rem( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )
{
	if( 0 == right_value.as_int() )
		return par.message( syntax::IMSG_ERROR_DIVISION_BY_ZERO );
	return execute_binary_bitwise_op(
		execute_rem_int, execute_rem_uint,
		result, left_value,	right_value );
}
BINARY_BITWISE_OP_IMPLEMENT_EX( rem, % )

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// COMPARISON OPERATORS

const status_t f1::c_pp::parser::execute_binary_compare_op(
	c_pp::parser & par,
	bool_binary_compare_op_fn * bool_fn, int_binary_compare_op_fn * int_fn, uint_binary_compare_op_fn * uint_fn, float_binary_compare_op_fn * float_fn, 
	pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )
{
	// Perform type selection
	switch( left_value.get_type() )
	{
	case f1::pp::variant::ty_bool:
		switch( right_value.get_type() )
		{
		case f1::pp::variant::ty_bool:
			return result.set_bool( bool_fn( left_value.as_bool(), right_value.as_bool() ) );
		case f1::pp::variant::ty_int:
			return result.set_bool( int_fn( left_value.as_int(), right_value.as_int() ) );
		case f1::pp::variant::ty_uint:
			return result.set_bool( uint_fn( left_value.as_uint(), right_value.as_uint() ) );
		case f1::pp::variant::ty_float:
			return result.set_bool( float_fn( left_value.as_float(), right_value.as_float() ) );
		default:
			DBG_FAIL();
			return STATUS_ERROR_INVALID_TYPE;
		}
	case f1::pp::variant::ty_uint:
		switch( right_value.get_type() )
		{
		case f1::pp::variant::ty_int:
			par.message( syntax::IMSG_WARNING_MIXED_SIGNEDNESS );
			__fallthrough;
		case f1::pp::variant::ty_uint:
			return result.set_bool( uint_fn( left_value.as_uint(), right_value.as_uint() ) );
		default:
			;
		}
		__fallthrough;
	case f1::pp::variant::ty_int:
		switch( right_value.get_type() )
		{
		case f1::pp::variant::ty_bool:
		case f1::pp::variant::ty_int:
			return result.set_bool( int_fn( left_value.as_int(), right_value.as_int() ) );
		case f1::pp::variant::ty_uint:
			par.message( syntax::IMSG_WARNING_MIXED_SIGNEDNESS );
			return result.set_bool( uint_fn( left_value.as_uint(), right_value.as_uint() ) );
		case f1::pp::variant::ty_float:
			return result.set_bool( float_fn( left_value.as_float(), right_value.as_float() ) );
		default:
			DBG_FAIL();
			return STATUS_ERROR_INVALID_TYPE;
		}
		break;
	case f1::pp::variant::ty_float:
		switch( right_value.get_type() )
		{
		case f1::pp::variant::ty_bool:
		case f1::pp::variant::ty_int:
		case f1::pp::variant::ty_uint:
		case f1::pp::variant::ty_float:
			return result.set_bool( float_fn( left_value.as_float(), right_value.as_float() ) );
		default:
			DBG_FAIL();
			return STATUS_ERROR_INVALID_TYPE;
		}
		break;
	default:
		DBG_FAIL();
		return STATUS_ERROR_INVALID_TYPE;
	}
}
#define BINARY_COMPARE_OP_IMPLEMENT( name, op )	\
const status_t f1::c_pp::parser::execute_##name( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )\
{\
	if( &execute_##name == &execute_equ && f1::pp::variant::ty_string == left_value.get_type() && f1::pp::variant::ty_string == right_value.get_type() )\
	{\
		return result.set_bool( !strcmp( left_value.as_string(), right_value.as_string() ) );\
	}\
	return execute_binary_compare_op(\
		par,\
		execute_##name##_bool, execute_##name##_int, execute_##name##_uint, execute_##name##_float,\
		result, left_value,	right_value );\
}\
bool f1::c_pp::parser::execute_##name##_bool( bool left_value, bool right_value )\
{\
	return left_value op right_value;\
}\
bool f1::c_pp::parser::execute_##name##_int( intmax_t left_value, intmax_t right_value )\
{\
	return left_value op right_value;\
}\
bool f1::c_pp::parser::execute_##name##_uint( uintmax_t left_value, uintmax_t right_value )\
{\
	return left_value op right_value;\
}\
bool f1::c_pp::parser::execute_##name##_float( max_float_t left_value, max_float_t right_value )\
{\
	return left_value op right_value;\
}
BINARY_COMPARE_OP_IMPLEMENT( equ,			== )
BINARY_COMPARE_OP_IMPLEMENT( not_equ,		!= )
BINARY_COMPARE_OP_IMPLEMENT( greater,		> )
BINARY_COMPARE_OP_IMPLEMENT( greater_equ,	>= )
BINARY_COMPARE_OP_IMPLEMENT( less,			< )
BINARY_COMPARE_OP_IMPLEMENT( less_equ,		<= )

BINARY_COMPARE_OP_IMPLEMENT( land,		&& )
BINARY_COMPARE_OP_IMPLEMENT( lor,		|| )

const status_t f1::c_pp::parser::preexecute_land( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const char * & s, const char * end )
{
	if( !left_value.as_bool() )
		par.modify_c_flags( c_flags_t::fl_disable_expression );
	__unreferenced_parameter( result );
	__unreferenced_parameter( s );
	__unreferenced_parameter( end );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::postexecute_land( c_pp::parser & par, pp::variant & result, const pp::variant & left_value )
{
	if( par.get_c_flags( c_flags_t::fl_disable_expression ) )
		par.modify_c_flags( c_flags_t::null, c_flags_t::fl_disable_expression );
	__unreferenced_parameter( result );
	__unreferenced_parameter( left_value );
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::parser::preexecute_lor( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const char * & s, const char * end )
{
	if( left_value.as_bool() )
		par.modify_c_flags( c_flags_t::fl_disable_expression );
	__unreferenced_parameter( result );
	__unreferenced_parameter( s );
	__unreferenced_parameter( end );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::postexecute_lor( c_pp::parser & par, pp::variant & result, const pp::variant & left_value )
{
	if( par.get_c_flags( c_flags_t::fl_disable_expression ) )
		par.modify_c_flags( c_flags_t::null, c_flags_t::fl_disable_expression );
	__unreferenced_parameter( left_value );
	__unreferenced_parameter( result );
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TERNARY SELECTION OPERATOR

// Called when all operands are executed
// In the case of condition (left_value) is true, result already contains resulting value.
// Otherwise (when condition is false), right operand contains resulting value.
const status_t f1::c_pp::parser::execute_select( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value )
{
	if( !left_value.as_bool() )
		result = right_value;
	__unreferenced_parameter( par );
	return STATUS_SUCCESS;
}
// Method called before operator's right operand is evaluated
const status_t f1::c_pp::parser::preexecute_select( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const char * & s, const char * end )
{
	// Disable evaluation of middle operand if condition was false
	if( !left_value.as_bool() )
		par.modify_c_flags( c_flags_t::fl_disable_expression );

	// Evaluate middle operand
	pp::variant right_value;
	par.parse_expression( right_value, s, end, ':' );
	__debugbreak_ifnot( ':' == *s )
		++ s;

	// If middle operand was evaluated (since condition was true), return its value
	// and disable evaluation of the right operand
	if( !par.get_c_flags( c_flags_t::fl_disable_expression ) )
	{
		result = right_value;
		par.modify_c_flags( c_flags_t::fl_disable_expression );
	}
	// Otherwise (if condition was false and middle operand was not evaluated),
	// enable evaluation of right operand
	else
		par.modify_c_flags( c_flags_t::null, c_flags_t::fl_disable_expression );

	// Exit
	return STATUS_SUCCESS;
}
// Method called after operator's right operand is evaluated in case preexecute was called
const status_t f1::c_pp::parser::postexecute_select( c_pp::parser & par, pp::variant & result, const pp::variant & left_value )
{
	// If right operand was evaluated (since condition is false), store result
	if( !par.get_c_flags( c_flags_t::fl_disable_expression ) )
		result = left_value;

	// Enable further evaluations
	par.modify_c_flags( c_flags_t::null, c_flags_t::fl_disable_expression );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF parser_expression.cxx*/
