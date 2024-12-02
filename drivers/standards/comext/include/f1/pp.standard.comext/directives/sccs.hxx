/*sccs.hxx*/

class F1_PP_API f1::c_pp::comext::sccs_directive : public f1::pp::known_directive
{
public:
	typedef f1::pp::known_directive				super;
	typedef f1::c_pp::comext::sccs_directive	self;

	explicit sccs_directive( unsigned flags = 0 ) __noexcept;

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	// Operations
public:
};

/*END OF sccs.hxx*/
