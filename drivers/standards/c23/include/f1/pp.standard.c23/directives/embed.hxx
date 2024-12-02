/*embed.hxx*/

class F1_PP_API f1::c_pp::c23::embed_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective				super;
	typedef f1::c_pp::c23::embed_directive	self;

	static self *	m_instance;

	explicit embed_directive( unsigned flags = 0 );
			~embed_directive();

	const status_t	initialize() override;

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
	virtual const status_t	execute( f1::pp::Iparser & parser, const pp::string & out_buf );
protected:
	static	const status_t	line_capture_callback( core::object * embed, f1::pp::Iparser & parser, const core::buffer & buf );

public:
	struct resource_state {
		resource_state();

		pp::string	m_file_name;
		c_pp::parser::include_flags_t	m_include_flags;

		core::string	m_prefix;
		core::string	m_suffix;
		core::string	m_if_empty;
		core::string	m_element_separator;
		core::string	m_element_prefix;
		core::string	m_element_suffix;

		uint64_t	m_offset;				///< offset inside the resource
		uint64_t	m_length;				///< maximum length of the resource

		unsigned	m_buffer_size;			///< size of the buffer per iteration
		uint32_t	m_timeout;				///< operation timeout
		uint8_t		m_radix;				///< radix of numerics
		uint8_t		m_bytes_per_element;	///< number of bytes per element in the file data

		unsigned	m_columns;				///< number of columns
	};
	virtual const status_t	execute( f1::c_pp::parser & parser, const resource_state & state );
			const status_t	parse_parameter_list( resource_state & state, c_pp::parser & parser, const char * & s, const char * end_s );
			const status_t	parse_parameter_list( resource_state & state, c_pp::parser & parser );

	// Parameters
public:

	interface Iparameter : public core::object
	{
		static const unsigned	cap_no_parameter	= 0x0001;
		virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const;

		virtual const char *	get_name() const abstract;
		virtual const char *	get_alt_name() const abstract;

		virtual const status_t	parse_argument( f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const abstract;
	};

	const status_t	append_parameter( Iparameter & param );
	const status_t	remove_parameter( Iparameter & param );
	const status_t	remove_parameter( const char * name, size_t len = size_t(-1) );
	const status_t	remove_all_parameters();

	const status_t	find_parameter( core::Tconst_ref<Iparameter> & param, const pp::string & name ) const;
protected:
	struct parameter_desc {
		core::Tref<Iparameter>	m_parameter;
		core::string			m_identifier;
	};
	typedef core::Tarray<parameter_desc>	parameter_list_t;	// TODO: make a hash map
	parameter_list_t	m_parameter_list;
	static Iparameter *	m_intrinsic_parameters[];

	class if_empty_parameter : public Iparameter {
		virtual const char *	get_name() const override { return "if_empty"; }
		virtual const char *	get_alt_name() const override { return "__if_empty__"; }
		virtual const status_t	parse_argument( f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const override;
	} m_if_empty_parameter;
	class limit_parameter : public Iparameter {
		virtual const char *	get_name() const override { return "limit"; }
		virtual const char *	get_alt_name() const override { return "__limit__"; }
		virtual const status_t	parse_argument( f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const override;
	} m_limit_parameter;
	class prefix_parameter : public Iparameter {
		virtual const char *	get_name() const override { return "prefix"; }
		virtual const char *	get_alt_name() const override { return "__prefix__"; }
		virtual const status_t	parse_argument( f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const override;
	} m_prefix_parameter;
	class suffix_parameter : public Iparameter {
		virtual const char *	get_name() const override { return "suffix"; }
		virtual const char *	get_alt_name() const override { return "__suffix__"; }
		virtual const status_t	parse_argument( f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const override;
	} m_suffix_parameter;
};

/*END OF embed.hxx*/
