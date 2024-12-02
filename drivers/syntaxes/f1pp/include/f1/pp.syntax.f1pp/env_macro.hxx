/*env_macro.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::env_macro : public f1::f1pp::macro
{
public:
	typedef f1::f1pp::macro		super;
	typedef f1::f1pp::env_macro	self;

	explicit env_macro( const core::Iidentifier * ident = nullptr );

	const status_t	initialize( f1::f1pp::parser & parser, const pp::string & name, const f1::pp::attribute_list * attrs = nullptr,
		const pp::param_list * params = nullptr, const pp::string * value = nullptr, macro_flags_t flags = null );

	// Properties
public:
	const f1::f1pp::parser *	get_parser() const;
		  f1::f1pp::parser *	get_parser();

	const status_t	set_env_var( const pp::string & env_var_name );
	const status_t	set_env_var_move( pp::string & env_var_name );
	const pp::string & get_env_var_name() const;

	const status_t	set_env_var_type( pp::variant::type_t type );
	pp::variant::type_t		get_env_var_type() const;
protected:
	core::Tweak_ref<f1::f1pp::parser>	m_parser;
	mutable pp::string		m_env_var_name;
	mutable f1::pp::string	m_env_var_value;
	mutable f1::pp::variant	m_env_var_variant;
	const status_t	_read_env_value() const;
	const status_t	_read_env_variant() const;
	const status_t	_write_env_value( const core::stringslice & value ) const;
//	pp::variant::type_t		m_type;

	// Operations
public:
	virtual const status_t	expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const override;

	virtual const f1::pp::variant &	get_value_variant() const override;
	virtual const f1::pp::string &	get_value() const override;
	virtual bool	pre_change_value( f1::pp::string & value_string, f1::pp::variant * value_variane ) override;
};

/*END OF env_macro.hxx*/
