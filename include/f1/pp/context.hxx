/*context.hxx*/
/** @file
 *
 * @brief Preprocessor context class definition.
 *
 * Context represents static data that set once before parsing takes place
 * and do not change. All current state is located in the parser.
 */

class F1_PP_API f1::pp::context : public f1::core::Tcontext<pp::context, pp::Iprovider, pp::service, pp::object>
{
	//virtual context &	operator=( const context & ) noexcept;
public:
	typedef f1::core::Tcontext<pp::context, pp::Iprovider, pp::service, pp::object>	super;
	typedef f1::pp::context		self;

	explicit context() __noexcept;
			~context() __noexcept;

	// Parameters
public:
	static const unsigned fl_show_includes				= 0x0001;
	static const unsigned fl_message_macro_expansion	= 0x0002;	///< expand macros in #error directive (and in extensions line #message too)
	static const unsigned fl_output_full_path			= 0x0004;
	static const unsigned fl_msg_full_path				= 0x0008;
	static const unsigned fl_output_file_names_only		= 0x0010;
//	static const unsigned fl_c_use_digraphs				= 0x0010;
//	static const unsigned fl_c_use_trigraphs			= 0x0020;
	static const unsigned fl_big_endian					= 0x0040;
	static const unsigned fl_little_endian				= 0x0080;
	static const unsigned fl_merge_strings				= 0x0100;
	static const unsigned fl_message_file_names_only	= 0x0200;
	static const unsigned fl_global_events				= 0x0400;	///< global event issuing is performed
	const status_t	modify_flags( unsigned flags_to_set, unsigned flags_to_clear = 0, unsigned * prev_flags = nullptr );
	unsigned	get_flags( unsigned mask = unsigned(-1) ) const;

	const status_t	set_provider( Iprovider * provider ) __noexcept;
	f1::pp::Iprovider *	get_provider() __noexcept;

	const f1::fs::context &		get_fs_context() const;
		  f1::fs::context &		get_fs_context();

	const f1::parser::context &	get_parser_context() const;
		  f1::parser::context &	get_parser_context();
	//const status_t	set_parser( parser::Iparser & parser );
	//parser::Iparser *	get_parser();

	const status_t	set_syntax( const f1::guid_t & syntax_guid ) __noexcept;
	const status_t	set_syntax( pp::Isyntax * syntax ) __noexcept;
	pp::Isyntax *	get_syntax() __noexcept;
	const status_t	disable_syntax( const Isyntax & syntax ) __noexcept;
	virtual bool			is_syntax_disabled( const f1::guid_t & syntax_guid ) const __noexcept;
	virtual bool			is_syntax_disabled( const Isyntax & syntax ) const __noexcept;
	virtual const status_t	get_preferred_syntax( core::Tconst_ref<pp::Isyntax> & res ) const;
	Isyntax *	extract_syntax( pp::Istandard & standard );

//		  nls::context &		get_nls_context();
	const nls::context &		get_nls_context() const;

	enum newline_type_t {
		newline_none,
		newline_cr,					// "\r"
		newline_crlf,				// "\r\n" 
		newline_crnl = newline_crlf,// -- "" --
		newline_lf,					// "\n" 
		newline_nl = newline_lf,	// -- "" --
		newline_lfcr,				// "\n\r" 
		newline_nlcr = newline_lfcr,// -- "" --
		newline_count,
		newline_error = -1
	};
	const status_t	set_newline_type( newline_type_t newline_type );
	newline_type_t	get_newline_type() const;
	static const char *	get_newline_type_string( newline_type_t type );
	static newline_type_t	get_newline_type_from_string( const char * str, size_t len = size_t(-1) );
	const char *	get_current_newline_type_string() const;
	static const char *		get_newline_type_seq( newline_type_t type );
	const char *	get_current_newline_type_seq() const;
	static const unsigned fl_newline_type_mask_nl	= 0x0001;
	static const unsigned fl_newline_type_mask_cr	= 0x0002;
	static const unsigned fl_newline_type_mask_nlcr	= 0x0004;
	static const unsigned fl_newline_type_mask_crnl	= 0x0008;
	unsigned		count_newlines( const char * s, const char * end_s, unsigned * new_line_types = nullptr ) const;

