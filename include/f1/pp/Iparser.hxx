/*Iparser.hxx*/

interface F1_PP_API f1::pp::Iparser : public f1::pp::object
{
	typedef f1::pp::object		super;
	typedef f1::pp::Iparser		self;

	explicit Iparser( const core::Iidentifier * ident = nullptr ) __noexcept;
			~Iparser() __noexcept;

	virtual void	addref() NOEXCEPT override;

	virtual const status_t	initialize( pp::context & context, pp::Isyntax & syntax, pp::Ioutput & outp, pp::Istorage & stg, parser::Isource & source );
	virtual const status_t	initialize( pp::context & context, pp::Isyntax & syntax, pp::Ioutput & outp, pp::Istorage & stg, parser::Iparser * parser, const char * base_source );
protected:
	virtual const status_t	_initialize( parser::Isource * source );
	using super::initialize;

	// Parameters
public:
	begin_scoped_enum( parser_flags_t )
		fl_expression_mode				= 0x0001,	//< parser currently parses an expression, so expression-only macros are available
		fl_global_events				= 0x0002,	///< allow global events
		fl_dump_macros_on_error			= 0x0004,	///< on error, dump all macros
		fl_dump_macros_on_warning		= 0x0008,	///< on warning, dump all macros
		fl_dump_macro_stack_on_error	= 0x0010,	///< dump stack of macro expansions on error
		fl_dump_macro_stack_on_warning	= 0x0020,	///< dump stack of macro expansions on warning
	end_scoped_enum;
	virtual const status_t	modify_parser_flags( parser_flags_t flags_to_set, parser_flags_t flags_to_clear = parser_flags_t::null, parser_flags_t * prev_flags = nullptr );
	parser_flags_t	get_parser_flags( parser_flags_t mask = parser_flags_t::all ) const;

	const f1::pp::context *		get_context() const;
		  f1::pp::context *		get_context();

	const f1::pp::Isyntax *		get_syntax() const;
		  f1::pp::Isyntax *		get_syntax();

//	const f1::pp::Istandard *	get_standard() const;
//	const status_t	set_standard( const f1::pp::Istandard & standard );

	const f1::pp::Ioutput *		get_base_output() const;
	const f1::pp::Ioutput *		get_output() const;
		  f1::pp::Ioutput *		get_output();
	virtual const status_t	set_output( f1::pp::Ioutput * outp, core::Tref<pp::Ioutput> * old_outp = nullptr );

	const f1::pp::Istorage *	get_storage() const;
		  f1::pp::Istorage *	get_storage();
	virtual const status_t	set_storage( f1::pp::Istorage * outp, core::Tref<pp::Istorage> * old_outp = nullptr );

	const f1::parser::context &	get_parser_context() const;
		  f1::parser::context &	get_parser_context();
	const status_t	set_parser( f1::parser::Iparser & parser );
	const f1::parser::Iparser *	get_parser() const;
		  f1::parser::Iparser *	get_parser();

	const fs::context &			get_fs_context() const;
		  fs::context &			get_fs_context();
	const fs::path &			get_base_directory() const;

	const msgout::context &		get_msgout_context() const;
		  msgout::context &		get_msgout_context();

	virtual const status_t	set_parameter( const f1::guid_t & param_guid, const core::variant & val );

	const status_t	set_input_encoding( const nls::Iencoding * enc );
	const nls::Iencoding *	get_input_encoding() const;
	const status_t	set_output_encoding( const nls::Iencoding * enc );
	const nls::Iencoding *	get_output_encoding() const;

	parser::lineno_t	get_global_line() const;
protected:
	core::Tweak_ref<pp::context>	m_context;
	core::Tweak_ref<pp::Isyntax>	m_syntax;
	core::Tref<pp::Ioutput>		m_base_output;
	core::Tref<pp::Ioutput>		m_output;
	core::Tref<pp::Istorage>	m_storage;

