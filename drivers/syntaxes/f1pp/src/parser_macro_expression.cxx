/*parse_macro_expression.cxx*/
/** @file
 *
 * @brief Boolean macro expression parser and evaluator.
 *
 *	The functions in the file parse and evaluate expressions like
 *		M == N && P == (C || D)
 * where identifiers denote macro names. Each macro name in the expression
 * is replaced with a boolean value that specifies whether macro exists or
 * does not. The above expression is evaluated as follows (N and D are not
 * defined):
 *		1 == 0 && 1 == (C || 0)
 * which eventually evaluated to 0 (false) and this value is returned.
 *
 *	The expression parser uses standard priority levels of operators found
 * in C/C++. The ||, && and != boolean operators can be expressed as C/C++
 * bitwise operators |, & and ^. Additionally, the ^^ operator is recognized
 * as a form of syntactic sugar for the != inequality operator. Also, the
 * equality operator (==) can be expressed as the C/C++ assignment operator,
 * which though looks a bit controversally.
 */
#include "pch.h"
#include "main.h"

enum f1::f1pp::parser::boolean_op_t
{
	bop_none	= 0x0001,
	bop_or		= int16_t_make(1, '|'),
	bop_xor		= int16_t_make(2, '^'),
	bop_and		= int16_t_make(3, '&'),
	bop_equ		= int16_t_make(4, '='),
	bop_inequ	= int16_t_make(4, '!'),
};
#define PRIORITY(bop)	int8_t((bop) >> 8)
struct context_desc_t
{
	f1::f1pp::parser::boolean_op_t	m_binary_op1;
	f1::f1pp::parser::boolean_op_t	m_binary_op2;
	bool	m_left_value;
	bool	m_not_prefix;
};
/*const status_t f1::f1pp::parser::parse_macro_expression( bool & result, const char * & s, const char * end_s, unsigned flags )
{
	status_t status;
	bool abort = false;
	boolean_op_t op1 = bop_none;
	boolean_op_t op2 = bop_none;
	bool not_prefix;
	bool left_value = false;
	bool right_value = false;
	core::Tarray<context_desc_t>	context_stack;

	for(;;)
	{
		// Execute operations basing on their priority
		if( bop_none != op1 )
		{
			if( bop_none == op2 || PRIORITY(op1) >= PRIORITY(op2) )
			{
				switch( op1 )
				{
				case bop_none:
					left_value = right_value;
					break;
				case bop_and:
					left_value = left_value && right_value;
					break;
				case bop_or:
					left_value = left_value || right_value;
					break;
				case bop_inequ:
				case bop_xor:
					left_value = left_value != right_value;
					break;
				case bop_equ:
					left_value = left_value == right_value;
					break;
				default:
					DBG_FAIL();
					return STATUS_ERROR_INTERNAL;
				}
			}
			else
			{
				op2 = bop_none;
				if( parse_macro_subexpression( right_value, op2, s, end_s, flags ).failed_status( status ) )
					return status;
				//left_value = right_value;
			}
		}
		else
			left_value = right_value;
		op1 = op2;

		// Check for exit condition
		if( abort )
			break;
		skip_real_spaces_and_comments( s, end_s );
		if( s >= end_s )
			break;

		// Parse unary operator
		not_prefix = false;
		for(; s < end_s; ++ s )
		{
			skip_real_spaces_and_comments( s, end_s );
			if( '!' != *s && '~' != *s )
				break;
			not_prefix = !not_prefix;
		}

		// Parse operand
		if( '(' == *s )
		{
			context_desc_t & desc = *context_stack.rbegin();
			desc.m_binary_op1 = op1;
			desc.m_binary_op2 = op2;
			desc.m_left_value = left_value;
			desc.m_not_prefix = not_prefix;

			not_prefix = false;
			op2 = bop_none;
			++ s;
			continue;
		}
		else if( ')' == *s )
		{
			if( context_stack.empty() )
				return message( syntax::IMSG_ERROR_UNEXPECTED_S, ")" );
			right_value = left_value;

			context_desc_t & desc = *context_stack.rbegin();
			op1			= desc.m_binary_op1;
			op2			= desc.m_binary_op2;
			left_value  = desc.m_left_value;
			not_prefix	= desc.m_not_prefix;

			context_stack.pop_back();
			++ s;
		}
		else if( is_ident_first_char( *s ) )
		{
			pp::string ident;
			parse_macro_identifier( ident, s, end_s );

			core::Tconst_ref<pp::Imacro> mac;
			find_macro( mac, ident );

			if( nullptr != mac )
			{
				right_value = true;
				mac.ref_const_cast()->on_macro_referenced();
			}
			else
				right_value = false;
		}
		else
			return message( syntax::IMSG_ERROR_EXPECTED_OPERAND );
		if( not_prefix )
			right_value = !right_value;

		// Parse binary operator
		skip_real_spaces_and_comments( s, end_s );
		if( s >= end_s )
			continue;

		if( '&' == *s || '|' == *s || '^' == *s || '=' == *s )
		{
			const int ch = *s;
			switch( ch ) {
				case '&': op2 = bop_and;	break;
				case '|': op2 = bop_or;		break;
				case '^': op2 = bop_xor;	break;
				case '=': op2 = bop_equ;	break;
			}
			++ s;
			if( s < end_s && ch == *s )
				++ s;
		}
		else if( s + 1 < end_s && ('!' == s[0] && '=' == s[1]) )
		{
			op2 = bop_inequ;
			s += 2;
		}
		else
		{
			abort = true;
			continue;
			//return message( syntax::IMSG_ERROR_EXPECTED_OPERATOR );
		}
	}

	// Done
	if( !context_stack.empty() )
		return message( syntax::IMSG_ERROR_EXPECTED_S, ")" );
	result = left_value;

	// Exit
	__unreferenced_parameter( flags );
	return STATUS_SUCCESS;
}*/
const status_t f1::f1pp::parser::parse_macro_expression( bool & result, unsigned flags )
{
	const char * s = m_parser->get_ptr();
	status_t status = parse_macro_expression( result, s, m_parser->get_end_ptr(), flags );
	m_parser->set_ptr( s );
	return status;
}

