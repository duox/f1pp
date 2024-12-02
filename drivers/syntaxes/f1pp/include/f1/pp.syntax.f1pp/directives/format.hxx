/*format.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::format_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective				super;
	typedef f1::f1pp::format_directive		self;

	explicit format_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *		get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned			get_caps( unsigned mask = unsigned(-1) ) const noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
protected:
	const status_t			execute( f1::pp::Iparser & parser, const core::buffer & out_buf );
	static const status_t	line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );

	enum prefix_t { pref_none, pref_half, pref_int, pref_long, pref_int32, pref_int64, pref_wide };
	static const unsigned fmt_left_align	= 0x0001;
	static const unsigned fmt_prefix_sign	= 0x0002;
	static const unsigned fmt_prefix_zeroes	= 0x0004;
	static const unsigned fmt_prefix_blanks	= 0x0008;
	static const unsigned fmt_prefix_sharp	= 0x0010;
	struct format_context
	{
		unsigned	m_flags;
		long		m_width;
		long		m_precision;
		prefix_t	m_prefix;
	};

	static const status_t	format_string( f1::c_pp::parser & c_pp_par, core::string & buf, const pp::string & format, const pp::arg_list & args );
	static const status_t	store_char( f1::c_pp::parser & c_pp_par, const format_context & fmt, core::string & buf, const pp::variant & arg, bool wide_char );
	static const status_t	store_int( f1::c_pp::parser & c_pp_par, const format_context & fmt, core::string & buf, const pp::variant & arg, int radix, bool is_signed, bool is_uppercase = false );
	static const status_t	store_str( f1::c_pp::parser & c_pp_par, const format_context & fmt, core::string & buf, const pp::variant & arg );
	static intmax_t			fetch_int_arg ( f1::c_pp::parser & c_pp_par, const pp::variant & arg, bool * valid_num = nullptr );
	static uintmax_t		fetch_uint_arg( f1::c_pp::parser & c_pp_par, const pp::variant & arg, bool * valid_num = nullptr );
	static bool				get_next_arg( f1::c_pp::parser & c_pp_par, pp::variant & buf, pp::arg_list::arg_desc_list_t::const_iterator & arg_it );

	pp::string	m_ident;
	pp::string	m_format;
};

/*END OF format.hxx*/