	core::Tref<parser::Iparser>	m_parser;

//	f1::core::Tconst_weak_ref<pp::Istandard>	m_standard;

	fs::path			m_base_path;

//	msgout::context		m_msgout_context;

	parser_flags_t		m_parser_flags;
	parser::lineno_t	m_global_line;	// TODO: remove

	// Limits
public:
	intmax_t		get_limit_value( limit_t lim ) const;
	const status_t	set_limit_value( limit_t lim, intmax_t value );
	const status_t	set_limit_value_min( limit_t lim, intmax_t value );	// set limit if current value is greater
	const status_t	set_limit_value_max( limit_t lim, intmax_t value );	// set limit if current value is less
	bool			check_limit_value_min( limit_t lim, intmax_t value ) const;// limit value must be greater or equal than the passed value
	bool			check_limit_value_max( limit_t lim, intmax_t value ) const;// limit value must be less or equal than the passed value
protected:
	intmax_t	m_limit[limit_t::li_count];

	// Character table
public:
	static const unsigned fl_char_break			= 0x01;
	static const unsigned fl_char_flags_mask	= 0x0f;
	const status_t	modify_char_flags( int ch, unsigned flags_to_set, unsigned flags_to_clear = 0, unsigned * prev_flags = nullptr );
	unsigned	get_char_flags( int ch, unsigned mask = unsigned(-1) ) const;
protected:
	uint8_t	m_char_table[256];

	// Messages
public:
	const status_t	message( f1::msgout::msgid_t msg, ... );
	const status_t	message( const f1::parser::Iparser::location & loc, f1::msgout::msgid_t msg, ... );
	const status_t	message( const char * file, f1::parser::lineno_t line, f1::msgout::msgid_t msg, ... );
	virtual const status_t	message( const char * file, f1::parser::lineno_t line, f1::msgout::msgid_t msg, va_list ap );

	bool			is_message_printed_at_line( f1::parser::lineno_t line, f1::msgout::msgid_t msg );
	const char *	get_message_file_name( const char * file = nullcstr ) const;

/*	unsigned	get_warning_maximum_count() const;
	unsigned	get_error_maximum_count() const;
	const status_t	set_warning_maximum_count( unsigned count );
	const status_t	set_error_maximum_count( unsigned count );*/

	unsigned	get_common_error_count() const;
protected:
	f1::parser::lineno_t				m_last_message_line;
	core::Tarray<f1::msgout::msgid_t>	m_line_messages;

	struct F1_PP_API event_handler : public f1::parser::Ievent_handler {
		virtual const status_t	invoke( f1::parser::event_type_t type_mask, void * handler_param, void * event_param, f1::parser::Iparser & parser ) override;
	};
	event_handler	m_event_handler;
	/*struct filename_change_callback : public core::Icallback {
		core::Tweak_ref<pp::Iparser>	m_parser;
		void	invoke( core::Icallback_message * ) override {
			if( nullptr != m_parser )	m_parser->on_filename_changed();
		}
	} m_filename_change_callback;*/
	virtual void on_filename_changed();
	core::string	m_full_current_file_name;
	/*struct lineno_change_callback : public core::Icallback {
		core::Tweak_ref<pp::Iparser>	m_parser;
		void	invoke( core::Icallback_message * ) override {
			if( nullptr != m_parser )	m_parser->on_lineno_changed();
		}
	} m_lineno_change_callback;*/
	virtual void			on_lineno_changed();
	virtual const status_t	on_line_changed( f1::parser::lineno_t line );

//	unsigned	m_warning_maximum_count;
//	unsigned	m_error_maximum_count;

	void _check_message_count( intmax_t maximum_count, msgout::severity_t severity, unsigned msgid );

	// Includes
public:
	const status_t	set_system_include_base_directory( const core::string & base_dir );
	const status_t	set_system_include_base_directory( const core::stringslice & base_dir );
	const status_t	set_system_include_base_directory( const core::const_string & base_dir );
	const core::string &	get_system_include_base_directory() const;

