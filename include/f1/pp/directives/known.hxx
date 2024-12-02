/*known.hxx*/

class F1_PP_API f1::pp::known_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective		super;
	typedef f1::pp::known_directive	self;

	explicit known_directive( unsigned flags = 0 ) __noexcept;

	// Properties
public:
	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
};

/*END OF known.hxx*/