	const status_t	set_message_base_directory( const pp::string & base_path );
	const status_t	set_message_base_directory( const core::string & base_path );
	const status_t	set_message_base_directory( const fs::path & base_path );
	const fs::path &	get_message_base_directory() const;

	const status_t	set_output_base_directory( const pp::string & base_path );
	const status_t	set_output_base_directory( const core::string & base_path );
	const status_t	set_output_base_directory( const fs::path & base_path );
	const fs::path &	get_output_base_directory() const;

//	const status_t	set_default_error_maximum_count( unsigned count );
//	unsigned	get_default_error_maximum_count() const;
//	const status_t	set_default_warning_maximum_count( unsigned count );
//	unsigned	get_default_warning_maximum_count() const;

	intmax_t		get_limit_value( limit_t lim ) const;
	const status_t	set_limit_value( limit_t lim, intmax_t value );

	const status_t	set_character_encoding( const nls::Iencoding * enc );
	const nls::Iencoding *	get_character_encoding() const;
	const status_t	set_default_input_encoding( const nls::Iencoding * enc );
	const nls::Iencoding *	get_default_input_encoding() const;
	const status_t	set_default_output_encoding( const nls::Iencoding * enc );
	const nls::Iencoding *	get_default_output_encoding() const;

	const status_t	set_frontend_context( const f1::pp::frontend::context * frontend_context );
	const f1::pp::frontend::context * get_frontend_context() const;

protected:
	core::Tref<pp::Iprovider>	m_provider;
	core::Tref<pp::Isyntax>		m_syntax;
	const_syntax_list_t			m_disabled_syntaxes;
	bitflags_t	m_flags;

	core::Tconst_ref<pp::frontend::context>	m_frontend_context;
	f1::parser::context		m_parser_context;
//	core::Tref<parser::Iparser>	m_parser;			///< parser forced by user
//	core::Tref<parser::Iparser>	m_active_parser;	///< currently active parser (equals m_parser if m_parser is not nullptr)

	nls::context			m_nls_context;
//	nls::encoding_codepage	m_default_encoding;
//	nls::codepage_iso8859_1	m_default_codepage;

//	unsigned	m_default_error_maximum_count;
//	unsigned	m_default_warning_maximum_count;

	core::Tconst_ref<nls::Iencoding>	m_character_encoding;
	core::Tconst_ref<nls::Iencoding>	m_default_input_encoding;
	core::Tconst_ref<nls::Iencoding>	m_default_output_encoding;

	newline_type_t	m_newline_type;

	fs::context		m_fs_context;
	fs::path		m_message_base_path;
	fs::path		m_output_base_path;

	// Events
public:
	begin_scoped_enum( event_id_t )

		ev_begin_task,
		ev_end_task,

		ev_parse_iteration,		//< an iteration is performed

		ev_source_started,
		ev_source_finished,

		ev_macro_added,
		ev_macro_removed,
		ev_macro_removed_all,

		ev_status_text_changed,
	
		ev_count,
		ev_event_invalid
	end_scoped_enum;
	interface F1_PP_API Ievent_handler : public f1::core::object {
		typedef f1::core::object	super;
		typedef Ievent_handler		self;
		virtual ~Ievent_handler();
		virtual const status_t	on_attach( pp::context & ctx );
		virtual void			on_detach( pp::context & ctx );
		virtual void	invoke( event_id_t id, void * param ) abstract;
	private:
		friend class f1::pp::context;
		core::Tref<Ievent_handler>	m_handler_next;
	};
	bool	is_event_enabled( event_id_t id ) const;
	const status_t	append_event_handler( event_id_t id, Ievent_handler & handler );
	const status_t	remove_event_handler( event_id_t id, Ievent_handler & handler );
	const status_t	remove_all_event_handlers( Ievent_handler & handler );
	const status_t	invoke_event ( event_id_t id, void * param = nullptr );
protected:
	struct event_desc_t {
		core::Tref<Ievent_handler>	m_handler_first;
		core::Tref<Ievent_handler>	m_handler_last;
	};
	event_desc_t	m_event[event_id_t::ev_count];
	core::Tbitmask<event_id_t::ev_count>	m_event_mask;
	const status_t	remove_all_event_handlers();