	begin_scoped_enum( include_path_flags_t )
		fl_system		= 0x0001,	//< use the path during system search only
		fl_user			= 0x0002,	//< use the path during user search only
		fl_common		= 0x0003,	//< use the path during system and user searches
//		fl_deprecated	= 0x0004,	//< TODO: display warning when files from this directory are fetched (will require storing metadata in fs::search_directory_list)
	end_scoped_enum;

	const status_t	modify_include_path( include_path_flags_t flags, const char * path, size_t len, bool remove = false );
	const status_t	append_include_path( include_path_flags_t flags, const char * path, size_t len = size_t(-1) );
	const status_t	append_include_path( include_path_flags_t flags, const core::string & path );
	const status_t	append_include_path( include_path_flags_t flags, const pp::string & path );
	const status_t	remove_include_path( include_path_flags_t flags, const char * path, size_t len = size_t(-1) );
	const status_t	remove_include_path( include_path_flags_t flags, const core::string & path );
	const status_t	remove_include_path( include_path_flags_t flags, const pp::string & path );
	const status_t	remove_all_include_paths( include_path_flags_t flags );

	const status_t	resolve_source_relative_file_path( core::string & res, const pp::string & file_name ) const;
	const status_t	resolve_source_relative_file_path( core::string & res, const core::stringslice & file_name ) const;
	const status_t	resolve_source_relative_file_path( core::string & res, const char * file_name, size_t len ) const;

	const fs::search_dir_list &	get_search_dir_list( include_path_flags_t flags ) const;
		  fs::search_dir_list &	get_search_dir_list( include_path_flags_t flags );

	begin_scoped_enum( dump_flags_t )
		fl_dump_aliases		= 0x0001,
	end_scoped_enum;
	virtual const status_t	dump_includes( msgout::context & ctx, dump_flags_t flags = dump_flags_t::null ) const;
	virtual const status_t	dump_include( msgout::context & ctx, f1::parser::Isource & src, dump_flags_t flags = dump_flags_t::null ) const;
	virtual const status_t	dump_include_info( msgout::context & ctx, f1::parser::Isource & src, dump_flags_t flags = dump_flags_t::null ) const;
protected:
	fs::search_dir_list	m_sys_search_dir_list;	// TODO: replace all direct references to this with the call to get_include_dir_list
	fs::search_dir_list	m_usr_search_dir_list;	// TODO: replace all direct references to this with the call to get_include_dir_list
	core::string		m_system_include_base_directory;

	// Commands
public:
	interface F1_PP_API Icommand : public f1::core::object {
		virtual const status_t	execute( pp::Iparser & parser ) abstract;
	};

