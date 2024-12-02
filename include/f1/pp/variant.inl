/*variant.inl*/

inline
f1::pp::variant::variant( const variant & var ) noexcept:
	m_type( type_id_t(ty_none) )
{
	m_cached_list.m_data = nullptr;
	m_cached_string.m_data = nullptr;
	operator=( var );
}
inline
f1::pp::variant::variant( const type_t ty ) noexcept:
	m_type( ty_none )
{
	m_cached_list.m_data = nullptr;
	m_cached_string.m_data = nullptr;
	set_type( ty );
}
inline
f1::pp::variant::variant( const bool val ) noexcept:
	m_type( type_id_t(ty_bool) )
{
	m_cached_list.m_data = nullptr;
	m_cached_string.m_data = nullptr;
	m_bool = val;
}
inline
f1::pp::variant::variant( const intmax_t val ) noexcept:
	m_type( type_id_t(ty_int) )
{
	m_cached_list.m_data = nullptr;
	m_cached_string.m_data = nullptr;
	m_int = val;
}
inline
f1::pp::variant::variant( const uintmax_t val ) noexcept:
	m_type( type_id_t(ty_uint) )
{
	m_cached_list.m_data = nullptr;
	m_cached_string.m_data = nullptr;
	m_uint = val;
}
inline
f1::pp::variant::variant( const max_float_t val ) noexcept:
	m_type( type_id_t(ty_float) )
{
	m_cached_list.m_data = nullptr;
	m_cached_string.m_data = nullptr;
	m_float = val;
}
inline
f1::pp::variant::variant( const char * val, size_t length ) noexcept:
	m_type( ty_none )
{
	m_cached_list.m_data = nullptr;
	m_cached_string.m_data = nullptr;
	set_string( val, length );
}
inline
f1::pp::variant::variant( const variant * var, size_t count ) noexcept:
	m_type( ty_none )
{
	m_cached_list.m_data = nullptr;
	m_cached_string.m_data = nullptr;
	set_list( var, count );
}
inline
f1::pp::variant::variant( const variant & start, const variant & end ) noexcept:
	m_type( ty_none )
{
	m_cached_list.m_data = nullptr;
	m_cached_string.m_data = nullptr;
	set_range( start, end );
}
inline
f1::pp::variant::variant( const variant & start, const variant & end, const variant & step ) noexcept:
	m_type( ty_none )
{
	m_cached_list.m_data = nullptr;
	m_cached_string.m_data = nullptr;
	set_range( start, end, step );
}
inline
f1::pp::variant::variant( const max_float_t start, const max_float_t end, max_float_t step ) noexcept:
	m_type( ty_none )
{
	m_cached_list.m_data = nullptr;
	m_cached_string.m_data = nullptr;
	set_range( start, end, step );
}
inline
f1::pp::variant::variant( const intmax_t start, const intmax_t end, intmax_t step ) noexcept:
	m_type( ty_none )
{
	m_cached_list.m_data = nullptr;
	m_cached_string.m_data = nullptr;
	set_range( start, end, step );
}
inline
f1::pp::variant::variant( const uintmax_t start, const uintmax_t end, uintmax_t step ) noexcept:
	m_type( ty_none )
{
	m_cached_list.m_data = nullptr;
	m_cached_string.m_data = nullptr;
	set_range( start, end, step );
}
inline
f1::pp::variant::~variant()
{
	if( ty_string == m_type || ty_text == m_type )
		free( (void *) m_string.m_data );
	else if( ty_list == m_type )
		_release_list();
	_reset_cache();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

inline
f1::pp::variant::type_t f1::pp::variant::get_type() const noexcept
{
	return type_t(m_type);
}

inline
uintmax_t f1::pp::variant::get_count() const noexcept
{
	switch( m_type )
	{
	case ty_list:
		return m_list.m_count;
	case ty_string:
		return m_string.m_length;
	case ty_range:
		intmax_t count;
		switch( m_range.m_type )
		{
		case ty_float:
			if( 0 != m_range.m_step.m_float )
				count = ssize_t( (m_range.m_max_bound.m_float - m_range.m_min_bound.m_float) / m_range.m_step.m_float );
			else
				count = ssize_t(m_range.m_max_bound.m_float - m_range.m_min_bound.m_float);
			break;
		case ty_int:
			if( 0 != m_range.m_step.m_int )
				count = m_range.m_max_bound.m_int - m_range.m_min_bound.m_int / m_range.m_step.m_int;
			else
				count = m_range.m_max_bound.m_int - m_range.m_min_bound.m_int;
			break;
		case ty_uint:
			if( 0 != m_range.m_step.m_uint )
				count = m_range.m_max_bound.m_uint - m_range.m_min_bound.m_uint / m_range.m_step.m_uint;
			else
				count = m_range.m_max_bound.m_uint - m_range.m_min_bound.m_uint;
			break;
		default:
			count = 0;
			__assert( !"invalid variant range type" );
			__assume( false );
		}
		return 0 > count ? 0 : uintmax_t(count);
	default:
		break;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operators

inline
f1::pp::variant::operator bool () const noexcept
{
	return as_bool();
}
inline
f1::pp::variant::operator intmax_t () const noexcept
{
	return as_int();
}
inline
f1::pp::variant::operator uintmax_t () const noexcept
{
	return as_uint();
}
inline
f1::pp::variant::operator max_float_t () const noexcept
{
	return as_float();
}
inline
f1::pp::variant::operator const char * () const noexcept
{
	return as_string();
}

inline
f1::pp::variant & f1::pp::variant::operator=( const variant & var ) noexcept
{
	if( this != &var )
	{
		switch( var.m_type )
		{
		case ty_none:
			set_type( ty_none );
			break;
		case ty_bool:
			set_bool( var.m_bool );
			break;
		case ty_int:
			set_int( var.m_int );
			break;
		case ty_uint:
			set_uint( var.m_uint );
			break;
		case ty_float:
			set_float( var.m_float );
			break;
		case ty_list:
			set_list( var.m_list.m_data, var.m_list.m_count );
			break;
		case ty_string:
			set_string( var.m_string.m_data, var.m_string.m_length );
			break;
		case ty_text:
			set_text( var.m_string.m_data, var.m_string.m_length );
			break;
		case ty_range:
			reset();
			m_range = var.m_range;
			m_type = ty_range;
			break;
		default:
			DBG_FAIL();
		}
	}
	return *this;
}
inline
f1::pp::variant & f1::pp::variant::operator=( const bool val ) noexcept
{
	set_bool( val );
	return *this;
}
inline
f1::pp::variant & f1::pp::variant::operator=( const intmax_t val ) noexcept
{
	set_int( val );
	return *this;
}
inline
f1::pp::variant & f1::pp::variant::operator=( const uintmax_t val ) noexcept
{
	set_uint( val );
	return *this;
}
inline
f1::pp::variant & f1::pp::variant::operator=( const max_float_t val ) noexcept
{
	set_float( val );
	return *this;
}
inline
f1::pp::variant & f1::pp::variant::operator=( const char * val ) noexcept
{
	set_string( val );
	return *this;
}

inline
bool f1::pp::variant::operator==( const variant & var ) const noexcept
{
	if( m_type != var.m_type )
		return false;
	switch( m_type )
	{
	case ty_none:	return true;
	case ty_bool:	return m_bool == var.m_bool;
	case ty_int:	return m_int == var.m_int;
	case ty_uint:	return m_uint == var.m_uint;
	case ty_float:	return m_float == var.m_float;
	case ty_list:
		if( m_list.m_count != var.m_list.m_count )
			return false;
		for( size_t i = 0; i < m_list.m_count; ++ i )
		{
			if( m_list.m_data[i] != var.m_list.m_data[i] )
				return false;
		}
		break;
	case ty_text:
	case ty_string:
		if( nullcstr == m_string.m_data || nullcstr == var.m_string.m_data )
			return false;
		return !strcmp( m_string.m_data, var.m_string.m_data );
	case ty_range:
		if( m_range.m_type != var.m_range.m_type )
			return false;
		switch( m_range.m_type )
		{
		case ty_float:
			return
				__float_equal( m_range.m_min_bound.m_float, var.m_range.m_min_bound.m_float ) &&
				__float_equal( m_range.m_max_bound.m_float, var.m_range.m_max_bound.m_float ) &&
				__float_equal( m_range.m_step.m_float, var.m_range.m_step.m_float );
		case ty_int:
			return
				m_range.m_min_bound.m_int == var.m_range.m_min_bound.m_int &&
				m_range.m_max_bound.m_int == var.m_range.m_max_bound.m_int &&
				m_range.m_step.m_int == var.m_range.m_step.m_int;
		case ty_uint:
			return
				m_range.m_min_bound.m_uint == var.m_range.m_min_bound.m_uint &&
				m_range.m_max_bound.m_uint == var.m_range.m_max_bound.m_uint &&
				m_range.m_step.m_uint == var.m_range.m_step.m_uint;
		default:
			__assert( !"invalid variant range type" );
			return false;
		}
	default:
		return false;
	}
	return true;
}
inline
bool f1::pp::variant::operator==( const bool val ) const noexcept
{
	if( ty_string == type_t(m_type) || ty_text == type_t(m_type) )
		return false;
	return as_bool() == val;
}
inline
bool f1::pp::variant::operator==( const intmax_t val ) const noexcept
{
	if( ty_string == type_t(m_type) || ty_text == type_t(m_type) )
		return false;
	return as_int() == val;
}
inline
bool f1::pp::variant::operator==( const uintmax_t val ) const noexcept
{
	if( ty_string == type_t(m_type) || ty_text == type_t(m_type) )
		return false;
	return as_uint() == val;
}
inline
bool f1::pp::variant::operator==( const max_float_t val ) const noexcept
{
	if( ty_string == type_t(m_type) || ty_text == type_t(m_type) )
		return false;
	return as_float() == val;
}
inline
bool f1::pp::variant::operator==( const char * val ) const noexcept
{
	if( ty_string != type_t(m_type) && ty_text != type_t(m_type) )
		return false;
	return nullcstr == m_string.m_data || nullcstr == val ? false : !strcmp( m_string.m_data, val );
}

inline
bool f1::pp::variant::operator!=( const variant & var ) const noexcept
{
	return !operator==( var );
}
inline
bool f1::pp::variant::operator!=( const bool val ) const noexcept
{
	return !operator==( val );
}
inline
bool f1::pp::variant::operator!=( const intmax_t val ) const noexcept
{
	return !operator==( val );
}
inline
bool f1::pp::variant::operator!=( const uintmax_t val ) const noexcept
{
	return !operator==( val );
}
inline
bool f1::pp::variant::operator!=( const max_float_t val ) const noexcept
{
	return !operator==( val );
}
inline
bool f1::pp::variant::operator!=( const char * val ) const noexcept
{
	return !operator==( val );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

inline
bool f1::pp::variant::is_valid() const noexcept
{
	return unsigned(m_type) < ty_count;
}

inline
bool f1::pp::variant::is_numeric() const noexcept
{
	return ty_int == m_type || ty_uint == m_type || ty_float == m_type;
}

inline
bool f1::pp::variant::is_empty() const noexcept
{
	return ty_none == m_type;
}
inline
const status_t f1::pp::variant::reset() noexcept
{
	if( ty_string == type_t(m_type) || ty_text == type_t(m_type) )
		free( (void *) m_string.m_data );
	else if( ty_list == m_type )
		_release_list();
	_reset_cache();

	m_type = ty_none;

	return STATUS_SUCCESS;
}
inline
void f1::pp::variant::move( variant & var ) noexcept
{
	reset();
	__small_memcpy( this, &var, sizeof(*this) );
	var.m_type = ty_none;
}

inline
f1::pp::variant * f1::pp::variant::dup() const
{
	variant * res = new(std::nothrow) variant[1];
	if( nullptr == res )
		return nullptr;
	switch( m_type )
	{
	case ty_string:
		res->set_string( m_string.m_data, m_string.m_length );
		break;
	case ty_text:
		res->set_text( m_string.m_data, m_string.m_length );
		break;
	case ty_list:
		res->set_list( m_list.m_data, m_list.m_count );
		break;
	case ty_range:
		res->m_type = ty_range;
		res->m_range = m_range;
	default:
		res->m_type = m_type;
		res->m_float = m_float;
		break;
	}
	return res;
}

inline
bool f1::pp::variant::as_bool() const noexcept
{
	switch( m_type ) {
		case ty_none:	return false;
		case ty_bool:	return m_bool;
		case ty_int:	return !!m_int;
		case ty_uint:	return !!m_uint;
		case ty_float:	return m_float < - MAX_FLOAT_EPSILON || + MAX_FLOAT_EPSILON < m_float;
		case ty_list:	return 0 != m_list.m_count;
		case ty_string:
		case ty_text:	return 0 != m_string.m_length;
		case ty_invalid:
			__assert( !"f1::pp::variant::as_bool for invalid object" );
			return false;
		default:
			return false;
	}
}
inline
intmax_t f1::pp::variant::as_int() const noexcept
{
	switch( m_type ) {
		case ty_none:	return 0;
		case ty_bool:	return intmax_t(m_bool);
		case ty_int:	return m_int;
		case ty_uint:	return m_uint;
		case ty_float:	return intmax_t(m_float);
		case ty_invalid:
			__assert( !"f1::pp::variant::as_int for invalid object" );
			return 0;
		default:
			return 0;
	}
}
inline
uintmax_t f1::pp::variant::as_uint() const noexcept
{
	switch( m_type ) {
		case ty_none:	return 0;
		case ty_bool:	return uintmax_t(m_bool);
		case ty_int:	return m_int;
		case ty_uint:	return m_uint;
		case ty_float:	return uintmax_t(m_float);
		case ty_invalid:
			__assert( !"f1::pp::variant::as_uint for invalid object" );
			return 0;
		default:
			return 0;
	}
}
inline
max_float_t f1::pp::variant::as_float() const noexcept
{
	switch( m_type ) {
		case ty_none:	return 0.0;
		case ty_bool:	return max_float_t(m_bool);
		case ty_int:	return max_float_t(m_int);
		case ty_uint:	return max_float_t(m_uint);
		case ty_float:	return m_float;
		case ty_invalid:
			__assert( !"f1::pp::variant::as_float for invalid object" );
			return 0;
		default:
			return 0;
	}
}
inline
const char * f1::pp::variant::as_string( size_t * length ) const noexcept
{
	const string_def * str = &m_string;
	if( ty_string != m_type && ty_text != m_type )
	{
		if( nullptr == m_cached_string.m_data )
		{
			pp::string buf;
			format( buf );
			m_cached_string.m_length = buf.length();
			m_cached_string.m_data = _strndup( buf.c_str(), m_cached_string.m_length );
		}
		str = &m_cached_string;
	}
	if( nullptr != length )
		*length = str->m_length;
	return nullptr == str->m_data ? "" : str->m_data;
}
inline
const char * f1::pp::variant::as_text( size_t * length ) const noexcept
{
	const string_def * str = &m_string;
	if( ty_string != m_type && ty_text != m_type )
	{
		if( nullptr == m_cached_string.m_data )
		{
			pp::string buf;
			format( buf );
			m_cached_string.m_length = buf.length();
			m_cached_string.m_data = _strndup( buf.c_str(), m_cached_string.m_length );
		}
		str = &m_cached_string;
	}
	if( nullptr != length )
		*length = str->m_length;
	return nullptr == str->m_data ? "" : str->m_data;
}
inline
const f1::pp::variant * f1::pp::variant::as_list( size_t * count ) const noexcept
{
	const list_def * lst = &m_list;
	if( ty_list != m_type )
	{
		if( nullptr == m_cached_list.m_data )
		{
			m_cached_list.m_data = dup();
			m_cached_list.m_count = 1;
		}
		lst = &m_cached_list;
	}
	if( nullptr != count )
		*count = lst->m_count;
	return nullptr == lst->m_data ? nullptr : lst->m_data;
}

inline
const status_t f1::pp::variant::set_bool( const bool val ) noexcept
{
	if( ty_string == m_type || ty_text == m_type )
		free( (void *) m_string.m_data );
	else if( ty_list == m_type )
		_release_list();
	_reset_cache();

	m_type = ty_bool;
	m_bool = val;

	return STATUS_SUCCESS;
}
inline
const status_t f1::pp::variant::set_int( const intmax_t val ) noexcept
{
	if( ty_string == m_type || ty_text == m_type )
		free( (void *) m_string.m_data );
	else if( ty_list == m_type )
		_release_list();
	_reset_cache();

	m_type = ty_int;
	m_int = val;

	return STATUS_SUCCESS;
}
inline
const status_t f1::pp::variant::set_uint( const uintmax_t val ) noexcept
{
	if( ty_string == m_type || ty_text == m_type )
		free( (void *) m_string.m_data );
	else if( ty_list == m_type )
		_release_list();
	_reset_cache();

	m_type = ty_uint;
	m_uint = val;

	return STATUS_SUCCESS;
}
inline
const status_t f1::pp::variant::set_float( const max_float_t val ) noexcept
{
	if( ty_string == m_type || ty_text == m_type )
		free( (void *) m_string.m_data );
	else if( ty_list == m_type )
		_release_list();
	_reset_cache();

	m_type = ty_float;
	m_float = val;

	return STATUS_SUCCESS;
}
inline
const status_t f1::pp::variant::set_string( const char * val, size_t length ) noexcept
{
	if( ty_list == m_type )
		_release_list();

	// Set empty string
	if( nullptr == val )
	{
		if( ty_string == m_type || ty_text == m_type )
			free( (void *) m_string.m_data );
		else if( ty_list == m_type )
			_release_list();
		_reset_cache();

		m_string.m_data = nullptr;
		m_type = ty_string;

		return STATUS_SUCCESS;
	}

	if( size_t(-1) == length )
		length = __small_strlen( val );

	char * str;
	if( ty_string == m_type || ty_text == m_type )
		str = (char *) realloc( (void *) m_string.m_data, sizeof(char) * (length + 1) );
	else
		str = (char *) malloc( sizeof(char) * (length + 1) );
	if( nullptr == str )
		return STATUS_ERROR_INSUFFICIENT_MEMORY;

	__small_memcpy( str, val, length );
	str[length] = '\0';

	m_string.m_data = str;
	m_string.m_length = length;
	m_type = ty_string;

	_reset_cache();

	return STATUS_SUCCESS;
}
inline
const status_t f1::pp::variant::set_string_move( const char * val, size_t length ) noexcept
{
	if( ty_string == m_type || ty_text == m_type )
		free( (void *) m_string.m_data );
	else if( ty_list == m_type )
		_release_list();
	_reset_cache();

	m_string.m_data = val;
	m_string.m_length = length;
	m_type = ty_string;

	return STATUS_SUCCESS;
}
inline
const status_t f1::pp::variant::set_text( const char * val, size_t length ) noexcept
{
	status_t status;

	if( set_string( val, length ).failed_status( status ) )
		return status;
	m_type = ty_text;

	_reset_cache();

	return status;
}
inline
const status_t f1::pp::variant::set_text_move( const char * val, size_t length ) noexcept
{
	status_t status;

	if( set_string_move( val, length ).failed_status( status ) )
		return status;
	m_type = ty_text;

	_reset_cache();

	return status;
}
inline
const status_t f1::pp::variant::set_list( const variant * val, size_t count ) noexcept
{
	if( ty_string == m_type || ty_text == m_type )
		free( (void *) m_string.m_data );
	else if( ty_list == m_type )
		_release_list();
	_reset_cache();

	if( 0 == count )
	{
		m_type = ty_none;
		return STATUS_SUCCESS;
	}

	m_list.m_data = new(std::nothrow) variant[count];
	if( nullptr == m_list.m_data )
	{
		m_type = ty_none;
		return STATUS_SUCCESS;
	}

	for( size_t i = 0; i < count; ++ i )
		m_list.m_data[i] = val[i];
	m_type = ty_list;
	m_list.m_count = count;

	return STATUS_SUCCESS;
}
inline
const status_t f1::pp::variant::set_list_move( variant * val, size_t count ) noexcept
{
	if( ty_string == m_type || ty_text == m_type )
		free( (void *) m_string.m_data );
	else if( ty_list == m_type )
		_release_list();
	_reset_cache();

	m_list.m_data = val;
	m_list.m_count = count;
	m_type = ty_list;

	return STATUS_SUCCESS;
}

inline
void f1::pp::variant::_release_list()
{
	for( size_t i = 0; i < m_list.m_count; ++ i )
		m_list.m_data[i].reset();
	delete[] m_list.m_data;
	m_list.m_data = nullptr;
}
inline
void f1::pp::variant::_reset_cache()
{
	if( nullptr != m_cached_list.m_data )
	{
		for( size_t i = 0; i < m_list.m_count; ++ i )
			m_cached_list.m_data[i].reset();
		delete[] m_cached_list.m_data;
		m_cached_list.m_data = nullptr;
	}

	if( nullptr != m_cached_string.m_data )
	{
		free( (void *) m_cached_string.m_data );
		m_cached_string.m_data = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline
f1::pp::variant::base_iterator::base_iterator()
{
	__small_memset( this, 0, sizeof(*this) );
}
inline
f1::pp::variant::base_iterator::base_iterator( variant & var )
{
	set_variant( var );
}

inline
void
f1::pp::variant::base_iterator::set_variant( variant & var )
{
	m_variant = &var;
	switch( m_variant->m_type )
	{
	case ty_range:
		switch( var.m_range.m_type )
		{
		case ty_float:	m_float	= var.m_range.m_min_bound.m_float - 1;	break;
		case ty_int:	m_int	= var.m_range.m_min_bound.m_int - 1;	break;
		case ty_uint:	m_uint	= var.m_range.m_min_bound.m_uint - 1;	break;
		default:
			__assert( !"invalid variant range type" );
		}
		break;
	default:
		m_index = size_t(-1);
		break;
	}
}

inline
bool f1::pp::variant::base_iterator::value( f1::pp::variant & buf ) const
{
	__debugbreak_if( nullptr == m_variant )
		return false;
	switch( m_variant->m_type )
	{
	case ty_list:
		if( m_index >= m_variant->m_list.m_count )
			return false;
		buf = m_variant->m_list.m_data[m_index];
		break;
	case ty_text:
	case ty_string:
		if( m_index >= m_variant->m_string.m_length )
			return false;
		buf.set_uint( m_variant->m_string.m_data[m_index] );
		break;
	case ty_range:
		switch( m_variant->m_range.m_type )
		{
		case ty_float:
			if( m_float > m_variant->m_range.m_max_bound.m_float )
				return false;
			buf.set_float( m_float );
			break;
		case ty_int:
			if( m_int > m_variant->m_range.m_max_bound.m_int )
				return false;
			buf.set_int( m_int );
			break;
		case ty_uint:
			if( m_uint > m_variant->m_range.m_max_bound.m_uint )
				return false;
			buf.set_uint( m_uint );
			break;
		default:
			__assert( !"invalid variant range type" );
			return false;
		}
		break;
	default:
		if( 0 != m_index )
			return false;
		buf = *m_variant;
		break;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline
f1::pp::variant::iterator::iterator()
{
}
inline
f1::pp::variant::iterator::iterator( variant & var ):
	base_iterator( var )
{
	if( ty_range != m_variant->m_type )
		m_index = size_t(-1);
	else
	{
		switch( var.m_range.m_type )
		{
		case ty_float:	m_float	= var.m_range.m_min_bound.m_float	- var.m_range.m_step.m_float;	break;
		case ty_int:	m_int	= var.m_range.m_min_bound.m_int		- var.m_range.m_step.m_int;		break;
		case ty_uint:	m_uint	= var.m_range.m_min_bound.m_uint	- var.m_range.m_step.m_uint;	break;
		default:
			__assert( !"invalid variant range type" );
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline
f1::pp::variant::const_iterator::const_iterator()
{
}
inline
f1::pp::variant::const_iterator::const_iterator( variant & var ):
	iterator( var )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline
f1::pp::variant::reverse_iterator::reverse_iterator()
{
}
inline
f1::pp::variant::reverse_iterator::reverse_iterator( variant & var )
{
	set_variant( var );
}
inline
void
f1::pp::variant::reverse_iterator::set_variant( variant & var )
{
	base_iterator::set_variant( var );
	switch( m_variant->m_type )
	{
	case ty_list:
		m_index = var.m_list.m_count;
		break;
	case ty_string:
	case ty_text:
		m_index = var.m_string.m_length;
		break;
	case ty_range:
		if( ty_range != m_variant->m_type )
			m_index = size_t(-1);
		else
		{
			switch( var.m_range.m_type )
			{
			case ty_float:	m_float	= var.m_range.m_max_bound.m_float	+ var.m_range.m_step.m_float;	break;
			case ty_int:	m_int	= var.m_range.m_max_bound.m_int		+ var.m_range.m_step.m_int;		break;
			case ty_uint:	m_uint	= var.m_range.m_max_bound.m_uint	+ var.m_range.m_step.m_uint;	break;
			default:
				__assert( !"invalid variant range type" );
			}
		}
		break;
	default:
		m_index = 1;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline
f1::pp::variant::const_reverse_iterator::const_reverse_iterator()
{
}
inline
f1::pp::variant::const_reverse_iterator::const_reverse_iterator( variant & var ):
	reverse_iterator( var )
{
}

/*END OF variant.inl*/