	// Global event support
public:
	/*
	const f1::core::Ievent_handler *	get_event_handler( Eevent_id id );
	const status_t	set_event_handler( Eevent_id id, f1::core::Icallback * cb );
	const status_t	invoke_event( Eevent_id id, void * data = nullptr );

	virtual const status_t	ev_on_parse_iteration( pp::Iparser & par );
	virtual const status_t	ev_on_macro_added( pp::Imacro & mac );
	virtual const status_t	ev_on_macro_removed( pp::Imacro & mac );
	virtual const status_t	ev_on_all_macros_removed();
protected:
	core::Tref<Oevent_handler>	m_event_handler[ev_event_count];*/

	// Includes
	// These includes are specified by the client, e.g. in the form of -I or -I- command
	// line switches.
public:
	struct include_file_t {
		core::string	m_file_name;
		static const unsigned fl_system			= 0x0001;
		static const unsigned fl_deprecate		= 0x0002;
		static const unsigned fl_auto_include	= 0x0004;
		unsigned		m_flags_to_set;
		unsigned		m_flags_to_clear;
		include_file_t():m_flags_to_set( 0 ), m_flags_to_clear( 0 )
		{
		}
	};
	const status_t	append_include_file( const core::static_string & filename, unsigned include_flags_to_set, unsigned include_flags_to_clear = 0 );
	const status_t	append_include_files( int sep, const core::static_string & filename, unsigned include_flags_to_set, unsigned include_flags_to_clear = 0 );
	const status_t	append_include_files( const core::Tarray<core::string> & filenames, unsigned include_flags_to_set, unsigned include_flags_to_clear = 0 );
	const status_t	remove_include_file( const core::static_string & filename );
	const status_t	remove_include_files( int sep, const core::static_string & filename );
	const status_t	remove_include_files( const core::Tarray<core::string> & filenames );
		  core::Tarray<include_file_t> &	get_include_files()		  { return m_include_files; }
	const core::Tarray<include_file_t> &	get_include_files() const { return m_include_files; }
//	const status_t	set_maximum_include_depth( unsigned maximum_depth );
//	unsigned		get_maximum_include_depth() const;

//		  f1::fs::search_dir_list &		get_include_dirs()		 { return m_include_dirs; }
//	const f1::fs::search_dir_list &		get_include_dirs() const { return m_include_dirs; }

private:
//	unsigned	m_maximum_include_depth;
//	f1::fs::search_dir_list		m_include_dirs;
	core::Tarray<include_file_t>m_include_files;

	// Macros
	// These macros are specified by the client, e.g. in the form of -D or -U command
	// line switches.
public:
		  core::Tarray<core::string> &	get_macros_to_undefine()	   { return m_macros_to_undefine; }
	const core::Tarray<core::string> &	get_macros_to_undefine() const { return m_macros_to_undefine; }
	const status_t	append_macro( const char * name, size_t name_len = size_t(-1) );
	const status_t	append_macro( const char * name, size_t name_len, const char * value, size_t value_len = size_t(-1) );
	const status_t	remove_macro( const char * name, size_t name_len = size_t(-1) );
	struct macro_def_t {
		core::string	m_name;
		core::string	m_value;
	};
		  core::Tarray<macro_def_t> &	get_macros_to_define()		 { return m_macros_to_define; }
	const core::Tarray<macro_def_t> &	get_macros_to_define() const { return m_macros_to_define; }
//	struct define_op_desc_t {
//		core::string	m_name;
//		core::string	m_value;
//		bool			m_is_define;
//	};
//	const status_t append_define_macro( const core::static_string & name, const core::static_string * value = nullptr );
//	const status_t append_undefine_macro( const core::static_string & name );
protected:
//	core::Tarray<define_op_desc_t>	m_define_ops;
	core::Tarray<core::string>	m_macros_to_undefine;
	core::Tarray<macro_def_t>	m_macros_to_define;