	struct F1_PP_API cmd_modify_flags : public Icommand {
		explicit cmd_modify_flags( parser_flags_t flags_to_set, parser_flags_t flags_to_clear = null );
		virtual const status_t	execute( pp::Iparser & parser ) override;
		parser_flags_t	m_flags_to_set, m_flags_to_clear;
	};
	struct F1_PP_API cmd_append_standard : public Icommand {
		explicit cmd_append_standard( Istandard & std );
		virtual const status_t	execute( pp::Iparser & parser ) override;
		const status_t	append_standard( Istandard & std );
	protected:
		standard_list_t	m_standard_list;
	};
	struct F1_PP_API cmd_remove_standard : public Icommand {
		explicit cmd_remove_standard( Istandard & std );
		virtual const status_t	execute( pp::Iparser & parser ) override;
		const status_t	append_standard( Istandard & std );
	protected:
		standard_list_t	m_standard_list;
	};
	struct F1_PP_API cmd_include_path : public Icommand {
		explicit cmd_include_path( include_path_flags_t flags, const f1::core::const_string & file_names, bool remove = false );
		virtual const status_t	execute( pp::Iparser & parser ) override;
				const status_t	execute( pp::Iparser & parser, const core::string & list, bool remove_paths );
		const status_t	append_include_path( const f1::core::const_string & file_names );
		const status_t	remove_include_path( const f1::core::const_string & file_names );
		const status_t	append_path( core::string & includes, const f1::core::const_string & file_names );
	protected:
		include_path_flags_t		m_include_flags;
		core::string		m_includes_to_add;
		core::string		m_includes_to_remove;
	};
	struct F1_PP_API cmd_set_directive_flags : public Icommand {
		explicit cmd_set_directive_flags( const core::stringslice & name, unsigned flags_to_set, unsigned flags_to_clear );
		virtual const status_t	execute( pp::Iparser & parser ) override;
		const status_t	append_directive_flags( const core::stringslice & name, unsigned flags_to_set, unsigned flags_to_clear );
	protected:
		struct directive_state {
			core::string	m_directive_name;
			unsigned		m_flags_to_clear;
			unsigned		m_flags_to_set;
		};
		core::Tarray<directive_state>	m_directive_states;
	};
	struct F1_PP_API cmd_system_include_base : public Icommand {
		explicit cmd_system_include_base( const core::const_string & base_dir );
		virtual const status_t	execute( pp::Iparser & parser ) override;
	protected:
		core::string	m_base_dir;
	};
	struct F1_PP_API cm_set_output_filename : public Icommand {
		explicit cm_set_output_filename ( const core::const_string & base_dir );
		virtual const status_t	execute( pp::Iparser & parser ) override;
	protected:
	};
protected:

	// Operations
public:
	virtual const status_t	parse() abstract;
	virtual const status_t	abort_parser() abstract;

	interface F1_PP_API Iparser_hooks {
		virtual ~Iparser_hooks() noexcept {}
		virtual bool			is_eof( pp::Iparser & parser ) const abstract;
		virtual const status_t	on_esc_char( pp::Iparser & parser ) abstract;
		virtual const status_t	output( pp::Iparser & parser, const char * start, size_t len ) abstract;
		virtual const status_t	on_line_changed( pp::Iparser & parser, int line ) abstract;
	};
	const status_t	set_parser_hooks( Iparser_hooks & intf );
	Iparser_hooks *	get_parser_hooks() const;

	class F1_PP_API root_parser_hooks : public Iparser_hooks {
	public:
		virtual bool	is_eof( pp::Iparser & parser ) const override;
		virtual const status_t	on_esc_char( pp::Iparser & parser ) override;
		virtual const status_t	output( pp::Iparser & parser, const char * start, size_t len ) override;
		virtual const status_t	on_line_changed( pp::Iparser & parser, int line ) override;
	};
	class F1_PP_API line_parser_hooks : public root_parser_hooks {
	public:
		virtual bool	is_eof( pp::Iparser & parser ) const override;
		virtual const status_t	output( pp::Iparser & parser, const char * start, size_t len ) override;
		pp::string	m_buffer;
	};

	virtual const status_t	output( const char * str, size_t len = size_t(-1) );
	virtual const status_t	output( const pp::string & str );
	virtual const status_t	output( const core::string & str );
	virtual const status_t	enable_output( bool enable = true );

protected:
	Iparser_hooks *	m_parser_hooks;
	unsigned	m_disable_output_count;

	// Execution stack
public:
	const status_t	enter_directive( Iframe & ectx );
	const status_t	leave_directive();
	virtual const Iframe *	get_directive_frame() const;
	virtual Iframe *	get_directive_frame();
	virtual Iframe *	get_directive_frame( const f1::guid_t & openning_guid );
	template<typename T> typename T::frame *	get_directive_frame()
	{
		return type_cast<T::frame *>( get_directive_frame( T::m_guid ) );
	}
	//const status_t	check_all_directives_closed();
	
