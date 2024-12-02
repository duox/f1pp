/*define.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::define_directive : public f1::c_pp::c99::define_directive
{
public:
	typedef f1::c_pp::c99::define_directive		super;
	typedef f1::f1pp::define_directive			self;

	explicit define_directive( unsigned flags = 0 );

	// Properties
public:
	virtual unsigned	get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	fetch_macro_flags(
		c_pp::parser & parser, pp::Imacro::macro_flags_t & flags,
		const pp::string & ident, const pp::param_list * params, const pp::string & value ) override;
	virtual const status_t	create_macro( core::Tref<pp::Imacro> & res,
		c_pp::parser & parser,
		const pp::string & ident, const pp::attribute_list * attrs, const pp::param_list * params, const pp::string & value, pp::Imacro::macro_flags_t flags ) override;
};

/*END OF macro.hxx*/
