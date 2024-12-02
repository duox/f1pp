/*parser.cxx*/
#include "pch.h"
#include "main.h"

f1::f1pp::parser::parser( const core::Iidentifier * ident ):
	super( ident ),
	m_in_function( false ),
	m_f1pp_flags( null ),
	m_was_enum_ident( false ), m_ignore_enum_ident( false ), m_active_enum_callback( nullptr )
{
}
f1::f1pp::parser::~parser()
{
	_deinitialize_channels();
}

static f1::c_pp::parser::binary_operator_desc_t	m_f1pp_binary_operator_desc[] = {
	{ STR_LEN_PAIR( ".." ),	 2,	f1::f1pp::parser::execute_range },
};

const status_t f1::f1pp::parser::_initialize( f1::parser::Isource * source )
{
	status_t status;

	// Call base class
	if( super::_initialize( source ).failed_status( status ) )
		return status;

	// Initialize object
	get_ns_context().get_root_namespace().set_path_separators( ".", 1 );

	set_esc_char_value( 'z', '0' );
	set_esc_char_callback( 'C', _esc_control );
	set_esc_char_callback( '^', _esc_control );

	add_binary_operators( m_f1pp_binary_operator_desc, countof(m_f1pp_binary_operator_desc) );

	_initialize_channels();

	// Exit
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::f1pp::parser::modify_f1pp_flags( f1pp_flags_t flags_to_set, f1pp_flags_t flags_to_clear, f1pp_flags_t * prev_flags )
{
	if( nullptr != prev_flags )
		*prev_flags = m_f1pp_flags;
	m_f1pp_flags = (m_f1pp_flags &~ flags_to_clear) | flags_to_set;
	return STATUS_SUCCESS;
}
f1::f1pp::parser::f1pp_flags_t f1::f1pp::parser::get_f1pp_flags( f1pp_flags_t mask ) const
{
	return m_f1pp_flags & mask;
}

const status_t f1::f1pp::parser::activate_enum_callback( Ienum_callback & callback )
{
	m_enum_callback_list.push_back( &callback );
	m_active_enum_callback = &callback;

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::deactivate_enum_callback( Ienum_callback & callback )
{
	__debugbreak_if( &callback != m_active_enum_callback )
		return STATUS_ERROR_INVALID_STATE;

	m_enum_callback_list.pop_back();
	m_active_enum_callback = m_enum_callback_list.empty() ? nullptr : m_enum_callback_list.back();

	return STATUS_SUCCESS;
}

f1::f1pp::parser::Ienum_callback::~Ienum_callback()
{
}
const status_t f1::f1pp::parser::Ienum_callback::on_next_item( f1pp::parser & parser )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	return STATUS_ERROR_PURE_CALL;
}
const status_t f1::f1pp::parser::Ienum_callback::append_attributes( f1pp::parser & parser, pp::attribute_list & attrs )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( attrs );
	return STATUS_ERROR_PURE_CALL;
}
const status_t f1::f1pp::parser::Ienum_callback::set_identifier( f1pp::parser & parser, const pp::string & str )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( str );
	return STATUS_ERROR_PURE_CALL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::parser::execute_range( c_pp::parser & par,
	pp::variant & result,
	const pp::variant & left_value, const pp::variant & right_value )
{
	__unreferenced_parameter( par );
	return pp::variant::ty_range == left_value.get_type() ?
		(result = left_value).set_range_end( right_value ):
		result.set_range( left_value, right_value );
}

const status_t f1::f1pp::parser::finish_parse()
{
	super::finish_parse();

	if( !m_delayed_label.empty() )
		message( syntax::IMSG_ERROR_LABEL_NOT_FOUND_S, m_delayed_label.c_str() );

	return STATUS_SUCCESS;
}

bool f1::f1pp::parser::set_newline( bool newline )
{
	if( newline && m_f1pp_flags & f1pp_flags_t::fl_enum_mode )
	{
		if( m_was_enum_ident && nullptr != m_active_enum_callback )
		{
			m_active_enum_callback->on_next_item( *this );
		}
		m_was_enum_ident = m_ignore_enum_ident = false;
	}
	return super::set_newline( newline );
}
bool f1::f1pp::parser::on_char( unsigned ch )
{
	if( m_f1pp_flags & f1pp_flags_t::fl_enum_mode )
	{
		switch( ch )
		{
		case '(': case '{':
			m_enum_parenthesis_stack.push_back( ch );
			break;
		case ')': case '}':
			if( !m_enum_parenthesis_stack.empty() && ch == m_enum_parenthesis_stack.back() )
				m_enum_parenthesis_stack.pop_back();
			break;
		case '[':
			if( !m_was_enum_ident )
			{
				core::Tref<pp::attribute_list> attrs;
				parse_attribute_list( attrs );
				if( nullptr != attrs )
					m_active_enum_callback->append_attributes( *this, *attrs );
				return true;
			}
			break;
		case '=':
			m_ignore_enum_ident = true;
			if( nullptr != m_active_enum_callback )
			{
				flush_chars();
				m_parser->skip_char();
				start_char_seq();
				return true;	//leave only expression in the output
			}
			break;
		case ',':
			if( m_enum_parenthesis_stack.empty() )
			{
				if( !m_was_enum_ident )
					message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
				m_was_enum_ident = m_ignore_enum_ident = false;

				if( nullptr != m_active_enum_callback )
				{
					flush_chars();
					m_parser->skip_char();
					start_char_seq();
					m_active_enum_callback->on_next_item( *this );
					return true;	//leave only expression in the output
				}
			}
			break;
		}
	}
	return super::on_char( ch );
}

int f1::f1pp::parser::get_end_quote( int ch ) const
{
	if( '`' == ch )
		return '`';
	return super::get_end_quote( ch );
}

const status_t f1::f1pp::parser::make_unique_identifier( core::string & buf ) const
{
	f1::guid_t		guid;
	core::string	temp;
	for( unsigned it = 0; ; ++ it )
	{
		// Loop guard check
		__debugbreak_if( it >= 100000 )
			return STATUS_ERROR_INTERNAL;

		// Generate unique string
		guid.random();
		guid.format( temp, f1::guid_t::bfFormatNoHypens | f1::guid_t::bfDisplayNoBraces );
		buf.format( "__f1pp_%s__", temp.c_str() );

		// Try to find macro
		core::Tref<pp::Imacro> mac;
		if( find_macro( mac, pp::string( buf ) ).failed() )
			break;
	}
	return STATUS_SUCCESS;
}

/*END OF parser.cxx*/
