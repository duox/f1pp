/*region.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::region_directive : public f1::pp::ignored_directive
{
public:
	typedef f1::pp::ignored_directive	super;
	typedef f1::f1pp::region_directive	self;

	explicit region_directive( unsigned flags = 0 );

	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;
};

/*END OF region.hxx*/
