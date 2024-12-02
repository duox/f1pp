/*exec.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::exec_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective			super;
	typedef f1::f1pp::exec_directive	self;

	explicit exec_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	//static const status_t	get_macro_value( pp::Imacro & mac,		 pp::variant & var, pp::variant::type_t ty );
	//static const status_t	set_macro_value( pp::Imacro & mac, const pp::variant & var );
protected:
	core::Tweak_ref<pp::Imacro>		m_macro;
	bool	m_soft;
	bool	m_raw;
	bool	m_append;

	//static f1::guid_t	m_raw_attribute_class_guid;
	//static f1::guid_t	m_soft_attribute_class_guid;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
	const status_t			execute( pp::Iparser & parser, const pp::string & out_buf );

	struct frame : public f1::c_pp::Iframe {
		typedef f1::c_pp::Iframe	super;
		typedef frame				self;

		explicit frame( f1pp::parser & par, exec_directive & drct );

		core::Tref<exec_directive>	m_exec_directive;
		f1::pp::raw_output			m_output;
		pp::string_storage			m_storage;
		core::Tref<pp::Ioutput>		m_old_output;
		core::Tref<pp::Istorage>	m_old_storage;
		f1::c_pp::parser::c_flags_t	m_old_c_flags;

		virtual const char *	get_openning_directive_name() const override;
		virtual const char *	get_closing_directive_name() const override;
		virtual const f1::guid_t & get_openning_directive_guid() const override;
		virtual const f1::guid_t & get_closing_directive_guid() const override;
	};
	virtual pp::Iframe *	alloc_idle_frame( f1::pp::Iparser & parser ) override;
protected:
	const status_t			execute( f1::pp::Iparser & parser, const core::buffer & out_buf );
	static const status_t	line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );
public:

protected:
	virtual const status_t	execute_commands( f1::f1pp::parser & f1par, const pp::string & cmd );
	virtual const status_t	create_process_and_wait( f1::f1pp::parser & f1par, const char * executable, const char * cmdl, pp::string & out_buf );
};

/*END OF exec.hxx*/
