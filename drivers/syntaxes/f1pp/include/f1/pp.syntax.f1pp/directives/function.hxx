/*function.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::function_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective		super;
	typedef f1::f1pp::function_directive	self;

	explicit function_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;

	struct frame : public f1::f1pp::Idefinition_frame {
		typedef f1::f1pp::Idefinition_frame	super;
		typedef frame						self;

		explicit frame( f1pp::parser & par );

		pp::string					m_ident;
		core::Tref<pp::param_list>	m_param_list;
		core::Tref<f1pp::macro>		m_existing_func;

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
};

/*END OF function.hxx*/
