/*undef.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::undef_directive : public f1::c_pp::c99::undef_directive
{
public:
	typedef f1::c_pp::c99::undef_directive		super;
	typedef f1::f1pp::undef_directive			self;

	explicit undef_directive( unsigned flags = 0 );

	// Properties
public:
	virtual unsigned	get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
};

/*END OF undef.hxx*/
