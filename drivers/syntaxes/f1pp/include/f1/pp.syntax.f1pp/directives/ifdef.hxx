/*ifdef.hxx*/

class F1_PP_API f1::f1pp::ifdef_directive : public f1::c_pp::c99::ifdef_directive
{
public:
	typedef f1::c_pp::c99::ifdef_directive	super;
	typedef f1::f1pp::ifdef_directive		self;

	explicit ifdef_directive( unsigned flags = 0 ) __noexcept;

	// Properties
public:
	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
};

/*END OF ifdef.hxx*/