	const status_t	set_frame_stack( const core::Tarray<Iframe *> & stack );
	const core::Tarray<Iframe *> &	get_frame_stack() const { return m_frame_stack; }
		  core::Tarray<Iframe *> &	get_frame_stack()		{ return m_frame_stack; }
protected:
	core::Tarray<Iframe *>	m_frame_stack;

	// Directives
public:
	const directive_list *	get_directive_list() const;
		  directive_list *	get_directive_list();
	virtual const status_t	find_directive( core::Tref<Idirective> & res, const f1::guid_t & guid ) const;
	static const unsigned	fl_compare_no_case	= directive_list::fl_compare_no_case;
	virtual const status_t	find_directive( core::Tref<Idirective> & res, unsigned flags, const char * name, size_t length = size_t(-1) ) const;
	virtual const status_t	find_directive( core::Tref<Idirective> & res, unsigned flags, const pp::string & name ) const;
	template<typename T>	const status_t	find_directive( core::Tconst_ref<T> & res ) const;

	bool	is_directive_available( const pp::Idirective & directive ) const;

	virtual const status_t	on_execute_directive( pp::Idirective & obj );
	virtual const status_t	on_unknown_directive( const pp::string & ident );

	virtual const char *	get_directive_start() const abstract;
	virtual const char *	get_directive_name_start() const abstract;
protected:
	directive_list		m_directive_list;

	// Macros
public:
	const status_t	create_macro( core::Tref<pp::Imacro> & res,
		const pp::string & ident, const pp::string & value, Imacro::macro_flags_t flags = null
		);
	const status_t	create_macro( core::Tref<pp::Imacro> & res,
		const pp::string & ident, const f1::pp::attribute_list * attrs,
		const f1::pp::param_list * params, const pp::string & value, Imacro::macro_flags_t flags = null
		);
	virtual const status_t	create_macro( core::Tref<pp::Imacro> & res,
		const pp::string & ident, const f1::pp::attribute_list * attrs = nullptr,
		const f1::pp::param_list * params = nullptr, const pp::string * value = nullptr, Imacro::macro_flags_t flags = null
		);
	template<typename T>
	const status_t	create_macro( core::Tref<T> & res,
		const f1::pp::attribute_list * attrs = nullptr,
		const pp::param_list * params = nullptr, const pp::string * value = nullptr, Imacro::macro_flags_t flags = null
		);
	template<typename T>
	const status_t	create_user_macro( core::Tref<T> & res,
		const pp::string & ident, const f1::pp::attribute_list * attrs = nullptr,
		const pp::param_list * params = nullptr, const pp::string * value = nullptr, Imacro::macro_flags_t flags = null
		);

	const status_t	append_macro( pp::Imacro & mac );
	const status_t	append_macros( pp::macro_list_t & list );
	const status_t	remove_macro( pp::Imacro & mac );
	const status_t	remove_macros( pp::macro_list_t & list );
	const status_t	remove_all_macro();

//	static const unsigned	fl_macro_query_has_params		= 0x0001;	///< macro has parameters
//	static const unsigned	fl_macro_query_hasnot_params	= 0x0002;	///< macro has not parameters (if fl_macro_query_has_params is also set, both variants are present)
//	virtual const status_t query_macro( const pp::string & ident, unsigned & query_result ) const;

	virtual bool	exists_macro( const pp::string & ident, Imacro::macro_flags_t flags = null, Imacro::macro_flags_t mask = Imacro::macro_flags_t::all ) const;

	begin_scoped_enum(find_macro_flags_t)
		fl_macro_search_force					= 0x001,	///< find all macros, event `undefined' ones
		fl_macro_search_current_namespace_only	= 0x002,	///< search macros in the current namespace, not in parent
		fl_macro_search_test_mode				= 0x004,	///< parser currently parses an #ifdef/#ifndef directive
		fl_macro_search_all_defined				= 0x008,	///< search in all defined macros, regardless of the parsing mode
	end_scoped_enum;
	virtual const status_t	find_macro_name( core::Tconst_ref<ns::name> & nm,
		const ns::name_space * ns_parent, const core::stringslice & ident,
		find_macro_flags_t search_flags = null ) const;

