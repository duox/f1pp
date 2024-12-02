/*variant.hxx*/

class F1_PP_API f1::pp::variant
{
public:
	typedef f1::pp::variant	self;

	enum type_t {
		ty_none,
		ty_bool,		// bool/_Bool type
		ty_int,			// intmax_t
		ty_uint,		// uintmax_t
		ty_float,		// max_float
		ty_string,		// const char *
		ty_text,		// special type denoting non-quoted text
		ty_list,		// list of variants
		ty_range,		// range N..M..K

		ty_count,		// number of types
		ty_invalid = -1	// invalid type, used as a flag to signal an error
	};

			 variant( const variant & var ) noexcept;	// TODO: make explicit when own Tarray is completed
	explicit variant( const type_t ty = ty_none ) noexcept;
	explicit variant( const bool val ) noexcept;
	explicit variant( const intmax_t val ) noexcept;
	explicit variant( const uintmax_t val ) noexcept;
	explicit variant( const max_float_t val ) noexcept;
	explicit variant( const char * val, size_t length = size_t(-1) ) noexcept;
	explicit variant( const variant * var, size_t count ) noexcept;
	explicit variant( const variant & start, const variant & end ) noexcept;
	explicit variant( const variant & start, const variant & end, const variant & step ) noexcept;
	explicit variant( const max_float_t start, const max_float_t end, max_float_t step = max_float_t(0) ) noexcept;
	explicit variant( const intmax_t start, const intmax_t end, intmax_t step = intmax_t(0) ) noexcept;
	explicit variant( const uintmax_t start, const uintmax_t end, uintmax_t step = uintmax_t(0) ) noexcept;
			~variant();

	// Properties
public:
	type_t	get_type() const noexcept;
	const status_t	set_type( type_t ty ) noexcept;
	bool	can_cast_to( type_t ty ) const noexcept;
	uintmax_t	get_count() const noexcept;
protected:

	// iterators
public:
	struct base_iterator;

	struct iterator;
	iterator	get_iterator();

	struct const_iterator;
	const_iterator	get_const_iterator();

	struct const_reverse_iterator;
	const_reverse_iterator	get_const_reverse_iterator();

	struct reverse_iterator;
	reverse_iterator	get_reverse_iterator();

	// Operators
public:

	operator bool () const noexcept;
	operator intmax_t () const noexcept;
	operator uintmax_t () const noexcept;
	operator max_float_t () const noexcept;
	operator const char * () const noexcept;

	variant &	operator=( const variant & var ) noexcept;
	variant &	operator=( const bool val ) noexcept;
	variant &	operator=( const intmax_t val ) noexcept;
	variant &	operator=( const uintmax_t val ) noexcept;
	variant &	operator=( const max_float_t val ) noexcept;
	variant &	operator=( const char * val ) noexcept;

	bool	operator==( const variant & var ) const noexcept;
	bool	operator==( const bool val ) const noexcept;
	bool	operator==( const intmax_t val ) const noexcept;
	bool	operator==( const uintmax_t val ) const noexcept;
	bool	operator==( const max_float_t val ) const noexcept;
	bool	operator==( const char * val ) const noexcept;

	bool	operator!=( const variant & var ) const noexcept;
	bool	operator!=( const bool val ) const noexcept;
	bool	operator!=( const intmax_t val ) const noexcept;
	bool	operator!=( const uintmax_t val ) const noexcept;
	bool	operator!=( const max_float_t val ) const noexcept;
	bool	operator!=( const char * val ) const noexcept;
protected:

	// Operations
public:
	bool	is_valid() const noexcept;
	bool	is_empty() const noexcept;
	bool	is_numeric() const noexcept;

	const status_t	format( pp::string & buf ) const noexcept;

	const status_t	reset() noexcept;
	void	move( variant & var ) noexcept;

	variant *	dup() const;
	//variant *	dup_move();

