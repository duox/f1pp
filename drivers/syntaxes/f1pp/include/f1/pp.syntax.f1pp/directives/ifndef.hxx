/*ifndef.hxx*/

class F1_PP_API f1::f1pp::ifndef_directive : public f1::c_pp::c99::ifndef_directive
{
public:
	typedef f1::c_pp::c99::ifndef_directive	super;
	typedef f1::f1pp::ifndef_directive		self;

	explicit ifndef_directive( unsigned flags = 0 ) __noexcept;

	// Properties
public:
	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
};

/*END OF ifndef.hxx*/
