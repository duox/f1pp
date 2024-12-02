/*macro.cxx*/
#include "pch.h"
#include "main.h"

f1::c_pp::macro::macro( const core::Iidentifier * ident ) NOEXCEPT:
	super( ident ),
	m_map_has_varags( false )
{
}

const status_t f1::c_pp::macro::initialize( const pp::string & name, const f1::pp::attribute_list * attrs,
	const f1::pp::param_list * params, const pp::string * value, pp::Imacro::macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize( name, attrs, params, value, flags ).failed_status( status ) )
		return status;

	// Initialize macro

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void f1::c_pp::macro::post_change_value( const f1::pp::string & string_value, const f1::pp::variant & variant_value )
{
	m_map.clear();
	return super::post_change_value( string_value, variant_value );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool f1::c_pp::macro::is_stringizing_operator( f1::c_pp::parser & par, const char *& s, const char * end_s ) const
{
	__unreferenced_parameter( par );
	__unreferenced_parameter( s );
	__unreferenced_parameter( end_s );
	return true;	// single '#' is a stringizing operator
}
bool f1::c_pp::macro::is_charizing_operator( f1::c_pp::parser & par, const char *& s, const char * end_s ) const
{
	if( s >= end_s )
		return false;

	if( par.get_charizing_char() != *s )
		return false;

	++ s;
	__unreferenced_parameter( par );
	return true;
}
bool f1::c_pp::macro::is_merge_operator( f1::c_pp::parser & par, const char *& s, const char * end_s ) const
{
	if( s >= end_s )
		return false;

	if( '#' != *s )
		return false;

	++ s;
	__unreferenced_parameter( par );
	return true;
}

/*const status_t f1::c_pp::macro::parse_value( c_pp::parser & parser )
{
	const char * data_start = get_value().data();
	const char * data_end = data_start + get_value().size();
	const char * s = data_start;
	core::string	quote_stack;
	pp::string		ident;

	m_map.clear();

	core::Tref<pp::param_list>	params = get_param_list();
	if( nullptr == params || 0 == params->get_param_list().size() )
		return STATUS_SUCCESS;

	while( s < data_end )
	{
		if( '\"' == *s || '\'' == *s )
		{
			if( quote_stack.empty() || *s !=  *quote_stack.rbegin() )
				quote_stack.push_back( *s );
			else
				quote_stack.pop_back();
		}
		else if( parser.is_ident_first_char( *s ) )
		{
			parser.parse_macro_identifier( ident );
		}
	}

	return STATUS_SUCCESS;
}*/

ptrdiff_t f1::c_pp::macro::add_map_entry( op_id_t id, const char * s, size_t length, signed param_index )
{
	if( 0 == length )
		return -1;

	size_t start = m_map_string.length();
#ifdef _DEBUG
	if( !m_map.empty() )
	{
		const map_entry & prev_entry = *m_map.rbegin();
		__assert( prev_entry.m_offset + prev_entry.m_length == start );
	}
#endif

	// Add map string part
	m_map_string.add_part( s, length );

	// Append new entry
	map_entry * entry = m_map.append_new();
	entry->m_op = uint16_t(id);
	entry->m_offset = uint32_t(start);
	entry->m_length = uint32_t(length);
	entry->m_param_index = int16_t(param_index);

	// Exit
	return m_map.size() - 1;
}
bool f1::c_pp::macro::generate_map( f1::c_pp::parser & par, const pp::arg_list * args ) noexcept
{
	unsigned in_merge;

	// Check current state
	if( nullptr != args )
		on_macro_referenced();	// for func-like macros, it's now called when '(' is parsed

	m_map.clear();

	// Prepare vararg parsing
	bool has_varargs = false;
	const char * vararg_name = nullcstr;
	core::Tconst_ref<pp::param_list> params = get_param_list();
	if( nullptr != params && !params->get_param_list().empty() )
	{
		const pp::param_list::param_desc_t & desc = *params->get_param_list().rbegin();
		if( desc.m_flags & pp::param_list::fl_vararg )
		{
			vararg_name = desc.m_name.empty() ? "__VA_ARGS__" : desc.m_name.c_str();
			has_varargs = nullptr != args && args->get_arg_list().size() >= params->get_param_list().size();
		}
	}

	// Generate list of parameter entries
	core::string parenthesis_stack;
	const char * start = get_value().data();
	const char * s = start;
	const char * end_s = s + get_value().size();
	op_id_t op_id = op_id_t::op_none;
	ptrdiff_t entry;
	ptrdiff_t last_entry = -1;
	const char * range_start = s;
	const char * op_start = nullptr, * op_end = nullptr;
	bool was_was_space, was_space = false;
	while( s < end_s )
	{
		was_was_space = was_space;	// spaces on the previous iteration

		//////////////////////////////////////////////////////////////////////////
		// Spaces of various kinds
		if( '\n' == *s || '\r' == *s )
		{
			if( 0 != get_flags( Imacro::macro_flags_t::fl_keep_new_lines ) )
			{
				++ s;
				continue;
			}
			// fallthrough to `if( isspace() )'
		}
		if( isspace( *s ) )	// including new line characters from the previous step
		{
			if( s != range_start )
			{
				last_entry = add_map_entry( op_id, range_start, s - range_start );
				op_id = op_id_t::op_none;
			}

			if( !was_space )
			{
				add_map_entry( op_id_t::op_space, s, 1 );
				++ s;
			}
			if( isspace( *s ) )
			{
				while( s < end_s && isspace( *s ) )
					++ s;
			}
			range_start = s;
			was_space = true;
			continue;
		}
		if( '/' == *s )
		{
			if( s != range_start )
			{
				last_entry = add_map_entry( op_id, range_start, s - range_start );	// literal chars
				op_id = op_id_t::op_none;
				range_start = s;
			}

			if( !par.parse_comment( s, end_s ) )
				goto none;
			if( !par.get_c_flags( parser::c_flags_t::fl_keep_comments ) )
			{
				if( !was_space )
					add_map_entry( op_id_t::op_space, "\x20", 1 );	// replace comments with single space
				range_start = s;
			}
			was_space = true;
			continue;
		}
		if( '\\' == *s )
		{
			if( s != range_start )
			{
				last_entry = add_map_entry( op_id, range_start, s - range_start );	// literal chars
				op_id = op_id_t::op_none;
			}

			++s;
			if( !par.skip_newline( s, end_s ) )
			{
				//par.message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
				//return false;
				range_start = s - 1;
			}
			else
				range_start = s;
			continue;
		}
		was_space = false;

		if( !parenthesis_stack.empty() )
		{
			// Parse openning bracket
			if( '(' == *s )
				parenthesis_stack.push_back( ')' );
			else if( '[' == *s )
				parenthesis_stack.push_back( ']' );
			// Parse closing bracket
			else if( *parenthesis_stack.rbegin() == *s )
			{
				parenthesis_stack.pop_back();
				if( parenthesis_stack.empty() )
				{
					if( range_start != s )
					{
						add_map_entry( op_id, range_start, s - range_start );
						op_id = op_id_t::op_none;
					}
					++ s;
					range_start = s;
					continue;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Identifiers
		if( par.is_ident_first_char( *s ) )
		{
			const char * ident_start = s;
			const char * range_end = s;

			pp::string ident;
			if( !par.parse_macro_identifier( ident, s, end_s ) )
			{
				if( op_id_t::op_merge_right & op_id )
				{
					par.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
					return false;
				}
				return true;
			}
			const char * ident_end = s;

			signed index;
			if( nullcstr != vararg_name && ident.length() == strlen( vararg_name ) && !memcmp( ident.data(), vararg_name, strlen( vararg_name ) ) )
			{
				// Check macro availability in the current standard version
				if( !par.m_VA_ARGS_supported )
				{
					const pp::Istandard * standard = pp::service::get().find_item_standard(
						Imacro::m_item_class_guid, pp::string( "__VA_ARGS__" ) );
					par.message( syntax::IMSG_WARNING_NOT_AVAILABLE_STANDARD_SS, "__VA_ARGS__",
						nullptr == standard ? "C99" : standard->get_description_string()
						);
					par.m_VA_ARGS_supported = true;	// avoid superfluous messages
				}

				index = IDX_VA_ARGS;
			}
			else if( ident.length() == __small_strlen( "__VA_OPT__" ) && !__small_memcmp( ident.data(), "__VA_OPT__", __small_strlen( "__VA_OPT__" ) ) )
			{
				// Check macro availability in the current standard version
				if( !par.m_VA_OPT_supported )
				{
					const pp::Istandard * standard = pp::service::get().find_item_standard(
						Imacro::m_item_class_guid, pp::string( "__VA_OPT__" ) );
					par.message( syntax::IMSG_WARNING_NOT_AVAILABLE_STANDARD_SS, "__VA_OPT__",
						nullptr == standard ? "C++20" : standard->get_description_string()
						);
					par.m_VA_OPT_supported = true;	// avoid superfluous messages
				}

				// Parse openning '('
				par.skip_real_spaces_and_comments( s, end_s );
				if( '(' != *s )
				{
					par.message( syntax::IMSG_ERROR_EXPECTED_ARG_LIST );
					return false;
				}

				// Fetch everything that's inside of ( and )
				if( !has_varargs )
				{
					ident_start = ++ s;
					par.parse_delimited_string( s, end_s, ')' );
					ident_end = s ++;
				}
				else
				{
					parenthesis_stack.push_back( ')' );
					++ s;
					range_start = s;
					continue;
				}

				index = IDX_VA_OPT;
			}
			else
			{
				if( nullptr == params )
					index = IDX_NOTHING;
				else
				{
					index = params->find_param( ident );
					if( has_varargs && nullptr != args && size_t(index) == args->get_arg_list().size() )
						index = IDX_VA_ARGS;
				}
				if( IDX_NOTHING == index )
				{
					if( (op_id_t::op_stringify | op_id_t::op_charify) & op_id )
					{
						par.message( syntax::IMSG_ERROR_EXPECTED_PARAMETER_S, ident.c_str() );
						//range_start = s;
						//continue;
					}
				}
			}

//			__assert( op_id != op_id_t::op_none || range_end == range_start );	// TODO: check what's up here
//					add_map_entry( op_id_t::op_none, range_start, range_end - range_start );	// literal chars
			if( range_end != range_start )
			{
				add_map_entry( op_id, range_start, range_end - range_start );				// literal chars
				op_id = op_id_t::op_none;
			}
			entry = add_map_entry( op_id, ident_start, ident_end - ident_start, index );	// macro parameter

			if( op_id_t::op_none != op_id )
				op_id = op_id_t::op_none;
			last_entry = entry;
			range_start = s;
			continue;
		}
		if( (op_id_t::op_stringify | op_id_t::op_charify) & op_id )
		{
			par.message( syntax::IMSG_WARNING_EXPECTED_IDENTIFIER );
			last_entry = add_map_entry( op_id_t::op_none, op_start, op_end - op_start );
			op_id &= ~(op_id_t::op_stringify | op_id_t::op_charify);
		}

		//////////////////////////////////////////////////////////////////////////
		// Operators
		if( '#' == *s )
		{
			if( s != range_start )
			{
				if( 1 != s - range_start || !isspace(*range_start) )
					last_entry = add_map_entry( op_id_t::op_none, range_start, s - range_start );	// write previous chars
			}

			op_start = s;
			++ s;			// skip '#'

			if( is_merge_operator( par, s, end_s ) )	// a '##' merge operator?
			{
				if( -1 == last_entry )
				{
					par.message( syntax::IMSG_ERROR_OPERATOR_EXPECTS_MACRO_ARG );// error, ident at left required
					return false;
				}
				//int id = m_map[last_entry].m_op;	// TODO: remove debug helper 'id'
				__assert( 0 == (op_id_t::op_merge_left & m_map[last_entry].m_op) || op_id_t::op_none == m_map[last_entry].m_op );
				m_map[last_entry].m_op |= uint16_t(op_id_t::op_merge_left);
				//id = m_map[last_entry].m_op;		// TODO: remove debug helper 'id'
				last_entry = -1;
				op_id |= op_id_t::op_merge_right;
			}
			else if( is_charizing_operator( par, s, end_s ) )	// a '#@' charizing operator?
			{
				if( 0 != ((op_id_t::op_stringify | op_id_t::op_charify) & op_id) )
					par.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
				else
					op_id |= op_id_t::op_charify;
			}
			else if( is_stringizing_operator( par, s, end_s ) )	// a '#' stringizing operator?
			{
				if( 0 != ((op_id_t::op_stringify | op_id_t::op_charify) & op_id) )
					par.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
				else
					op_id |= op_id_t::op_stringify;
			}
			else
				goto none;

			op_end = s;
			par.skip_real_spaces_and_comments( s, end_s );
			range_start = s;

			continue;
		}

		//////////////////////////////////////////////////////////////////////////
		// All chars that form single map entry
		if( '\"' == *s || '\'' == *s )
		{
			//last_entry = -1;
			par.skip_string( s, end_s, nullptr, parser::string_parse_flags_t::fl_parse_string_ignore_unterm );
		}
		else
		{
	none:
			//last_entry = -1;
			++ s;
		}
	}

	// Finish parser state
	if( op_id & (op_id_t::op_charify | op_id_t::op_stringify) )
	{
		par.message( syntax::IMSG_WARNING_EXPECTED_IDENTIFIER );
		op_id &= ~(op_id_t::op_stringify | op_id_t::op_charify);
		last_entry = add_map_entry( op_id, op_start, op_end - op_start );
	}
	else
		add_map_entry( op_id, range_start, s - range_start );

	// Parse closing ')'
	if( !parenthesis_stack.empty() )
	{
		char s[2];
		s[0] = char( *parenthesis_stack.rbegin() );
		s[1] = nullchar;
		par.message( syntax::IMSG_ERROR_EXPECTED_S, s );
		return false;
	}

	// Process list of elements, removing empty elements if needed
	map_entry * en_prev = nullptr;
	start = m_map_string.data();
	end_s = start + m_map_string.size();
	core::Tarray<map_entry> dst_map;
	dst_map.reserve( m_map.size() );
	in_merge = 0;
	array_foreach( core::Tarray<map_entry>::iterator, it, m_map )
	{
		map_entry & en = *it;
		//__assert( nullptr == en_prev || en_prev->m_offset + en_prev->m_length == en.m_offset );

		// Depending on the operation, trim spaces from the argument
		if( op_id_t::op_none == en.m_op )
		{
			__assert( 0 == in_merge );
			if( en.m_length > 1 )
			{
				size_t off;
				for( off = 0; off < en.m_length && isspace( start[en.m_offset + off] ); ++ off )
					;
				if( off == en.m_length )
					en.m_length = 0;
			}
		}
		if( en.m_op & (op_id_t::op_charify | op_id_t::op_stringify) ) {	// trim spaces from left, right and inside
			while( en.m_length > 0 && isspace( start[en.m_offset] ) )			// trim from left
				++ en.m_offset,
				-- en.m_length;
			while( en.m_length > 0 && isspace( start[en.m_offset + en.m_length - 1] ) )	// trim from right
				-- en.m_length;
			// TODO: trim inside
		}
		if( en.m_op & op_id_t::op_merge_left ) {		// trim spaces from right
			while( en.m_length > 0 && isspace( start[en.m_offset + en.m_length - 1] ) )
				-- en.m_length;
			++ in_merge;
		}
		if( en.m_op & op_id_t::op_merge_right ) {		// trim spaces from left
			__assert( 0 != in_merge );
			while( en.m_length > 0 && isspace( start[en.m_offset] ) )
				++ en.m_offset,
				-- en.m_length;
			-- in_merge;
		}
		if( en.m_op == op_id_t::op_space && in_merge )
			en.m_length = 0;

		if( 0 != en.m_length )
		{
			// Merge similar map entries
			if( nullptr != en_prev && en_prev->m_op == en.m_op && en_prev->m_param_index == en.m_param_index )
			{
				en_prev->m_length += en.m_length;
			}
			else
			{
				dst_map.append( en );
				en_prev = &dst_map.back();
			}

			// Display warning if entry is operand of two operators
			if( op_id_t::op_merge_left  != (en.m_op & (op_id_t::op_merge_left |op_id_t::op_charify|op_id_t::op_stringify)) ||
				op_id_t::op_merge_right != (en.m_op & (op_id_t::op_merge_right|op_id_t::op_charify|op_id_t::op_stringify)) )
			{
				// TODO
//				par.message( syntax::IMSG_WARNING_OPERATION_ORDER_UNDEFINED_S,
//					pp::string( start + en.m_offset, en.m_length ).c_str()
//					);
			}
		}
	}
	__assert( 0 == in_merge );
	m_map.move( dst_map );
	m_map_has_varags = has_varargs;

	// Exit
	return true;
}

const status_t f1::c_pp::macro::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	status_t	status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	core::Tconst_ref<pp::param_list> params = get_param_list();
	if( nullptr != params )
	{
		__debugbreak_if( nullptr == args )
			return STATUS_ERROR_INTERNAL;
	}

	// Prepare vararg parsing
	bool has_varargs = false;
	const char * vararg_name = nullcstr;
	if( nullptr != params && !params->get_param_list().empty() )
	{
		const pp::param_list::param_desc_t & desc = *params->get_param_list().rbegin();
		if( desc.m_flags & pp::param_list::fl_vararg )
		{
			vararg_name = desc.m_name.empty() ? "__VA_ARGS__" : desc.m_name.c_str();
			has_varargs = nullptr != args && args->get_arg_list().size() >= params->get_param_list().size();
		}
	}

	// If map hasn't been generated yet, generate it now
	if( m_map.empty() || m_map_has_varags != has_varargs )
	{
		if( !const_cast<self *>(this)->generate_map( *c_pp_par, args ) )	// generate parameter location
			return STATUS_SUCCESS;	// could not generate map
	}

	// Fast exit for simple macros
	if( 1 == m_map.size() && IDX_NOTHING == (*m_map.begin()).m_param_index )
	{
		const map_entry & entry = *m_map.begin();
		out_str( *c_pp_par, buf, entry, m_map_string.data() + entry.m_offset, entry.m_length );
		parser.on_macro_expanded( *const_cast<self *>(this) );
		return STATUS_SUCCESS;
	}

	// Check argument list and get vararg if present
	const size_t param_count =
		(nullptr != params && !params->get_param_list().empty()) ? params->get_param_list().size() : 0;
	const bool vararg_present =
		param_count > 0 &&
		(*(params->get_param_list().rbegin())).m_flags & pp::param_list::fl_vararg;
	const size_t named_param_count = vararg_present ? param_count - 1 : param_count;

	size_t arg_count = 0;
	if( nullptr != args )
	{
		if( !args->get_arg_list().empty() )
			arg_count = args->get_arg_list().size();
		else if( 1 == param_count )
			//arg_count = 1;	// empty parameter ()
			arg_count = 0;
	}

	if( 0 == param_count && 1 == arg_count )
	{
		const pp::arg_list::arg_desc_t & arg = *args->get_arg_list().begin();
		if( arg.m_value.is_empty() && arg.m_expanded_value.is_empty() )
			arg_count = 0;
	}

	if( !vararg_present && arg_count > named_param_count )
		return c_pp_par->message( syntax::IMSG_WARNING_TOO_MANY_ARGUMENTS_S, get_name() );
//	if( arg_count < named_param_count )
//		return c_pp_par->message( syntax::IMSG_ERROR_INSUFFICIENT_MACRO_ARGUMENTS );

	pp::string va_args_expanded;
	pp::string va_args_unexpanded;
	if( vararg_present && arg_count > named_param_count )
	{
		// Build vararg string
		pp::arg_list::arg_desc_list_t::const_iterator arg_it(args->get_arg_list());
		//if( !arg_it.go_to_index( args->get_arg_list(), named_param_count ) )
		//if( arg_it.set_position( named_param_count ).failed_status( status ) )
		if( arg_it.reset( args->get_arg_list(), named_param_count ).failed_status( status ) )
			return status;
		for(;;)
		{
			const pp::arg_list::arg_desc_t & arg = *arg_it;

			//va_args.add_part( arg.m_expanded_value.is_empty() ? arg.m_value : arg.m_expanded_value );
			va_args_expanded.add_part( arg.m_expanded_value );
			va_args_unexpanded.add_part( arg.m_value );

			if( !arg_it.next() )
				break;
			va_args_expanded.add_part( "," );
			va_args_unexpanded.add_part( "," );
		}
	}

	// Expand value
	const char * start = m_map_string.data();
	const char * s = start;
	const char * end_s = s + m_map_string.size();
	core::Tarray<map_entry>::const_iterator map_it(m_map);
	pp::string	merge_result;
	bool		in_merge = false;
	const map_entry *	prev_entry = nullptr;
	while( s < end_s )
	{
		// Move to next entry
		if( !map_it.valid() )	// no more args - add the rest and exit the loop
		{
			if( in_merge )
				c_pp_par->message( syntax::IMSG_ERROR_OPERATOR_EXPECTS_MACRO_ARG );	// error, ident at right required
			break;
		}
		const map_entry & entry = *map_it;

		// Access to entry's argument data
		pp::string temp;
		const pp::string * arg_str;
		switch( entry.m_param_index )
		{
		case IDX_NOTHING:
			arg_str = nullptr;
			break;
		case IDX_VA_ARGS:
			if( entry.m_op & ( op_id_t::op_merge_left | op_id_t::op_merge_right | op_id_t::op_stringify | op_id_t::op_charify ) )
				arg_str = &va_args_unexpanded;
			else
				arg_str = &va_args_expanded;	//&va_args_unexpanded;
			break;
		case IDX_VA_OPT:
			if( va_args_unexpanded.is_empty() )
				arg_str = &va_args_unexpanded;		// insert empty string
			else
				arg_str = nullptr;		// insert given character sequence
			break;
		default:
			__debugbreak_if( nullptr == args )
				return STATUS_ERROR_INTERNAL;
			const size_t arg_index = IDX_VA_OPT == entry.m_param_index ? 0 : entry.m_param_index;

			arg_str = args->get_arg_expanded_value( *params, arg_index );
			if( /*nullptr == arg_str || arg_str->is_empty() || */
				(entry.m_op & ( op_id_t::op_merge_left | op_id_t::op_merge_right | op_id_t::op_stringify | op_id_t::op_charify ) ) )
			{
				arg_str = args->get_arg_value( *params, arg_index );
			}

//			__debugbreak_if( nullptr == arg_str )
//				return STATUS_ERROR_INTERNAL;
			if( nullptr == arg_str )
				arg_str = &pp::string::null;

			break;
		}
		int op = entry.m_op;

		// Get access to entry data
		size_t length;
		const char * string;
		if( nullptr != arg_str )
		{
			string = arg_str->data();
			length = arg_str->length();
		}
		else
		{
			string = start + entry.m_offset;
			length = entry.m_length;
		}

		// Finish previous merge operation
		if( op & op_id_t::op_merge_right ) {	// write second part
			__assert( in_merge );
			in_merge = false;

			// Check for GCC's 'abcd,##__VA_ARGS__' construct
			if( IDX_VA_ARGS == entry.m_param_index &&
				!merge_result.empty() &&
				va_args_unexpanded.is_empty() )
			{
				// Simply delete last operand
				if( merge_result.data()[merge_result.length() - 1] == ',' )
					merge_result.erase_last();
			}
			else
			{
				// Check for GCC's '__VA_ARGS__##,abcd' construct
				if( nullptr != prev_entry &&
					IDX_VA_ARGS == prev_entry->m_param_index &&
					0 != length &&
					va_args_unexpanded.is_empty() )
				{
					if( string[0] == ',' )
					{
						++ string;
						-- length;
					}
				}

				// Insert string
				pp::service::trim_spaces( string, length );
				out_str( *c_pp_par, merge_result, entry, string, length );
			}

			// Done
			if( c_pp_par->on_merge_operator( *this, merge_result ).failed_status( status ) )
				return status;
			buf.add_part_move( merge_result );
		}

		// Initiate new merge operation
		if( entry.m_op & op_id_t::op_merge_left )	// write first part
		{
			if( 0 == (entry.m_op & op_id_t::op_merge_right) )
			{
				pp::service::trim_spaces( string, length );
				out_str( *c_pp_par, merge_result, entry, string, length );
			}
			in_merge = true;
		}

		// Output simple string if no merge operation is issued
		if( op_id_t::op_none  == (entry.m_op & ~(op_id_t::op_stringify | op_id_t::op_charify)) ||
			op_id_t::op_space == (entry.m_op & ~(op_id_t::op_stringify | op_id_t::op_charify)) )//(entry.m_op & ~(op_id_t::op_merge_left | op_id_t::op_merge_right)) )
		{
			if( op_id_t::op_space != entry.m_op || !in_merge )
				out_str( *c_pp_par, in_merge ? merge_result : buf, entry, string, length );
		}

		// Iterate
		s += entry.m_length;		// add length of replaced parameter
		prev_entry = &entry;
		map_it.next();
	}

	parser.on_macro_expanded( *const_cast<self *>(this) );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::macro::out_str( f1::c_pp::parser & c_pp_par, pp::string & buf, const map_entry & entry, const char * string, size_t length ) const
{
	if( op_id_t::op_none == (entry.m_op & ~(op_id_t::op_merge_left | op_id_t::op_merge_right)) || op_id_t::op_space & entry.m_op )
	{
		buf.add_part( string, length );
	}
	else if( entry.m_op & op_id_t::op_stringify )
	{
		//if( IDX_NOTHING == entry.m_param_index )
		//	return c_pp_par.message( syntax::IMSG_ERROR_OPERATOR_EXPECTS_MACRO_ARG );

		pp::string res;
		pp::service::compact_spaces( res, string, length );
		c_pp_par.stringify_chars( buf, res, '\"' );
	}
	else if( entry.m_op & op_id_t::op_charify )
	{
		//if( IDX_NOTHING == entry.m_param_index )
		//	return c_pp_par.message( syntax::IMSG_ERROR_OPERATOR_EXPECTS_MACRO_ARG );

		pp::string res;
		pp::service::compact_spaces( res, string, length );
		c_pp_par.stringify_chars( res, pp::string( string, length ), '\'' );
	}
	return STATUS_SUCCESS;
}

/*END OF macro.cxx*/
