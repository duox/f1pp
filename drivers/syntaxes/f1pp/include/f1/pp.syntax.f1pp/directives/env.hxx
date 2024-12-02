/*env.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::env_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective		super;
	typedef f1::f1pp::env_directive		self;

	explicit env_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
protected:
	const status_t			execute( f1::pp::Iparser & parser, const core::buffer & out_buf );
	static const status_t	line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );
protected:
	pp::string	m_ident;
	pp::variant::type_t	m_type;
	pp::Imacro::macro_flags_t	m_macro_flags;
	unsigned	m_def_flags;
};

/*END OF env.hxx*/
