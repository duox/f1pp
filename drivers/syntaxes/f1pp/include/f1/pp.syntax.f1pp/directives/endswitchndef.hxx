/*endswitchndef.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::endswitchndef_directive : public f1::f1pp::endswitch_directive
{
public:
	typedef f1::f1pp::endswitch_directive		super;
	typedef f1::f1pp::endswitchndef_directive	self;

	explicit endswitchndef_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;
};

/*END OF endswitchndef.hxx*/
