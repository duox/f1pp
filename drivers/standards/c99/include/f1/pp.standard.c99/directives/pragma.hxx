/*pragma.hxx*/

class F1_PP_API f1::c_pp::c99::pragma_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective		super;
	typedef f1::c_pp::c99::pragma_directive	self;

	explicit pragma_directive( unsigned flags = 0 ) __noexcept;

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
			const status_t	execute( pp::Iparser & parser,
				const pp::string & name, const pp::string & args, c_pp::Ipragma * target_pragma );

	const status_t parse_pragma( c_pp::parser & cpp_par,
		const char *& s, const char * const end_s,
		pp::string & target_ident, core::Tref<c_pp::Ipragma> & target_prg );
	const status_t parse_pragma( c_pp::parser & cpp_par,
		pp::string & target_ident, core::Tref<c_pp::Ipragma> & target_prg );
protected:
	pp::string	m_pragma_name;
	core::Tref<c_pp::Ipragma> m_target_pragma;
	static const status_t	line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );
};

/*END OF pragma.hxx*/
