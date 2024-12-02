/*enum.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::enum_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective				super;
	typedef f1::f1pp::enum_directive		self;

	explicit enum_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
protected:
//	const status_t			execute( f1::pp::Iparser & parser, const core::buffer & out_buf );
//	static const status_t	line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );

public:
	struct enum_callback : public f1::f1pp::parser::Ienum_callback {
		virtual const status_t	on_next_item( f1pp::parser & parser ) override;
		virtual const status_t	append_attributes( f1pp::parser & parser, pp::attribute_list & attrs ) override;
		virtual const status_t	set_identifier( f1pp::parser & parser, const pp::string & str ) override;

		pp::string	m_ident;
		core::Tref<pp::attribute_list> m_attrs;
	};
	struct frame : public f1::c_pp::Iframe {
		typedef f1::c_pp::Iframe	super;
		typedef frame				self;

		explicit frame( f1pp::parser & par );

		core::Tref<enumeration>		m_enum;
		f1::pp::raw_output			m_output;
		pp::string_storage			m_storage;
		core::Tref<pp::Ioutput>		m_old_output;
		core::Tref<pp::Istorage>	m_old_storage;
		f1::c_pp::parser::c_flags_t		m_old_c_pp_flags;
		f1::f1pp::parser::f1pp_flags_t	m_old_f1pp_flags;
		enum_callback				m_enum_callback;

		virtual const char *	get_openning_directive_name() const override;
		virtual const char *	get_closing_directive_name() const override;
		virtual const f1::guid_t & get_openning_directive_guid() const override;
		virtual const f1::guid_t & get_closing_directive_guid() const override;
	};
	virtual pp::Iframe *	alloc_idle_frame( f1::pp::Iparser & parser ) override;
};

/*END OF enum.hxx*/
