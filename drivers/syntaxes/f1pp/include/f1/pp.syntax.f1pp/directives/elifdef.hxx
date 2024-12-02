/*elifdef.hxx*/

class F1_PP_API f1::f1pp::elifdef_directive : public f1::c_pp::cpp23::elifdef_directive
{
public:
	typedef f1::c_pp::cpp23::elifdef_directive	super;
	typedef f1::f1pp::elifdef_directive			self;

	explicit elifdef_directive( unsigned flags = 0 ) __noexcept;

	// Properties
public:
	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
};

/*END OF elifdef.hxx*/