	// Options
public:
	const status_t	create_option( core::Tref<Ioption> & res,
		pp::variant::type_t ty,
		const f1::guid_t & guid,
		const char * name, size_t len = size_t(-1) );

	const status_t	append_option(
		pp::variant::type_t ty,
		const f1::guid_t & guid,
		const char * name, size_t len = size_t(-1) );
	const status_t	append_option( Ioption & opt );
	const status_t	remove_option( const Ioption & opt );
	const status_t	remove_options( const const_option_list_t & option_list );
	const status_t	remove_options( const core::Tarray<pp::string> & ident_list );
	const status_t	remove_options( const core::Tarray<f1::guid_t> & guids );
	const status_t	remove_all_options();

	bool	exists_option( const f1::guid_t & guid ) const;
	bool	exists_option( const char * name, size_t len = size_t(-1) ) const;

	const status_t	find_option( core::		 Tref<Ioption> & res, const f1::guid_t & guid );
	const status_t	find_option( core::Tconst_ref<Ioption> & res, const f1::guid_t & guid ) const;
	const status_t	find_option( core::		 Tref<Ioption> & res, const char * name, size_t len = size_t(-1) );
	const status_t	find_option( core::Tconst_ref<Ioption> & res, const char * name, size_t len = size_t(-1) ) const;
	const status_t	find_option( core::		 Tref<Ioption> & res, const pp::string & name );
	const status_t	find_option( core::Tconst_ref<Ioption> & res, const pp::string & name ) const;
	const status_t	find_options( const_option_list_t & opts, const core::Tarray<pp::string> & names ) const;

	const pp::variant &	get_option_value( const f1::guid_t & guid );
	const pp::variant &	get_option_value( const char * name, size_t len = size_t(-1) ) const;
	const status_t	set_option_value( const pp::variant & value, const f1::guid_t & guid );
	const status_t	set_option_value( const pp::variant & value, const char * name, size_t len = size_t(-1) ) const;
	const status_t	set_option_value( const pp::variant & value, const pp::string & name ) const;

	const status_t	push_all_options();
	const status_t	push_options( const option_list_t & lst );
	const status_t	push_options( const core::Tarray<pp::string> & names );
	const status_t	push_options( const core::Tarray<f1::guid_t> & guids );
	const status_t	push_option( const f1::guid_t & guid );
	const status_t	push_option( const pp::string & name );
	const status_t	push_option(	   pp::Ioption & opt );
	const status_t	pop_options();

	const option_list_t &	get_option_list() const;
		  option_list_t &	get_option_list();
protected:
	option_list_t	m_option_list;

	struct option_desc {
		f1::guid_t	m_guid;		///< option GUID
		pp::string	m_name;		///< option name, preserved in case the option is deleted
		pp::variant	m_value;	///< option value
	};
	struct option_stack_entry {
		core::Tarray<option_desc>	m_option_desc_list;
	};
	core::Tarray<option_stack_entry>	m_option_stack;

	// Outputs
public:
	begin_scoped_enum( output_flags_t )
		fl_write_bom	= 0x0001,
	end_scoped_enum;
	const status_t	modify_output_flags( output_flags_t flags_to_set, output_flags_t flags_to_clear = null, output_flags_t * prev_flags = nullptr );
	output_flags_t	get_output_flags( output_flags_t mask = output_flags_t::all ) const;

	const status_t	make_output_file_name( core::string & buf, const char * ext, const char * src_filename, size_t src_len = size_t(-1) ) const;