const status_t f1::f1pp::parser::parse_macro_expression(
	bool & res,
	const char * & s, const char * end_s, unsigned flags )
{
	boolean_op_t op = bop_none;
	return parse_macro_subexpression( res, op, s, end_s, flags );
}

const status_t f1::f1pp::parser::parse_macro_subexpression(
	bool & left_operand, boolean_op_t & op,
	const char * & s, const char * end_s, unsigned flags )
{
	status_t status;
	boolean_op_t op_next;
	bool right_operand;

	for(;;)
	{
		// Parse operand
		if( parse_operand( right_operand, s, end_s, flags ).failed_status( status ) )
			break;

		// Parse operation
		parse_operation( op_next, s, end_s, flags );

		// Check for first steps
		if( bop_none == op )
		{
			left_operand = right_operand;
			if( bop_none == op_next )
				break;
			op = op_next;
			continue;
		}

		// Gather all operands of higher priority
		for(;;)
		{
			// Evaluate
			if( bop_none == op_next || PRIORITY(op) >= PRIORITY(op_next) )
			{
				// collapse left_operand/op/right_operand into a value
				evaluate( left_operand, left_operand, op, right_operand, flags ); 
				break;
			}
			else
			{
				// evaluate right operand and get operator following it
				parse_macro_subexpression( right_operand, op_next, s, end_s, flags );
			}
		}
		op = op_next;

		// Abort loop if operation hasn't been parsed
		if( bop_none == op_next )
			break;
	}

	// Exit
	return status;
}
const status_t f1::f1pp::parser::parse_operation( boolean_op_t & result, const char * &s, const char * end_s, unsigned flags )
{
	skip_real_spaces_and_comments( s, end_s );
	if( s >= end_s )
	{
		result = bop_none;
		return STATUS_INFORMATION_NO_MORE_DATA;
	}

	if( '&' == *s || '|' == *s || '^' == *s || '=' == *s )
	{
		const int ch = *s;
		switch( ch ) {
			case '&': result = bop_and;	break;
			case '|': result = bop_or;	break;
			case '^': result = bop_xor;	break;
			case '=': result = bop_equ;	break;
			default:
				DBG_FAIL();
				return STATUS_ERROR_INTERNAL;
		}
		++ s;
		if( s < end_s && ch == *s )
			++ s;
	}
	else if( s + 1 < end_s && ('!' == s[0] && '=' == s[1]) )
	{
		result = bop_inequ;
		s += 2;
	}
	else
		result = bop_none;

	__unreferenced_parameter( flags );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::parse_operand( bool & result, const char * &s, const char * end_s, unsigned flags )
{
	status_t status;

	// Parse unary operator
	bool not_prefix = false;
	for(; s < end_s; ++ s )
	{
		skip_real_spaces_and_comments( s, end_s );
		if( '!' != *s && '~' != *s )
			break;
		not_prefix = !not_prefix;
	}

	// Check whether there's an operand at all
	if( s >= end_s )
	{
		message( syntax::IMSG_ERROR_EXPECTED_OPERAND );
		return STATUS_ERROR;
	}

	// Parse operand value
	if( '(' == *s )
	{
		// TODO: remove recursion
		++ s;
		if( parse_macro_expression( result, s, end_s, flags ).failed_status( status ) )
			return status;

		skip_real_spaces_and_comments( s, end_s );
		if( s >= end_s || ')' != *s )
			return message( syntax::IMSG_ERROR_EXPECTED_S, "')'" );
		++ s;
		return STATUS_SUCCESS;
	}
	/*if( '(' == *s )
	{
		context_desc_t & desc = *context_stack.rbegin();
		desc.m_binary_op1 = op1;
		desc.m_binary_op2 = op2;
		desc.m_left_value = left_value;
		desc.m_not_prefix = not_prefix;

		not_prefix = false;
		op2 = bop_none;
		++ s;
		continue;
	}
	else if( ')' == *s )
	{
		if( context_stack.empty() )
			return message( syntax::IMSG_ERROR_UNEXPECTED_S, ")" );
		right_value = left_value;

		context_desc_t & desc = *context_stack.rbegin();
		op1			= desc.m_binary_op1;
		op2			= desc.m_binary_op2;
		left_value  = desc.m_left_value;
		not_prefix	= desc.m_not_prefix;

		context_stack.pop_back();
		++ s;
	}
	else */if( is_ident_first_char( *s ) )
	{
		pp::string ident;
		parse_macro_identifier( ident, s, end_s );

		core::Tconst_ref<pp::Imacro> mac;
		find_macro( mac, ident, pp::Iparser::find_macro_flags_t::fl_macro_search_all_defined );

		if( nullptr != mac )
		{
			result = true;
			mac.ref_const_cast()->on_macro_referenced();
		}
		else
			result = false;
	}
	else
	{
		message( syntax::IMSG_ERROR_EXPECTED_OPERAND );
		return STATUS_ERROR;
	}

	// Apply prefix
	if( not_prefix )
		result = !result;

	// Exit
	__unreferenced_parameter( flags );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::evaluate( bool & result, bool left_value, boolean_op_t op, bool right_value, unsigned flags )
{
	switch( op )
	{
	case bop_and:
		result = left_value && right_value;
		break;
	case bop_or:
		result = left_value || right_value;
		break;
	case bop_inequ:
	case bop_xor:
		result = left_value != right_value;
		break;
	case bop_equ:
		result = left_value == right_value;
		break;
	case bop_none:
	default:
		DBG_FAIL();
		return STATUS_ERROR_INTERNAL;
	}

	__unreferenced_parameter( flags );
	return STATUS_SUCCESS;
}

/*END OF parse_macro_expression.cxx*/
