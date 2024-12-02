/*set.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::set_directive : public f1::f1pp::assignment_directive
{
public:
	typedef f1::f1pp::assignment_directive		super;
	typedef f1::f1pp::set_directive				self;

	explicit set_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;
protected:

	// Target overrides
public:
	virtual const status_t	operator_complete( f1pp::parser & parser ) override;
};

/*END OF set.hxx*/