	const status_t	set_output_file_name( const char * name, size_t len = size_t(-1) );
	const status_t	set_output_file_name( const core::string & name );
	const status_t	set_output_file_name( const pp::string & name );
	const status_t	set_output_file_name( const fs::path & name );
	const core::string &	get_output_file_name() const;

	const status_t	set_output_directory( const char * name, size_t len = size_t(-1) );
	const status_t	set_output_directory( const core::string & name );
	const status_t	set_output_directory( const pp::string & name );
	const status_t	set_output_directory( const fs::path & name );
	const core::string &	get_output_directory() const;

	const status_t	set_output_class( pp::Ioutput_class * output_class ) __noexcept;
	pp::Ioutput_class *	get_output_class() __noexcept;

	const status_t	set_output( pp::Ioutput * output ) __noexcept;
	pp::Ioutput *	get_output() __noexcept;

	const status_t	set_storage_class( pp::Istorage_class * storage_class ) __noexcept;
	pp::Istorage_class *	get_storage_class() __noexcept;
	pp::Istorage_class &	get_default_storage() __noexcept;

	const status_t	create_storage( core::Tref<pp::Istorage> & res,
		const core::string &		output_filename,
		const f1::guid_t *			preferred_class = nullptr,
		Istorage::storage_flags_t	flags = Istorage::storage_flags_t::null,
		const core::Iidentifier *	ident = nullptr );
	virtual const status_t	set_storage( pp::Istorage * storage ) __noexcept;
	pp::Istorage *	get_storage() __noexcept;
protected:
	core::Tref<pp::Ioutput_class>	m_output_class;
	core::Tref<pp::Ioutput>			m_output;
	file_storage_class				m_file_storage_class;
	core::Tref<pp::Istorage_class>	m_storage_class;
	core::Tref<pp::Istorage>		m_storage;
	core::string	m_output_directory;
	core::string	m_output_file_name;
	core::Tbitflags<output_flags_t>	m_output_flags;

	// Sources
	// Several sources are supported, results are stored in the same output
public:
	const status_t	append_source( const char * filename, size_t len = size_t(-1) );
	const status_t	append_source( parser::Isource & src );
	const status_t	remove_all_sources();

	//const status_t	set_root_source( const char * file );
	//const status_t	set_root_source( parser::Isource & src );
	//parser::Isource *	get_root_source();
protected:
	struct source_desc {
		core::string				m_filename;
		core::Tref<parser::Isource>	m_source;
	};
	core::Tarray<source_desc>	m_sources;
	//parser::source_list_t	m_sources;

	// Attached standards
public:
	const status_t	append_standard( pp::Istandard & standard );
	const status_t	append_standards( standard_list_t & standards );
	const status_t	remove_standard( pp::Istandard & standard );
	const status_t	remove_all_standards();

	const standard_list_t &	get_standard_list() const;
		  standard_list_t &	get_standard_list();

	const status_t	set_default_standard( const pp::Istandard & standard );
	const status_t	set_default_standard( const f1::guid_t & guid );
	const status_t	set_default_standard( const char * name, ptrdiff_t len = -1 );

	const Istandard *	find_item_standard( const f1::guid_t & item_class_guid, const pp::string & name ) const;
	const Istandard *	find_item_standard( const f1::guid_t & item_class_guid, const Istandard & standard, const pp::string & name ) const;
protected:
	standard_list_t		m_standard_list;
	core::Tconst_ref<pp::Istandard>	m_default_standard;

	// Storage
public:
//	const status_t	include( const char * data, size_t len = size_t(-1) );
//	const status_t	include( parser::Isource & src );
protected:

	// Parser commands
public:
	const status_t	add_preinit_parser_command( Iparser::Icommand & cmd );
	const status_t	add_postinit_parser_command( Iparser::Icommand & cmd );

/*	const status_t	append_parser_parameter( const f1::guid_t & param_guid, bool val );
	const status_t	append_parser_parameter( const f1::guid_t & param_guid, uintmax_t val );
	//const status_t	append_parser_parameter( const f1::guid_t & param_guid, const char * val, size_t len = size_t(-1) );
	const status_t	append_parser_parameter( const f1::guid_t & param_guid, const core::variant & val );*/

