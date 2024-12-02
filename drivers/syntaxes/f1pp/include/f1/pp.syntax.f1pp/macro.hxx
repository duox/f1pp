/*macro.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::macro : public f1::c_pp::macro
{
public:
	typedef f1::c_pp::macro	super;
	typedef f1::f1pp::macro	self;

	explicit macro( const core::Iidentifier * ident = nullptr );

	// Properties
public:
//	const status_t		set_value_variant( const pp::variant & var );
//	const pp::variant &	get_value_variant() const;
//		  pp::variant &	get_value_variant();

	begin_scoped_enum( ex_flags_t )
		fl_parenthesis	= 0x0001,	///< definition of the macro uses parenthesis
		fl_multiline	= 0x0002,	///< multiline value with directives
		fl_function		= 0x0004,	///< macro value is a list of directives to execute
		fl_const		= 0x0008,	///< macro value cannot be changed
		fl_global		= 0x0010,	///< the macro is located in the root namespace
		fl_public		= 0x0020,	///< the macro is located in the parent namespace
	end_scoped_enum;
	const status_t	modify_ex_flags( ex_flags_t flags_to_set, ex_flags_t flags_to_clear = ex_flags_t::null, ex_flags_t * flags_prev = nullptr );
	const status_t	modify_ex_flags( ex_flags_t flags, bool set_or_clear, ex_flags_t * flags_prev = nullptr );
		ex_flags_t	get_ex_flags( ex_flags_t mask = ex_flags_t::all ) const;
protected:
	ex_flags_t	m_ex_flags;
//	pp::variant	m_value_variant;

	// Operations
public:
	virtual bool pre_change_value( f1::pp::string & string_value, f1::pp::variant * variant_value ) override;
	virtual bool is_stringizing_operator( f1::c_pp::parser & par, const char *& s, const char * end_s ) const override;
	virtual const status_t	parse( pp::Iparser & parser, pp::string & result ) const override;
	virtual const status_t	expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const override;
protected:
	f1pp::parser::multiline_patch_callback	m_multiline_patch_callback;
};

/*END OF macro.hxx*/
