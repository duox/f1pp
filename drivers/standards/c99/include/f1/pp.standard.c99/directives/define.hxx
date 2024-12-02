/*define.hxx*/

class F1_PP_API f1::c_pp::c99::define_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective				super;
	typedef f1::c_pp::c99::define_directive	self;

	explicit define_directive( unsigned flags = 0 ) __noexcept;

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;

	// Hooks
public:
	virtual const status_t	fetch_macro_flags(
		c_pp::parser & parser, pp::Imacro::macro_flags_t & flags,
		const pp::string & ident, const pp::param_list * params, const pp::string & value );
	virtual const status_t	can_create_macro( bool & result,
		c_pp::parser & parser,
		const f1::pp::string & ident, const f1::pp::attribute_list * attrs,
		const f1::pp::param_list * params, const pp::string * value, pp::Imacro::macro_flags_t flags );
	virtual const status_t	find_macro( core::Tref<pp::Imacro> & res,
		c_pp::parser & parser,
		const core::stringslice & ident, const pp::param_list * params,
		pp::Iparser::find_macro_flags_t flags, core::Tref<f1::ns::name> * nm = nullptr );
	virtual const status_t	create_macro( core::Tref<pp::Imacro> & res,
		c_pp::parser & parser,
		const pp::string & ident, const pp::attribute_list * attrs, const pp::param_list * params, const pp::string & value, pp::Imacro::macro_flags_t flags );
};

/*END OF define.hxx*/
