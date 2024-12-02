/*default.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::default_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective		super;
	typedef f1::f1pp::default_directive	self;

	explicit default_directive( unsigned flags = 0 );

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
};

/*END OF default.hxx*/
