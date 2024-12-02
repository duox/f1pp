/*parser.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::parser : public f1::c_pp::parser
{
public:
	typedef f1::c_pp::parser	super;
	typedef f1::f1pp::parser	self;

	explicit parser( const core::Iidentifier * ident = nullptr );
			~parser();

	virtual const status_t	_initialize( f1::parser::Isource * source ) override;

	// Properties
public:
	begin_scoped_enum( f1pp_flags_t )
		fl_enum_mode	= 0x0001,	///< enumeration parsing mode
	end_scoped_enum;
	const status_t	modify_f1pp_flags( f1pp_flags_t flags_to_set, f1pp_flags_t flags_to_clear = f1pp_flags_t::null, f1pp_flags_t * prev_flags = nullptr );
	f1pp_flags_t	get_f1pp_flags( f1pp_flags_t mask = f1pp_flags_t::all ) const;
protected:
	f1pp_flags_t	m_f1pp_flags;

	// Directives
public:
	virtual const status_t	parse_directive() override;
	virtual const status_t	parse_identifier() override;

	virtual const status_t	on_execute_directive( pp::Idirective & obj ) override;
	virtual const status_t	on_unknown_directive( const pp::string & ident ) override;

	const pp::attribute *	get_directive_attribute( const char * name, size_t len = size_t(-1) ) const;
	const status_t	find_directive_attribute( core::Tconst_ref<pp::attribute> & attr, const char * name, size_t len = size_t(-1) ) const;
	const status_t	find_directive_attribute( core::Tconst_ref<f1::pp::attribute> & res, const pp::string & name ) const;

	bool	exists_directive_attribute( const f1::guid_t & guid ) const;
	bool	exists_directive_attribute( const pp::string & name ) const;
	bool	exists_directive_attribute( const char * name, size_t len = size_t(-1) ) const;

	f1pp::assignment_directive &	get_assignment_directive();

	virtual const status_t	output_directive( pp::Idirective & drective ) override;

	virtual const pp::attribute_list *	get_directive_attribute_list() const override { return m_directive_attribute_list; }
protected:
	core::Tref<pp::attribute_list>	m_directive_attribute_list;
	core::Tweak_ref<f1pp::assignment_directive>	m_assignment_directive;

public:
	struct multiline_patch_callback : public f1::parser::Iparser::Ipatch_callback {
		explicit multiline_patch_callback( f1pp::parser * parser = nullptr );
		const status_t	initialize( f1pp::macro & mac );

		virtual void	on_patch_initiated( f1::parser::Iparser & parser, void * ) override;
		virtual void	on_patch_completed( f1::parser::Iparser & parser, void * ) override;

		core::Tweak_ref<f1pp::parser>	m_parser;
		core::Tweak_ref<f1pp::macro>	m_macro;
		f1::ns::name					m_locals_ns_name;
		f1::ns::name_space				m_locals_ns;
		core::Tweak_ref<f1::ns::name_space>	m_parent_ns;
	};

	bool	m_in_function;

	// Macros
public:
	using super::create_macro;
	virtual const status_t	create_macro( core::Tref<pp::Imacro> & res,
		const pp::string & ident, const f1::pp::attribute_list * attrs = nullptr,
		const f1::pp::param_list * params = nullptr, const pp::string * value = nullptr, pp::Imacro::macro_flags_t flags = null
		) override;
protected:

	// Attributes
public:
//	const pp::attribute_manager &	get_attribute_manager() const;
//		  pp::attribute_manager &	get_attribute_manager();

	virtual const status_t	parse_attribute_list( core::Tref<pp::attribute_list> & res, const char * & s, const char * end_s );
	virtual const status_t	parse_attribute_list( core::Tref<pp::attribute_list> & res );
	virtual const status_t	parse_attribute( core::Tconst_ref<pp::attribute> & res, const char * & s, const char * end_s );
	virtual const status_t	parse_attribute( core::Tconst_ref<pp::attribute> & res );

//	virtual const status_t	on_unknown_attribute( core::Tconst_ref<pp::Iattribute> & res, const pp::string & ident, const pp::arg_list * args );
protected:
//	pp::attribute_manager	m_attribute_manager;

	// Labels
public:
	const status_t	create_label( core::Tref<label> & res, const pp::string & name );
	const status_t	find_label( core::Tconst_ref<label> & res, const pp::string & name ) const;

	const status_t	wait_for_label( const pp::string & name );
	const status_t	arrived_to_delayed_label( f1pp::label & lab );
	const pp::string &	get_delayed_label_name() const;
protected:
	const_label_list_t	m_label_list;
	pp::string			m_delayed_label;

	// Channels
public:
	const status_t	create_channel( core::Tref<channel> & res,
		const pp::string & name, const pp::string & output_name, const f1::guid_t * output_class = nullptr,
		const core::Iidentifier * ident = nullptr );
	const status_t	append_channel( channel & chan );
	const status_t	remove_channel( channel & chan );
	const status_t	remove_all_channels();

	const status_t	find_channel( core::Tconst_ref<channel> & res, const pp::string & name ) const;

	const status_t	set_active_channel( channel * chan );
	const channel *	get_active_channel() const;
		  channel *	get_active_channel();

	const status_t	push_active_channel();
	const status_t	pop_active_channel();
protected:
	channel_list_t		m_channel_list;		///< list of all defined channels
	core::Tref<channel>	m_active_channel;
	channel_list_t		m_channel_stack;	///< stack of channels used by channel.push/.pop directives
private:
	const status_t	_initialize_channels();
	const status_t	_deinitialize_channels();

	// Enumerations
public:
	const status_t	create_enumeration( core::Tref<enumeration> & res, const char * name, size_t len = size_t(-1) );

	const status_t	append_enumeration( enumeration & en );
	const status_t	remove_enumeration( enumeration & en );
	const status_t	remove_all_enumerations();

	const status_t	get_enumeration( core::Tconst_ref<enumeration> & res, const char * name, size_t len = size_t(-1) );
	const status_t	get_enumeration( core::Tconst_ref<enumeration> & res, const pp::string & name );
	const status_t	find_enumeration( core::Tconst_ref<enumeration> & res, const char * name, size_t len = size_t(-1) );
	const status_t	find_enumeration( core::Tconst_ref<enumeration> & res, const pp::string & name );

	const enumeration_list_t &	get_enumeration_list() const;

	interface F1_SYNTAX_F1PP_API Ienum_callback {
		virtual ~Ienum_callback();
		virtual const status_t	on_next_item( f1pp::parser & parser ) abstract;
		virtual const status_t	append_attributes( f1pp::parser & parser, pp::attribute_list & attrs ) abstract;
		virtual const status_t	set_identifier( f1pp::parser & parser, const pp::string & str ) abstract;
	};
	const status_t	activate_enum_callback( Ienum_callback & callback );
	const status_t	deactivate_enum_callback( Ienum_callback & callback );
protected:
	enumeration_list_t	m_enumerations;

	core::Tarray<unsigned>	m_enum_parenthesis_stack;
	bool	m_was_enum_ident;
	bool	m_ignore_enum_ident;
	core::Tarray<Ienum_callback *>	m_enum_callback_list;
	Ienum_callback *	m_active_enum_callback;

	// Type support
public:
	const status_t	parse_type( pp::variant::type_t & type_id, pp::string * type_name_ptr = nullptr );
	const status_t	parse_type( pp::variant::type_t & type_id, pp::string * type_name_ptr, const char * & s, const char * end_s );
	pp::variant::type_t	get_type_id( const char * name, size_t len = size_t(-1) ) const;
	pp::variant::type_t	get_type_id( const pp::string & type_name ) const;
	const char * get_type_name( pp::variant::type_t type_id, size_t * len = nullptr ) const;
protected:

	// Includes
public:
	virtual  const status_t on_source_included( f1::parser::Isource & src ) noexcept override;
	virtual  const status_t on_source_enter( f1::parser::Isource & src ) noexcept override;
	virtual  const status_t on_source_leave( f1::parser::Isource & src ) noexcept override;

	struct source_context
	{
		core::Tref<f1::parser::Isource>		m_source;			///< corresponding source
		core::Tref<f1::ns::name_space>		m_ns;				///< local namespace
		core::Tref<f1::ns::name_space>		m_ns_previous;		///< previous namespace
		core::Tref<f1::pp::arg_list>		m_args;				///< active list of arguments specified in the #include directive
		size_t	m_next_arg_index;								///< index of the next argument to be fetchd by the #param directive
		f1::pp::raw_output					m_output;
		core::Tref<pp::Ioutput>				m_output_previous;
		core::Tref<f1::pp::Istorage>		m_storage_previous;	///< previously used storage
		f1::pp::string_storage				m_storage;			///< storage used for storing parsed data
		core::Tref<f1::pp::Imacro>			m_macro;			///< storage used to store results of inclusion
		f1::pp::macro_list_t				m_params;			///< all paraneter macros defined with #param
		f1::pp::macro_list_t			m_saved_param_macros;	///< macros shadowed by the parameters
		bool	m_target_macro_append_mode;						///< append value to the target macro
	};
	const source_context *	get_top_source_context() const;
		  source_context *	get_top_source_context();

		  source_context *	create_source_context( f1::parser::Isource & src );

	const source_context *	get_source_context( f1::parser::Isource & src ) const;
		  source_context *	get_source_context( f1::parser::Isource & src );

	const core::Tarray<source_context> &	get_sources_context_list() const;
protected:
	core::Tarray<source_context>	m_sources;
	core::Tarray<source_context *>	m_source_stack;

	// Operations
public:
	bool	set_newline( bool newline = true ) override;
	bool	on_char( unsigned ch ) override;
	const status_t	finish_parse() override;

	using super::parse_definition_identifier;
	virtual bool	parse_definition_identifier( pp::string & buf, const char *& s, const char * end_s, f1::parser::Iparser * parser = nullptr ) override;
	using super::parse_directive_identifier;
	virtual bool	parse_directive_identifier( pp::string & buf, const char *& s, const char * end_s, f1::parser::Iparser * parser = nullptr ) override;
	using super::parse_macro_identifier;
	virtual bool	parse_macro_identifier( pp::string & buf, const char *& s, const char * end_s, f1::parser::Iparser * parser = nullptr ) override;

	virtual const status_t	parse_identifier_list( core::Tarray<pp::string> & res, const char *& s, const char * end_s );
	virtual const status_t	parse_identifier_list( core::Tarray<pp::string> & res );
	virtual const status_t	parse_identifier_and_string_list( core::Tarray<pp::string> & res, const char *& s, const char * end_s );
	virtual const status_t	parse_identifier_and_string_list( core::Tarray<pp::string> & res );

	virtual const status_t	parse_identifier_and_type( pp::string & ident, pp::variant::type_t & ty );
	virtual const status_t	parse_identifier_and_type( core::Tref<pp::Imacro> & mac, bool * append = nullptr );
	virtual const status_t	parse_identifier_and_type( pp::string & ident, pp::variant::type_t & ty, const char * & s, const char * end_s );
	virtual const status_t	parse_identifier_and_type( core::Tref<pp::Imacro> & mac, const char * & s, const char * end_s, bool * append = nullptr );

	virtual const status_t	parse_value( pp::variant & value, const char * & s, const char * end_s, unsigned flags = 0 ) override;

	virtual const status_t	parse_macro_expression( bool & result, unsigned flags = 0 );
	virtual const status_t	parse_macro_expression( bool & result, const char * & s, const char * end_s, unsigned flags = 0 );
	enum boolean_op_t;
	virtual const status_t	parse_macro_subexpression(
		bool & left_operand, boolean_op_t & op,
		const char * & s, const char * end_s, unsigned flags );
	virtual const status_t parse_operation( boolean_op_t & result, const char * &s, const char * end_s, unsigned flags );
	virtual const status_t parse_operand( bool & result, const char * &s, const char * end_s, unsigned flags );
	virtual const status_t evaluate( bool & result, bool a, boolean_op_t op, bool b, unsigned flags );

	static const status_t	execute_range( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );

	virtual int		get_end_quote( int ch ) const override;

	const status_t	make_unique_identifier( core::string & buf ) const;
protected:
	static const status_t _esc_control( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );
	const status_t on_esc_control( pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );

protected:
public:

/*	virtual const status_t	load_default_escapes();
	virtual const status_t	set_esc_char_value( int ch, int val );
	typedef const status_t (*esc_char_callback)( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s );
	virtual const status_t	set_esc_char_callback( int ch, esc_char_callback fn );
	virtual const status_t	parse_esc_char( f1::pp::string & buf, const char *& s, const char * end_s ) const;*/
//	virtual const status_t	parse_escape( intmax_t & buf, size_t & nbytes );
protected:
};

/*END OF parser.hxx*/
