/*if.hxx*/

class F1_PP_API f1::c_pp::c99::if_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective	super;
	typedef f1::c_pp::c99::if_directive	self;

	explicit if_directive( unsigned flags = 0 ) __noexcept;

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	class frame : public c_pp::Iframe {
	public:
		typedef c_pp::Iframe	super;
		typedef frame			self;

		frame( c_pp::parser & parser, bool allow_parse );
		virtual void	delete_self() override;
		virtual const status_t	enter() override;
		virtual const status_t	leave() override;
		virtual const char *	get_openning_directive_name() const override;
		virtual const char *	get_closing_directive_name() const override;
		virtual const f1::guid_t & get_openning_directive_guid() const override;
		virtual const f1::guid_t & get_closing_directive_guid() const override;
	public:
		bool m_prev_allow_parse;
		bool m_allow_parse;
		bool m_was_else;
		bool m_if_executed;
	};
	virtual pp::Iframe *	alloc_idle_frame( f1::pp::Iparser & parser ) override;
	virtual const status_t	parse( pp::Iparser & parser ) override;

	const status_t	execute( f1::pp::Iparser & parser, const core::buffer & buf );
	static const status_t	line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );
};

/*END OF if.hxx*/