	const status_t	find_macro( core::Tconst_ref<pp::Imacro> & res,
		const core::stringslice & ident,
		find_macro_flags_t search_flags = null, core::Tref<ns::name> * pname = nullptr ) const
	{
		return find_macro( res, nullptr, ident, search_flags, pname );
	}
	virtual const status_t	find_macro( core::Tconst_ref<pp::Imacro> & res,
		const core::stringslice & ident, const pp::param_list & params,
		find_macro_flags_t search_flags = null, core::Tref<ns::name> * pname = nullptr ) const
	{
		return find_macro( res, nullptr, ident, params, search_flags, pname );
	}
	virtual const status_t	find_macro( core::Tconst_ref<pp::Imacro> & res,
		const core::stringslice & ident, const pp::arg_list & args,
		find_macro_flags_t search_flags = null, core::Tref<ns::name> * pname = nullptr ) const
	{
		return find_macro( res, nullptr, ident, args, search_flags, pname );
	}

	virtual const status_t	find_macro( core::Tconst_ref<pp::Imacro> & res,
		const ns::name_space * ns_parent, const core::stringslice & ident,
		find_macro_flags_t search_flags = null, core::Tref<ns::name> * pname = nullptr ) const;
	virtual const status_t	find_macro( core::Tconst_ref<pp::Imacro> & res,
		const ns::name_space * ns_parent, const core::stringslice & ident, const pp::param_list & params,
		find_macro_flags_t search_flags = null, core::Tref<ns::name> * pname = nullptr ) const;
	virtual const status_t	find_macro( core::Tconst_ref<pp::Imacro> & res,
		const ns::name_space * ns_parent, const core::stringslice & ident, const pp::arg_list & args,
		find_macro_flags_t search_flags = null, core::Tref<ns::name> * pname = nullptr ) const;

	virtual const status_t	get_macro_from_name( core::Tconst_ref<pp::Imacro> & res, const ns::name & nm ) const;
	virtual const status_t	get_macro_from_name( core::Tconst_ref<pp::Imacro> & res, const ns::name & nm, const pp::arg_list & args ) const;
	virtual const status_t	get_macro_from_name( core::Tconst_ref<pp::Imacro> & res, const ns::name & nm, const pp::param_list & params ) const;

	const f1::ns::context &	get_ns_context() const;
		  f1::ns::context &	get_ns_context();

	virtual const status_t	on_macro_expanded( pp::Imacro & mac );
	virtual const status_t	on_unknown_macro_identifier( core::Tconst_ref<pp::Imacro> & res, const core::stringslice & ident ) const;
	virtual const status_t	on_unknown_macro( core::Tconst_ref<pp::Imacro> & res, const core::stringslice & ident ) const;

	const status_t	push_macros( const core::Tarray<pp::string> & ident_list );
	const status_t	pop_macros();

	const status_t	dump_macros( io::text_ostream & str );
	const status_t	dump_macro( io::text_ostream & str, const pp::Imacro & mac );
	virtual const status_t	dump_macro_stack( msgout::context & ctx );
public:
	struct macro_object : public ns::named_object {
		//core::Tconst_ref<pp::Imacro>	m_macro;
		pp::const_macro_list_t	m_macro_list;
		macro_object()
		{
		}
		macro_object( const pp::Imacro & mac )//:m_macro( &mac )
		{
			m_macro_list.append( &mac );
		}
		virtual void	on_all_names_removed() noexcept override
		{
			array_foreach( pp::const_macro_list_t::const_iterator, it, m_macro_list )
			{
				core::Tconst_ref<pp::Imacro> mac = *it;
				if( nullptr != mac )
					mac.ref_const_cast()->on_all_names_removed();
			}
			ns::named_object::on_all_names_removed();
		}
	private:
		macro_object( const macro_object & obj );
	};
	virtual const status_t	get_macro_name_object( core::Tref<macro_object> & res, const pp::string & ident ) const;
protected:
	//f1::ns::name_space		m_ns;
	f1::ns::context		m_ns_context;
	core::Tobject_array<macro_object>	m_macro_list;

