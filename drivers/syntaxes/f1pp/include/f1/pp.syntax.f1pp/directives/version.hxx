/*version.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::version_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective			super;
	typedef f1::f1pp::version_directive	self;

	explicit version_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned	get_caps( unsigned mask ) const __noexcept override;

	const status_t	set_fail_if_not_matched( bool val );
	bool			get_fail_if_not_matched() const;
protected:
	bool	m_fail_if_not_matched;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
protected:
	static const status_t	line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );
	virtual const status_t	execute( pp::Iparser & parser, const pp::string & buf );
};

/*END OF version.hxx*/
