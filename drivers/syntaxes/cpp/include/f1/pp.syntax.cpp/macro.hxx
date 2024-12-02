/*macro.hxx*/

class F1_PP_API f1::c_pp::macro : public f1::pp::Iuser_macro
{
public:
	typedef f1::pp::Iuser_macro	super;
	typedef f1::c_pp::macro		self;

	explicit macro( const core::Iidentifier * ident = nullptr ) NOEXCEPT;

	virtual const status_t	initialize( const pp::string & name, const f1::pp::attribute_list * attrs = nullptr,
		const pp::param_list * params = nullptr, const pp::string * value = nullptr, pp::Imacro::macro_flags_t flags = null ) override;

	// Properties
public:
	virtual void	post_change_value( const f1::pp::string & string_value, const f1::pp::variant & variant_value ) override;

	//const status_t	set_recursive_expansion_nesting( unsigned macro_expansion_nesting );
	//unsigned		get_recursive_expansion_nesting() const;
protected:
	//unsigned	m_macro_expansion_nesting;

	// Operations
public:
	virtual const status_t	expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const override;
	//virtual const status_t	parse_value( f1::c_pp::parser & par );

	virtual bool	is_stringizing_operator( f1::c_pp::parser & par, const char *& s, const char * end_s ) const;
	virtual bool	is_charizing_operator( f1::c_pp::parser & par, const char *& s, const char * end_s ) const;
	virtual bool	is_merge_operator( f1::c_pp::parser & par, const char *& s, const char * end_s ) const;
protected:
	static const int16_t IDX_NOTHING = -1;	///< entry doesn't refer to anything
	static const int16_t IDX_VA_ARGS = -2;	///< entry refers to the __VA_ARGS__ pseudo-macro
	static const int16_t IDX_VA_OPT	 = -3;	///< entry refers to the __VA_OPT__ pseudo-macro
	struct map_entry {
//		pp::string	m_id;				///< identifier of free string
		uint32_t	m_offset;			///< offset inside value string
		uint32_t	m_length;			///< len of the parameter, including all break ups
		 int16_t	m_param_index;		///< index of macro parameter (if IDX_NOTHING, it's just an identifier)
		uint16_t	m_op;				///< operator id (see op_id_t), op_none = no operator
	};
	core::Tarray<map_entry>	m_map;
	pp::string	m_map_string;			///< string either in m_value, or transformed (w/o comments etc.)
	bool		m_map_has_varags;
	bool generate_map( f1::c_pp::parser & par, const pp::arg_list * args ) noexcept;
	begin_scoped_enum( op_id_t )
		op_none			= 0x00,	///< a sequence of symbols or a parameter (depends on the map_entry::m_param_index value)
		op_space		= 0x01,	///< one or more spaces
		op_stringify	= 0x02,	///< stringizing operator
		op_charify		= 0x04,	///< charazing operator
		op_merge_left	= 0x08,	///< merge operator is at left
		op_merge_right	= 0x10	///< merge operator is at right
	end_scoped_enum;
	ptrdiff_t add_map_entry( op_id_t id, const char * start, size_t length, signed param_index = -1 );
	//void trim( const pp::string & str, size_t & start, size_t & len ) const;
	const status_t out_str( f1::c_pp::parser & c_pp_par, pp::string & buf, const map_entry & entry, const char * str, size_t len ) const;
	//const status_t	_expand_string( pp::string & outbuf, const pp::arg_list * args, bool & were_macros ) const;
};

/*END OF macro.hxx*/
