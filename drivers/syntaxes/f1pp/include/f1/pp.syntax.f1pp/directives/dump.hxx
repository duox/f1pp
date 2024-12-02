/*dump.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::dump_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective			super;
	typedef f1::f1pp::dump_directive	self;

	explicit dump_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;
protected:

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
protected:
	const status_t	dump_object( f1::f1pp::parser & f1par,
		const ns::name_space & ns_parent, const pp::string & ident,
		pp::Imacro * target_macro, bool append ) const;
	const status_t	dump_macro( f1::f1pp::parser & f1par,
		const ns::name_space & ns_parent, const pp::string & ident,
		pp::Imacro * target_macro, bool append ) const;
	const status_t	dump_macro( f1::f1pp::parser & f1par,
		const pp::Imacro & macro,
		pp::Imacro * target_macro, bool append ) const;
	const status_t	dump_namespace( f1::f1pp::parser & f1par,
		const ns::name_space & ns_parent, const pp::string & ident,
		pp::Imacro * target_macro, bool append ) const;
	const status_t	dump_namespace( f1::f1pp::parser & f1par,
		const ns::name_space & ns,
		pp::Imacro * target_macro, bool append ) const;
};

/*END OF dump.hxx*/
