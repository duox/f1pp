/*include_next.hxx*/

class F1_PP_API f1::c_pp::include_next_directive : public f1::c_pp::Idirective
{
public:
	typedef f1::c_pp::Idirective				super;
	typedef f1::c_pp::include_next_directive	self;

	explicit include_next_directive( unsigned flags = 0 ) __noexcept;

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
};

/*END OF include_next.hxx*/