	struct macro_desc {
		core::Tref<macro_object>	m_macro_object;
		core::Tref<ns::name_space>	m_namespace;
		pp::string					m_ident;
	};
	struct macro_stack_entry {
		core::Tarray<macro_desc>	m_macro_desc_list;
	};
	core::Tarray<macro_stack_entry>	m_macro_stack;

	const status_t	_finish_macro_search( core::Tconst_ref<pp::Imacro> & res, const pp::Imacro & macro, const core::stringslice & ident, find_macro_flags_t search_flags ) const;
	const status_t	_ask_for_macro( core::Tconst_ref<pp::Imacro> & res, const core::stringslice & ident, status_t default_status ) const;

	// Functions
	// TODO: remove
/*public:
	const status_t	append_function( const pp::Ifunction & func );
	const status_t	remove_function( const pp::Ifunction & func );
	const status_t	remove_all_function();

	const status_t	find_function( core::Tconst_ref<pp::Ifunction> & res, const pp::string & ident ) const;
	const status_t	find_function( core::Tconst_ref<pp::Ifunction> & res, const core::stringslice & ident ) const;
	const status_t	find_function( core::Tconst_ref<pp::Ifunction> & res, const char * str, size_t len = size_t(-1) ) const;

	const function_list_t &	get_function_list() const;
protected:
	typedef stdext::hash_map< std::string, core::Tconst_ref<pp::Ifunction> >	function_map_t;
	typedef std::pair		< std::string, core::Tconst_ref<pp::Ifunction> >	function_pair_t;
	function_map_t	m_function_map;
	function_list_t	m_function_list;*/

	// Attached standards
public:
	const status_t	attach_standard( const Istandard & standard );
	const status_t	detach_standard( const Istandard & standard );

	bool	is_standard_attached( const Istandard & standard ) const;
	const const_standard_list_t &	get_standard_list() const;

	const f1::pp::Istandard *	find_item_standard( const f1::guid_t & item_class_guid, const pp::string & name ) const;
protected:
	const_standard_list_t	m_standard_list;
};

template<typename T>
inline
const status_t f1::pp::Iparser::create_macro( core::Tref<T> & res,
	const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, Imacro::macro_flags_t flags
	)
{
	status_t status;

	// Create macro object
	core::Tref<T> obj = nullptr;
	if( obj.template createT<T>().failed_status( status ) )
		return status;
	__debugbreak_if( nullptr == obj )
		return STATUS_ERROR_INTERNAL;

	if( obj->initialize( attrs, params, value, flags ).failed_status( status ) )
		return status;

	// Append object
	if( append_macro( *obj ).failed_status( status ) )
		return status;

	// Exit
	res = obj;
	return STATUS_SUCCESS;
}
template<typename T>
inline
const status_t f1::pp::Iparser::create_user_macro( core::Tref<T> & res,
	const pp::string & ident, const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, Imacro::macro_flags_t flags
	)
{
	status_t status;

	// Create macro object
	core::Tref<T> obj = nullptr;
	if( obj.template createT<T>().failed_status( status ) )
		return status;
	__debugbreak_if( nullptr == obj )
		return STATUS_ERROR_INTERNAL;

	if( obj->initialize( ident, attrs, params, value, flags ).failed_status( status ) )
		return status;

	// Append object
	if( append_macro( *obj ).failed_status( status ) )
		return status;

	// Exit
	res = obj;
	return STATUS_SUCCESS;
}

template<typename T>
inline
const status_t f1::pp::Iparser::find_directive( core::Tconst_ref<T> & res ) const
{
	return m_directive_list.find_directive<T>( res );
}

/*END OF Iparser.hxx*/
