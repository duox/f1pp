/*variant.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::variant	f1::pp::variant::null;
f1::pp::variant	f1::pp::variant::invalid;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const status_t f1::pp::variant::set_type( type_t ty ) noexcept
{
	if( ty == m_type )
		return STATUS_SUCCESS;
	__debugbreak_if( ty_range == ty )
		return STATUS_ERROR_NOT_SUPPORTED;

	if( ty_string == m_type || ty_text == m_type )
		free( (void *) m_string.m_data );
	else if( ty_list == m_type )
		_release_list();
	_reset_cache();

	if( ty_invalid != ty && ty_none != ty )
	{
		__debugbreak_if( unsigned(ty) >= ty_count )
			return STATUS_ERROR_INVALID_PARAMETER;

		switch( m_type ) {
		case ty_none:
			switch( ty ) {
				case ty_bool:	m_bool = false;	break;
				case ty_int:	m_int = 0LL;	break;
				case ty_uint:	m_uint = 0ULL;	break;
				case ty_float:	m_float = 0.0f;	break;
				case ty_list:	m_list.m_data = nullptr;	break;
				case ty_string:
				case ty_text:	m_string.m_data = nullptr;	break;
				default:	DBG_FAIL();	return STATUS_ERROR_INVALID_TYPE;
			}
			break;
		case ty_bool:
			switch( ty ) {
				case ty_int:	m_int = m_bool;		break;
				case ty_uint:	m_uint = m_bool;	break;
				case ty_float:	m_float = m_bool;	break;
				default:	DBG_FAIL();	return STATUS_ERROR_INVALID_TYPE;
			}
			break;
		case ty_int:
			switch( ty ) {
				case ty_bool:	m_bool = 0 != m_int;	break;
				case ty_float:	m_float = max_float_t(m_int);	break;
				case ty_uint:	break;
				default:	DBG_FAIL();	return STATUS_ERROR_INVALID_TYPE;
			}
			break;
		case ty_uint:
			switch( ty ) {
				case ty_bool:	m_bool = 0 != m_uint;	break;
				case ty_float:	m_float = max_float_t(m_uint);	break;
				case ty_int:	break;
				default:	DBG_FAIL();	return STATUS_ERROR_INVALID_TYPE;
			}
			break;
		case ty_float:
			switch( ty ) {
				case ty_bool:	m_bool = 0 != m_float;	break;
				case ty_int:	m_int = intmax_t(m_float);	break;
				case ty_uint:	m_int = uintmax_t(m_float);	break;
				default:	DBG_FAIL();	return STATUS_ERROR_INVALID_TYPE;
			}
			break;
		case ty_text:
		case ty_string:
			m_string.m_data = nullptr;
			break;
		case ty_list:
			m_list.m_data = nullptr;
			break;
		default:
			// ignore
			break;
		}
	}
	m_type = type_id_t(ty);

	return STATUS_SUCCESS;
}

bool f1::pp::variant::can_cast_to( type_t ty ) const noexcept
{
	if( ty_none == m_type || ty_none == ty )
		return false;	// null type is incompatible to all types including self
	if( m_type == ty )
		return true;

	if( ty_range == m_type || ty_range == ty )
		return false;	// if one type is range and another is not, return false
	if( (ty_string == m_type && ty_text == ty) || (ty_text == m_type && ty_string == ty) )
		return true;
	if( ty_text == m_type || ty_string == m_type )
		return false;	// strings can only be cast to string

	return true;		// numeric and boolean types are mutual castable
}

const status_t f1::pp::variant::set_range( const variant & start, const variant & end, const variant & step ) noexcept
{
	// Select common type
	type_t common_type;
	if( ty_float == start.m_type || ty_float == end.m_type || ty_float == step.m_type )
		common_type = ty_float;
	else if( ty_int == start.m_type || ty_int == end.m_type || ty_int == step.m_type )
		common_type = ty_int;
	else if( ty_uint == start.m_type || ty_uint == end.m_type || ty_uint == step.m_type )
		common_type = ty_uint;
	else
		__debugbreak_return( STATUS_ERROR_INVALID_TYPE );

	// Store type
	switch( common_type )
	{
	case ty_float:
		m_range.m_min_bound.m_float = start.as_float();
		m_range.m_max_bound.m_float = end.as_float();
		m_range.m_step.m_float = step.as_float();
		break;
	case ty_int:
		m_range.m_min_bound.m_int = start.as_int();
		m_range.m_max_bound.m_int = end.as_int();
		m_range.m_step.m_int = step.as_int();
		break;
	case ty_uint:
		m_range.m_min_bound.m_uint = start.as_uint();
		m_range.m_max_bound.m_uint = end.as_uint();
		m_range.m_step.m_uint = step.as_uint();
		break;
	default:
		__assert( !"invalid variant range type" );
		__assume(false);
		//return STATUS_ERROR_INTERNAL;
	}

	// Done
	m_type = ty_range;
	m_range.m_type = type_id_t(common_type);

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::pp::variant::set_range( const variant & start, const variant & end ) noexcept
{
	// Select common type
	type_t common_type;
	if( ty_float == start.m_type || ty_float == end.m_type )
		common_type = ty_float;
	else if( ty_int == start.m_type || ty_int == end.m_type )
		common_type = ty_int;
	else if( ty_uint == start.m_type || ty_uint == end.m_type )
		common_type = ty_uint;
	else if( ty_bool == start.m_type || ty_bool == end.m_type )
		common_type = ty_bool;
	else
		__debugbreak_return( STATUS_ERROR_INVALID_TYPE );

	// Store type
	switch( common_type )
	{
	case ty_float:
		m_range.m_min_bound.m_float = start.as_float();
		m_range.m_max_bound.m_float = end.as_float();
		m_range.m_step.m_float = 1.0f;
		break;
	case ty_int:
		m_range.m_min_bound.m_int = start.as_int();
		m_range.m_max_bound.m_int = end.as_int();
		m_range.m_step.m_int = 1;
		break;
	case ty_uint:
		m_range.m_min_bound.m_uint = start.as_uint();
		m_range.m_max_bound.m_uint = end.as_uint();
		m_range.m_step.m_uint = 1;
		break;
	/*case ty_bool:
		m_range.m_min_bound.m_bool = start.as_bool();
		m_range.m_max_bound.m_bool = end.as_bool();
		m_range.m_step.m_bool = 1;
		break;*/
	default:
		__assert( !"invalid variant range type" );
		__assume(false);
		//return STATUS_ERROR_INTERNAL;
	}

	// Done
	m_type = ty_range;
	m_range.m_type = type_id_t(common_type);

	// Exit
	return STATUS_SUCCESS;
}
//inline
const status_t f1::pp::variant::set_range_end( const variant & end ) noexcept
{
	__debugbreak_if( ty_range != m_type )
		return STATUS_ERROR_INVALID_STATE;

	m_range.m_step = m_range.m_max_bound;
	switch( m_range.m_type )
	{
	case ty_float:
		m_range.m_max_bound.m_float = end.as_float();
		break;
	case ty_int:
		m_range.m_max_bound.m_int = end.as_int();
		break;
	case ty_uint:
		m_range.m_max_bound.m_uint = end.as_uint();
		break;
	/*case ty_bool:
		m_range.m_max_bound.m_bool = end.as_bool();
		break;*/
	default:
		__assert( !"invalid variant range type" );
		__assume(false);
		//return STATUS_ERROR_INTERNAL;
	}

	return STATUS_SUCCESS;
}
const status_t f1::pp::variant::set_range( const max_float_t start, const max_float_t end, max_float_t step ) noexcept
{
	m_range.m_min_bound.m_float = start;
	m_range.m_max_bound.m_float = end;
	m_range.m_step.m_float = step;
	m_range.m_type = ty_float;
	m_type = ty_range;

	return STATUS_SUCCESS;
}
const status_t f1::pp::variant::set_range( const intmax_t start, const intmax_t end, intmax_t step ) noexcept
{
	m_range.m_min_bound.m_int = start;
	m_range.m_max_bound.m_int = end;
	m_range.m_step.m_int = step;
	m_range.m_type = ty_int;
	m_type = ty_range;

	return STATUS_SUCCESS;
}
const status_t f1::pp::variant::set_range( const uintmax_t start, const uintmax_t end, uintmax_t step ) noexcept
{
	m_range.m_min_bound.m_uint = start;
	m_range.m_max_bound.m_uint = end;
	m_range.m_step.m_uint = step;
	m_range.m_type = ty_uint;
	m_type = ty_range;

	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::pp::variant::format( pp::string & buf ) const noexcept
{
	char str[256];
	char * ptr;
	size_t len;

	switch( m_type )
	{
	case ty_bool:
		buf.add_part( m_bool ? "true" : "false" );
		break;
	case ty_int:
		ptr = _imaxtostr( str, _countof(str), m_int, 10 );
		__assert( ptr >= str );
		buf.add_part_alloc( ptr, _countof(str) - (ptr - str) - 1 );
		break;
	case ty_uint:
		ptr = _umaxtostr( str, _countof(str), m_uint, 10 );
		__assert( ptr >= str );
		buf.add_part_alloc( ptr, _countof(str) - (ptr - str) );
		break;
	case ty_float:
		len = _snprintf( str, _countof(str), "%Lf", m_float );
		buf.add_part_alloc( str, len );
		break;
	case ty_string:
		buf.add_part( "\"", 1 );
		buf.add_part( m_string.m_data );
		buf.add_part( "\"", 1 );
		break;
	case ty_text:
		buf.add_part( m_string.m_data );
		break;
	case ty_list:
		for( size_t i = 0; ; )
		{
			pp::string temp;
			m_list.m_data[i].format( temp );
			buf.add_part( temp );
			++ i;
			if( i >= m_list.m_count )
				break;
			buf.add_part( "," );
		}
		break;
	case ty_range:
		switch( m_range.m_type )
		{
		case ty_float:
			if( 0 == m_range.m_step.m_float )
			{
				len = m_range.m_min_bound.m_float < m_range.m_max_bound.m_float ?
					_snprintf( str, _countof(str), "%f..%f", (double) m_range.m_min_bound.m_float, (double) m_range.m_max_bound.m_float ):
					_snprintf( str, _countof(str), "%f..%f", (double) m_range.m_max_bound.m_float, (double) m_range.m_min_bound.m_float );
			}
			else
			{
				len = m_range.m_min_bound.m_float < m_range.m_max_bound.m_float ?
					_snprintf( str, _countof(str), "%f..%f..%f", (double) m_range.m_min_bound.m_float, (double) m_range.m_step.m_float, (double) m_range.m_max_bound.m_float ):
					_snprintf( str, _countof(str), "%f..%f..%f", (double) m_range.m_max_bound.m_float, (double) m_range.m_step.m_float, (double) m_range.m_min_bound.m_float );
			}
			break;
		case ty_int:
			if( 0 == m_range.m_step.m_int )
			{
				len = m_range.m_min_bound.m_int < m_range.m_max_bound.m_int ?
					_snprintf( str, _countof(str), "%Id..%Id", (ptrdiff_t) m_range.m_min_bound.m_int, (ptrdiff_t) m_range.m_max_bound.m_int ):
					_snprintf( str, _countof(str), "%Id..%Id", (ptrdiff_t) m_range.m_max_bound.m_int, (ptrdiff_t) m_range.m_min_bound.m_int );
			}
			else
			{
				len = m_range.m_min_bound.m_int < m_range.m_max_bound.m_int ?
					_snprintf( str, _countof(str), "%Id..%Id..%Id", (ptrdiff_t) m_range.m_min_bound.m_int, (ptrdiff_t) m_range.m_step.m_int, (ptrdiff_t) m_range.m_max_bound.m_int ):
					_snprintf( str, _countof(str), "%Id..%Id..%Id", (ptrdiff_t) m_range.m_max_bound.m_int, (ptrdiff_t) m_range.m_step.m_int, (ptrdiff_t) m_range.m_min_bound.m_int );
			}
			break;
		case ty_uint:
			if( 0 == m_range.m_step.m_uint )
			{
				len = m_range.m_min_bound.m_uint < m_range.m_max_bound.m_uint ?
					_snprintf( str, _countof(str), "%Iu..%Iu", (size_t) m_range.m_min_bound.m_uint, (size_t) m_range.m_max_bound.m_uint ):
					_snprintf( str, _countof(str), "%Iu..%Iu", (size_t) m_range.m_max_bound.m_uint, (size_t) m_range.m_min_bound.m_uint );
			}
			else
			{
				len = m_range.m_min_bound.m_uint < m_range.m_max_bound.m_uint ?
					_snprintf( str, _countof(str), "%Iu..%Iu..%Iu", (size_t) m_range.m_min_bound.m_uint, (size_t) m_range.m_step.m_uint, (size_t) m_range.m_max_bound.m_uint ):
					_snprintf( str, _countof(str), "%Iu..%Iu..%Iu", (size_t) m_range.m_max_bound.m_uint, (size_t) m_range.m_step.m_uint, (size_t) m_range.m_min_bound.m_uint );
			}
			break;
		default:
			__assert( !"invalid variant range type" );
			__assume(false);
			//return STATUS_ERROR_INTERNAL;
		}
		break;
	default:
		DBG_FAIL();
		buf.clear();
		return STATUS_ERROR_INVALID_TYPE;
	}
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool f1::pp::variant::iterator::next()
{
	if( nullptr == m_variant )
		return false;	// detached iterator
	switch( m_variant->m_type )
	{
	case ty_list:
		if( size_t(-1) == m_index )
		{
			m_index = 0;
			return 0 != m_variant->m_list.m_count;
		}
		if( m_index >= m_variant->m_list.m_count )
			return false;
		++ m_index;
		break;
	case ty_string:
	case ty_text:
		if( size_t(-1) == m_index )
		{
			m_index = 0;
			return 0 != m_variant->m_string.m_length;
		}
		++ m_index;
		if( m_index >= m_variant->m_string.m_length )
			return false;
		break;
	case ty_range:
		switch( m_variant->m_range.m_type )
		{
		case ty_float:{
			max_float_t res = m_float + m_variant->m_range.m_step.m_float;
			if( m_variant->m_range.m_max_bound.m_float < res )
				return false;
			if( res < m_variant->m_range.m_min_bound.m_float )
				res = m_variant->m_range.m_min_bound.m_float;
			m_float = res;
			}break;
		case ty_int:{
			intmax_t res = m_int + m_variant->m_range.m_step.m_int;
			if( m_variant->m_range.m_max_bound.m_int < res )
				return false;
			if( res < m_variant->m_range.m_min_bound.m_int )
				res = m_variant->m_range.m_min_bound.m_int;
			m_int = res;
			}break;
		case ty_uint:{
			uintmax_t res = m_uint + m_variant->m_range.m_step.m_uint;
			if( m_variant->m_range.m_max_bound.m_uint < res )
				return false;
			if( res < m_variant->m_range.m_min_bound.m_uint )
				res = m_variant->m_range.m_min_bound.m_uint;
			m_uint = res;
			}break;
		default:
			__assert( !"invalid variant range type" );
			return false;
		}
		break;
	default:
		if( size_t(-1) == m_index )
		{
			m_index = 0;
			break;
		}
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool f1::pp::variant::reverse_iterator::next()
{
	if( nullptr == m_variant )
		return false;	// detached iterator
	switch( m_variant->m_type )
	{
	case ty_list:
	case ty_string:
	case ty_text:
		if( 0 == m_index )
			return false;
		-- m_index;
		break;
	case ty_range:
		switch( m_variant->m_range.m_type )
		{
		case ty_float:{
			max_float_t res = m_float - m_variant->m_range.m_step.m_float;
			if( res < m_variant->m_range.m_min_bound.m_float || m_variant->m_range.m_max_bound.m_float < res )
				return false;
			m_float = res;
					  }break;
		case ty_int:{
			intmax_t res = m_int - m_variant->m_range.m_step.m_int;
			if( res < m_variant->m_range.m_min_bound.m_int || m_variant->m_range.m_max_bound.m_int < res )
				return false;
			m_int = res;
					}break;
		case ty_uint:{
			uintmax_t res = m_uint - m_variant->m_range.m_step.m_uint;
			if( res < m_variant->m_range.m_min_bound.m_uint || m_variant->m_range.m_max_bound.m_uint < res )
				return false;
			m_uint = res;
					 }break;
		default:
			__assert( !"invalid variant range type" );
			return false;
		}
		break;
	default:
		if( 1 == m_index )
		{
			m_index = 0;
			break;
		}
		return false;
	}
	return true;
}

/*END OF variant.cxx*/