	bool			as_bool() const noexcept;
	intmax_t		as_int() const noexcept;
	uintmax_t		as_uint() const noexcept;
	max_float_t		as_float() const noexcept;
	const char *	as_string( size_t * length = nullptr ) const noexcept;
	const char *	as_text( size_t * length = nullptr ) const noexcept;
	const variant *	as_list( size_t * count = nullptr ) const noexcept;

	const status_t	set_bool( const bool val ) noexcept;
	const status_t	set_int( const intmax_t val ) noexcept;
	const status_t	set_uint( const uintmax_t val ) noexcept;
	const status_t	set_float( const max_float_t val ) noexcept;
	const status_t	set_string( const char * val, size_t length = size_t(-1) ) noexcept;
	const status_t	set_string_move( const char * val, size_t length = size_t(-1) ) noexcept;
	const status_t	set_text( const char * val, size_t length = size_t(-1) ) noexcept;
	const status_t	set_text_move( const char * val, size_t length = size_t(-1) ) noexcept;
	const status_t	set_list( const variant * val, size_t count ) noexcept;
	const status_t	set_list_move( variant * val, size_t count ) noexcept;
	const status_t	set_range( const variant & start, const variant & end ) noexcept;
	const status_t	set_range( const variant & start, const variant & end, const variant & step ) noexcept;
	const status_t	set_range( const max_float_t start, const max_float_t end, max_float_t step = max_float_t(0) ) noexcept;
	const status_t	set_range( const intmax_t start, const intmax_t end, intmax_t step = intmax_t(0) ) noexcept;
	const status_t	set_range( const uintmax_t start, const uintmax_t end, uintmax_t step = uintmax_t(0) ) noexcept;
	const status_t	set_range_end( const variant & end ) noexcept;
protected:
  #if 0//def _DEBUG
	typedef type_t type_id_t;
  #else
	typedef uint8_t type_id_t;
  #endif

	struct string_def {
		const char *m_data;
		size_t		m_length;
	};
	struct list_def {
		variant *	m_data;
		size_t		m_count;
	};
	struct range_def_t {
		union value_type_t {
			intmax_t	m_int;
			uintmax_t	m_uint;
			max_float_t	m_float;
		};
		value_type_t	m_min_bound;
		value_type_t	m_max_bound;
		value_type_t	m_step;
		type_id_t		m_type;
	};
	union {
		bool		m_bool;
		intmax_t	m_int;
		uintmax_t	m_uint;
		max_float_t	m_float;
		string_def	m_string;
		list_def	m_list;
		range_def_t	m_range;
	};
	type_id_t	m_type;
private:
	void	_release_list();
	void	_reset_cache();
	mutable string_def	m_cached_string;
	mutable list_def	m_cached_list;
public:
	static variant	null;
	static variant	invalid;
};

struct f1::pp::variant::base_iterator {
protected:
	explicit base_iterator();
	explicit base_iterator( variant & var );
	void set_variant( variant & var );
	bool	value( f1::pp::variant & buf ) const;
	variant *	m_variant;
	union {
		size_t		m_index;
		bool		m_bool;
		max_float_t	m_float;
		intmax_t	m_int;
		uintmax_t	m_uint;
	};
};
struct f1::pp::variant::iterator : protected base_iterator {
	explicit iterator();
	explicit iterator( variant & var );
	bool	next();
	using base_iterator::value;
};
struct f1::pp::variant::const_iterator : protected iterator {
	explicit const_iterator();
	explicit const_iterator( variant & var );
	using iterator::set_variant;
	using iterator::next;
	using iterator::value;
};
struct f1::pp::variant::reverse_iterator : protected base_iterator {
	explicit reverse_iterator();
	explicit reverse_iterator( variant & var );
	void	set_variant( variant & var );
	bool	next();
	using base_iterator::value;
};
struct f1::pp::variant::const_reverse_iterator : protected reverse_iterator {
	explicit const_reverse_iterator();
	explicit const_reverse_iterator( variant & var );
	using reverse_iterator::set_variant;
	using reverse_iterator::next;
	using reverse_iterator::value;
};

#include "./variant.inl"

/*END OF variant.hxx*/
