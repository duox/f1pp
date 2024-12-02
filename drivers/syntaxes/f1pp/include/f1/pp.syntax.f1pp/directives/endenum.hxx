/*endenum.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::endenum_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective			super;
	typedef f1::f1pp::endenum_directive	self;

	explicit endenum_directive( unsigned flags = 0 );

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
//	virtual bool	get_next_value( f1pp::parser & par, const enumeration & en, pp::variant & result, status_t & status );
};

/*END OF endenum.hxx*/
