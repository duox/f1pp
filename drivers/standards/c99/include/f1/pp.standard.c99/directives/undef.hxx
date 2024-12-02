/*undef.hxx*/

class F1_PP_API f1::c_pp::c99::undef_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective		super;
	typedef f1::c_pp::c99::undef_directive	self;

	explicit undef_directive( unsigned flags = 0 ) __noexcept;

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;

	// Hooks
protected:
	virtual const status_t	undefine_macro( pp::Iparser & parser, const pp::string & ident, bool force = false );
};

/*END OF undef.hxx*/