	const core::Tobject_array<Iparser::Icommand> &	get_preinit_parser_commands() const noexcept { return m_preinit_parser_commands; }
		  core::Tobject_array<Iparser::Icommand> &	get_preinit_parser_commands()		noexcept { return m_preinit_parser_commands; }
	const core::Tobject_array<Iparser::Icommand> &	get_postinit_parser_commands() const noexcept{ return m_postinit_parser_commands; }
		  core::Tobject_array<Iparser::Icommand> &	get_postinit_parser_commands()		noexcept { return m_postinit_parser_commands; }
protected:
/*	struct parser_param_desc_t {
		f1::guid_t		m_param_guid;
		core::variant	m_value;
	};
	core::Tarray<parser_param_desc_t>	m_parser_param_list;	// we can't use maps because there can be several entries with the same GUID
*/
	core::Tobject_array<Iparser::Icommand>	m_preinit_parser_commands;
	core::Tobject_array<Iparser::Icommand>	m_postinit_parser_commands;

	// Dump tasks
public:
	begin_scoped_enum( dump_flags_t )
		fl_dump_directives			= 0x00010000,
		fl_dump_includes			= 0x00020000,
		fl_dump_macros				= 0x00040000,
		fl_dump_pragmas				= 0x00080000,
		fl_dump_known_pragmas		= 0x00100000,
		fl_dump_standards			= 0x00200000,
	end_scoped_enum;
	struct dump_task {
		core::string	m_target_filename;
		//core::Tref<dump_formatter>	m_formatter;
		dump_flags_t	m_dump_flags;
	};

	const status_t	append_dump_task( dump_flags_t flags, const char * target_filename, size_t length );
	const core::Tarray<dump_task> &	get_dump_task_list() const;
protected:
	core::Tarray<dump_task>	m_dump_task_list;

	// Messages
public:
	enum {
		MSGID_STANDARD_NOT_FOUND,
		MSGID_MACRO_NOT_AVAILABLE
	};

	const msgout::context &	get_msgout_context() const;
		  msgout::context &	get_msgout_context();

	enum Efilename_print {
		Efilename_print_none,
		Efilename_print_warning,
		Efilename_print_error,
		Efilename_print_each,
		Efilename_print_invalid = -1,
	};
	Efilename_print	get_filename_print() const;
	const status_t set_filename_print( Efilename_print val );

	struct Imessage_sink : public core::object {
		virtual const status_t	output_message( msgout::context & ctx, f1::msgout::msgid_t format, va_list ap ) abstract;
	};
	struct default_message_sink : public Imessage_sink {
		virtual const status_t	output_message( msgout::context & ctx, f1::msgout::msgid_t format, va_list ap ) override;
	};
	const status_t	append_message_sink( Imessage_sink & sink );
	const status_t	remove_message_sink( Imessage_sink & sink );

	const status_t	message( f1::msgout::msgid_t format, ... );
	const status_t	message_ap( f1::msgout::msgid_t format, va_list ap, msgout::context & ctx );
protected:
	msgout::context		m_msgout_context;

	msgout::raw_group	m_message_group;

	Efilename_print		m_filename_print;

	intmax_t	m_limit[limit_t::li_count];

	static const msgout::Igroup::message_desc	m_msgout_messages[];

	default_message_sink	m_default_message_sink;
	core::Tweak_object_array<Imessage_sink>	m_message_sink_list;

	// Operations
public:
	const status_t	process();

	virtual const status_t	on_parser_attached( pp::Iparser & parser );
	virtual const status_t	on_parser_detached( pp::Iparser & parser );
protected:
	const status_t	_process( parser::Isource & src );
	const status_t	_process( const char * base_source );
	const status_t	_process_common( pp::Iparser & par );

	parser_list_t	m_parsers;
};

/*END OF context.hxx*/
