/*embed.hxx*/

class F1_PP_API f1::f1pp::embed_directive : public f1::c_pp::c23::embed_directive
{
public:
	typedef f1::c_pp::c23::embed_directive	super;
	typedef f1::f1pp::embed_directive		self;

	explicit embed_directive( unsigned flags = 0 );

	const status_t	initialize() override;

	// Properties
public:
	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	execute( f1::pp::Iparser & parser, const pp::string & out_buf ) override;

	// Parameters
protected:
	class column_count_parameter : public Iparameter {
		virtual const char *	get_name() const override { return "f1pp::column_count"; }
		virtual const char *	get_alt_name() const override { return "f1pp::__column_count__"; }
		virtual const status_t	parse_argument( f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const override;
	} m_column_count_parameter;
	class element_byte_size_parameter : public Iparameter {
		virtual const char *	get_name() const override { return "f1pp::element_byte_size"; }
		virtual const char *	get_alt_name() const override { return "f1pp::__element_byte_size__"; }
		virtual const status_t	parse_argument( f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const override;
	} m_element_byte_size_parameter;
	class element_prefix_parameter : public Iparameter {
		virtual const char *	get_name() const override { return "f1pp::element_prefix"; }
		virtual const char *	get_alt_name() const override { return "f1pp::__element_prefix__"; }
		virtual const status_t	parse_argument( f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const override;
	} m_element_prefix_parameter;
	class element_suffix_parameter : public Iparameter {
		virtual const char *	get_name() const override { return "f1pp::element_suffix"; }
		virtual const char *	get_alt_name() const override { return "f1pp::__element_suffix__"; }
		virtual const status_t	parse_argument( f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const override;
	} m_element_suffix_parameter;
	class element_separator_parameter : public Iparameter {
		virtual const char *	get_name() const override { return "f1pp::element_separator"; }
		virtual const char *	get_alt_name() const override { return "f1pp::__element_separator__"; }
		virtual const status_t	parse_argument( f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const override;
	} m_element_separator_parameter;
	class offset_parameter : public Iparameter {
		virtual const char *	get_name() const override { return "f1pp::offset"; }
		virtual const char *	get_alt_name() const override { return "f1pp::__offset__"; }
		virtual const status_t	parse_argument( f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const override;
	} m_offset_parameter;
	class radix_parameter : public Iparameter {
		virtual const char *	get_name() const override { return "f1pp::radix"; }
		virtual const char *	get_alt_name() const override { return "f1pp::__radix__"; }
		virtual const status_t	parse_argument( f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const override;
	} m_radix_parameter;
	class timeout_parameter : public Iparameter {
		virtual const char *	get_name() const override { return "f1pp::timeout"; }
		virtual const char *	get_alt_name() const override { return "f1pp::__timeout__"; }
		virtual const status_t	parse_argument( f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const override;
	} m_timeout_parameter;
};

/*END OF embed.hxx*/
