/*parser.hxx*/

class F1_PP_API f1::c_pp::parser : public f1::pp::Iparser
{
	explicit parser( f1::c_pp::parser & );
public:
	typedef f1::pp::Iparser		super;
	typedef f1::c_pp::parser	self;

	explicit parser( const core::Iidentifier * ident = nullptr );

	//virtual const status_t	initialize( const f1::pp::context & context, f1::pp::Isyntax & syntax, f1::pp::Ioutput & outp, f1::parser::Iparser * parser, const char * base_source ) override;
protected:
	virtual const status_t	_initialize( f1::parser::Isource * source ) override;

//	core::Tlistener<self>	m_source_listener;
//	void on_source_added( core::object & src, core::object * param );
	struct F1_PP_API parser_event_handler : public f1::parser::Ievent_handler {
		virtual const status_t	invoke( f1::parser::event_type_t type_mask, void * handler_param, void * event_param, f1::parser::Iparser & parser ) override;
	};
	parser_event_handler	m_parser_event_handler;	// TODO: check why is the same object defined in the base class

	const status_t on_line_changed( f1::parser::lineno_t line ) override;

	// Properties
public:
	begin_scoped_enum( c_flags_t )
		fl_disable_directives			= 0x00000001,	///< directives are not executed, just parsed; no state gets changed
		fl_disable_expression			= 0x00000002,	///< disable expession evaluation (just parsing is performing, without calls to operator_desc_t::m_execute)
		fl_multiple_error_directives	= 0x00000004,	///< allow multiple #error directives
		fl_pass_invalid_includes		= 0x00000008,	///< if #include file wasn't found, pass the #include directive to output
		fl_keep_comments				= 0x00000010,	///< keep comments in output
		fl_standard_digraphs			= 0x00000020,	///< use standard digraphs only (optimization hint, on by default)
		fl_standard_trigraphs			= 0x00000040,	///< use standard trigraphs only (optimization hint, on by default)
		fl_disable_string_escapes		= 0x00000080,	///< disable escapes in strings
		fl_process_digraphs				= 0x00000100,	///< process digraphs
		fl_process_trigraphs			= 0x00000200,	///< process trigraphs
		fl_process_ucn_chars			= 0x00000400,	///< process UCN sequences
		fl_process_multigraphs			= 0x00000700,	///< process digraphs and trigraphs
		fl_execute_exec_top_only		= 0x00000800,	///< process directive only from execution top (open and close directives)
		fl_dequotize_messages			= 0x00001000,	///< dequotize messages in #error and #message directives
		fl_dollar_sign_in_idents		= 0x00002000,	///< allow '$' in identifiers
		fl_nested_comments				= 0x00004000,	///< allow nested comments
		fl_unsigned_char				= 0x00008000,	///< character constants are unsigned
		fl_auto_once_includes			= 0x00010000,	///< assume #pragma once in the start of each included file
		fl_do_not_expand_macros			= 0x00020000,	///< do not expand macros
		fl_keep_doxygen_comments		= 0x00040000,	///< keep doxygen comments
		fl_at_sign_in_idents			= 0x00080000,	///< allow '@' in identifiers
		fl_dump_directives				= 0x00100000,
		fl_dump_includes				= 0x00200000,
		fl_dump_macros					= 0x00400000,
		fl_dump_pragmas					= 0x00800000,
		fl_dump_known_pragmas			= 0x01000000,
		fl_dump_standards				= 0x02000000,
		fl_drop_pass_through_directives	= 0x04000000,	///< pass-through directives are dropped instead of being sent to the output
		fl_directive_no_case			= 0x08000000,
		fl_long_wchar_t					= 0x10000000,	///< if set, wchar_t occupies 4 bytes, otherwise 2
		fl_disable_comments				= 0x20000000,	///< disable comments parsing (do not recogniz them; used after macro expansion)
		fl_disable_directives_in_args	= 0x40000000,	///< disable directives in while parsing macro arguments
		//fl_keep_new_lines			= 0x00400000,	///< output new-lines as is
	end_scoped_enum;
	const status_t	modify_c_flags( c_flags_t flags_to_set, c_flags_t flags_to_clear = c_flags_t::null, c_flags_t * prev_flags = nullptr );
	const status_t	set_c_flags( c_flags_t flags, c_flags_t * prev_flags = nullptr );
	c_flags_t	get_c_flags( c_flags_t mask = c_flags_t::all ) const;

	begin_scoped_enum( c_message_flags_t )
		fl_warn_time_macros			= 0x0001,	///< warn when one of __DATE__, __TIME__ or __TIMESTAMP__ is used
		fl_warn_digraphs			= 0x1000,	///< warn digraphs
		fl_warn_trigraphs			= 0x2000,	///< warn trigraphs
		fl_warn_ucn_chars			= 0x4000,	///< warn UCN sequences
	end_scoped_enum;
	const status_t	modify_c_message_flags( c_message_flags_t flags_to_set, c_message_flags_t flags_to_clear = c_message_flags_t::null, c_message_flags_t * prev_flags = nullptr );
	c_message_flags_t	get_c_message_flags( c_message_flags_t mask = c_message_flags_t::all ) const;

	const status_t	set_esc_char( int ch );
	int		get_esc_char() const;

	const status_t	set_charizing_char( int ch );
	int		get_charizing_char() const;

	bool	set_ignore_next_newline( bool disable = true );

	unsigned	get_error_directive_count() const;

	unsigned	get_macro_expansion_nesting() const;

	static const f1::guid_t	m_guid_clear_c_flags;	// u64
	static const f1::guid_t	m_guid_set_c_flags;		// u64
	const status_t	set_parameter( const f1::guid_t & param_guid, const core::variant & val ) override;

	bool	m_VA_ARGS_supported;
	bool	m_VA_OPT_supported;
	bool		m_new_line;
protected:
	c_flags_t			m_c_flags;
	c_message_flags_t	m_c_message_flags;
	int			m_esc_char;
	int			m_charizing_char;
	unsigned	m_error_directive_count;	///< number of processed #error directives if fl_multiple_error_directives flag is set
	const char *m_seq_start;
	bool		m_allow_args_separators;
	bool		m_was_new_line;
	bool		m_disable_line_update;
	bool		m_disable_next_line_update;
	unsigned	m_macro_expansion_nesting;
	core::Tarray<int> m_parentheses_stack;

	core::Tref<f1::parser::Isource>		m_source;

	// Messages
public:
	using super::message;
	virtual const status_t	message( const char * file, f1::parser::lineno_t line, f1::msgout::msgid_t msg, va_list ap ) override;

	// Execution
public:
	const status_t	set_allow_exec( bool allow = true, bool force = false );
	bool	is_exec_allowed() const;
	bool	was_exec_allowed() const;

	using super::get_directive_frame;
	virtual pp::Iframe *	get_directive_frame() override;
	virtual pp::Iframe *	get_directive_frame( const f1::guid_t & openning_guid ) override;
protected:
	bool		m_can_exec;
	//unsigned	m_exec_disable_count;
	bool		m_post_directive_can_exec;
	bool		m_pre_directive_can_exec;

	// Subsections
public:
	virtual const status_t	begin_subsection( const pp::Imacro & mac ) noexcept;
	virtual const status_t	begin_subsection( const pp::Iframe & frm, const pp::string & ident ) noexcept;

	virtual const status_t	begin_subsection( const char * ident = nullcstr, size_t len = size_t(-1) ) noexcept;
	virtual const status_t	begin_subsection( const core::static_string & ident ) noexcept;
	virtual const status_t	begin_subsection( const pp::string & ident ) noexcept;

	virtual const status_t	end_subsection() noexcept;
protected:
	struct subsection_desc {
		core::Tconst_weak_ref<pp::Imacro>	m_macro;
		const pp::Iframe *	m_frame;
	};
	typedef core::Tarray<subsection_desc>	subsection_stack_t;
	subsection_stack_t	m_subsection_stack;

	// Directives
public:
	virtual const pp::attribute_list *	get_directive_attribute_list() const { return nullptr; }

	// Operations
public:
	virtual  const status_t	on_merge_operator( const pp::Imacro & mac, const pp::string & buf );
	virtual  const status_t on_source_included( f1::parser::Isource & src ) noexcept;
	virtual  const status_t on_source_enter( f1::parser::Isource & src ) noexcept;
	virtual  const status_t on_source_leave( f1::parser::Isource & src ) noexcept;

	class root_parser_hooks : public super::root_parser_hooks {
	public:
		virtual const status_t	on_esc_char( pp::Iparser & parser ) override;
	};
	class line_parser_hooks : public super::line_parser_hooks {
	public:
		virtual const status_t	on_esc_char( pp::Iparser & parser ) override;
	};

	virtual const status_t	parse() override;
	virtual const status_t	abort_parser() override;
	typedef const status_t	capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & rest );
	virtual const status_t	capture_line( capture_callback * cb, core::object * param, size_t max_len = size_t(-1) );
	virtual const status_t	abort_capture();
	virtual const status_t	finish_parse();
			const status_t	disable_capture();
			const status_t	enable_capture();

	virtual const status_t	skip_lexem( const char * & s, const char * end_s, core::Tarray<char> & bracket_stack );
	virtual const status_t	skip_to_separator( const char * & s, const char * end_s, int end_ch );
	virtual const status_t	skip_to_separator( size_t & len, int end_ch );
	virtual const status_t	find_separator	 ( size_t & len, int end_ch );
	virtual const status_t	skip_to_separator( const char * & s, const char * end_s, int end_ch1, int end_ch2 );
	virtual const status_t	skip_to_separator( size_t & len, int end_ch1, int end_ch2 );
	virtual const status_t	find_separator	 ( size_t & len, int end_ch1, int end_ch2 );
	virtual const status_t	skip_to_separator( const char * & s, const char * end_s, const char * end_ch );
	virtual const status_t	skip_to_separator( size_t & len, const char * end_ch );
	virtual const status_t	find_separator	 ( size_t & len, const char * end_ch );

	virtual bool			on_char( unsigned ch );
	virtual bool			is_eof() const;
	virtual const status_t	on_esc_char();

//	const argument_parsing_context_t *	get_current_argument_parsing_context() const;
//	const argument_parsing_context_t *	get_previous_argument_parsing_context() const;
//	const argument_parsing_context_list_t &	get_argument_parsing_context_list() const;
	class F1_PP_API patch_callback : public f1::parser::Iparser::Ipatch_callback
	{
	public:
		void on_patch_completed( f1::parser::Iparser & parser, void * param ) noexcept override;

		core::Tref<pp::Imacro>			m_macro;
		core::Tconst_ref<pp::arg_list>	m_arg_list;
		pp::buffer_storage		m_saved_unexpanded_buffer_output;
		size_t					m_arg_parse_context_top;
		c_flags_t				m_saved_c_flags;
		int						m_saved_args_parse_mode;
		bool					m_saved_allow_unexpanded_output;
		bool					m_saved_capturing_line;
		bool					m_unlock_all;
		f1::pp::macro_list_t	m_recursive_macros;	// all recursive macros found when expanding macro arguments
	};
	typedef core::Tarray<patch_callback *>	patch_callback_list_t;
	const patch_callback *	get_current_patch_callback() const;
	const patch_callback *	get_previous_patch_callback() const;
	const patch_callback_list_t &	get_argument_patch_callback_list() const;
protected:
	//pp::buffer_output		m_buffer_output;
	//f1::pp::macro_list_t	m_recursive_macros;

	virtual const status_t	dump_items();
	virtual const status_t	dump_macro_stack( msgout::context & ctx ) override;
	const status_t	dump_patch( msgout::context & ctx, const patch_callback & patch );

	patch_callback_list_t	m_patch_callback_list;
	friend class patch_callback;

	struct capture_line_breakpoint_callback : public f1::parser::Iparser::Ibreakpoint_callback {
		virtual const status_t	on_breakpoint_reached() override;
		core::Tweak_ref<f1::c_pp::parser>	m_parser;
	};/* m_capture_line_breakpoint_callback;
	bool						m_capturing_line;
	core::Tref<pp::Ioutput>		m_pre_capture_output;
	c_flags_t					m_pre_capture_c_flags;
	bool						m_pre_capture_can_exec;
	capture_callback *			m_capture_callback;
	core::Tref<core::object>	m_capture_callback_param;
	int							m_pre_capture_args_parse_mode;*/
	bool						m_finish_capture_line;
	bool						m_capturing_line;
	int							m_capture_callback_call;
	core::Tarray<bool>	m_capture_save_stack;

	struct F1_PP_API capture_output : public pp::Ioutput {
		typedef pp::Ioutput		super;
		explicit capture_output( const core::Iidentifier * ident = nullptr ) __noexcept;
		const status_t	initialize( f1::c_pp::parser & parser );
				const void		reset();
		virtual const status_t	write( const char * string, size_t len = size_t(-1) ) override;
		//virtual const status_t	on_line_changed( f1::parser::lineno_t lineno ) noexcept override;
		core::Tweak_ref<f1::c_pp::parser>	m_parser;
	};
	//capture_output	m_capture_output;
	const status_t	finish_capture_line( f1::parser::lineno_t lineno = 0, bool call_callback = true );

	struct F1_PP_API capture_context
	{
		core::Tarray<patch_callback *>		m_patch_callback_list;
		capture_line_breakpoint_callback *	m_capture_line_breakpoint_callback;
		core::Tref<pp::Ioutput>		m_pre_capture_output;
		c_flags_t					m_pre_capture_c_flags;
		bool						m_pre_capture_can_exec;
		capture_callback *			m_capture_callback;
		core::Tref<core::object>	m_capture_callback_param;
		int							m_pre_capture_args_parse_mode;
		core::Tref<capture_output>	m_capture_output;
		bool						m_pre_capturing_line;
		f1::parser::Iparser::break_desc *	m_breakpoint;
		bool	m_limited;
		size_t						m_parentheses_stack_depth;
	};
	core::Tarray<capture_context>	m_capture_stack;

	struct argument_parsing_context_t {
		pp::string					m_ident;
		core::Tref<pp::arg_list>	m_arg_list;
		core::Tref<pp::Imacro>		m_macro;
		f1::pp::macro_list_t		m_recursive_macros;	// list of all recursive macros
		core::Tref<pp::Ioutput>		m_previous_output;
		core::Tref<pp::Istorage>	m_previous_storage;
		pp::raw_output				m_output;
		pp::buffer_storage			m_expanded_buffer_output;
		pp::buffer_storage			m_saved_unexpanded_buffer_output;
//		pp::buffer_output			m_sub_unexpanded_buffer_output;
		core::Tarray<int>			m_parentheses_stack;
		int							m_previous_args_parse_mode;
		//f1::parser::lineno_t		m_line;
		f1::parser::Iparser::location	m_location;
		bool						m_allow_args_separators;
		bool						m_saved_capturing_line;
	};
	typedef core::Tarray<argument_parsing_context_t*>	argument_parsing_context_list_t;

	const status_t	begin_arg_parse( const pp::string & ident, pp::Imacro & mac );
	const status_t		next_arg_parsed();
	const status_t	end_arg_parse( bool abort = false );
	const status_t	abort_arg_parse();
	const status_t	format_unexpanded_macro_call( pp::string & buf,
		const argument_parsing_context_t & argparse ) const;
	const status_t	format_unexpanded_macro_call( pp::string & call_buf,
		const pp::string & ident, const pp::Imacro & mac, const pp::arg_list * args ) const;
	const status_t	dump_arg_parsing_context( msgout::context & ctx, const argument_parsing_context_t & arg );

	argument_parsing_context_list_t	m_arg_parse_stack;
	int					m_args_parse_mode;
	pp::buffer_storage	m_unexpanded_buffer_output;

	// Output
public:
	virtual const status_t	output_expanded( const char * str, size_t len = size_t(-1) );
	virtual const status_t	output_expanded( const pp::string & str );
	virtual const status_t	output_unexpanded( const char * str, size_t len = size_t(-1) );
	virtual const status_t	output_unexpanded( const pp::string & str );
	virtual const status_t	output( const char * str, size_t len = size_t(-1) ) override;
	using super::output;

	virtual const status_t	output_directive( pp::Idirective & drective );

	// Directives
public:
	/*const directive_list *	get_directive_list() const;
		  directive_list *	get_directive_list();
	virtual const status_t	find_directive( core::Tref<Idirective> & res, const f1::guid_t & guid ) const;
	virtual const status_t	find_directive( core::Tref<Idirective> & res, const char * name, size_t length = size_t(-1) ) const;
	virtual const status_t	find_directive( core::Tref<Idirective> & res, const pp::string & name ) const;

	bool	is_directive_available( const pp::Idirective & directive ) const;*/

	virtual const status_t	on_execute_directive( pp::Idirective & obj ) override;
	virtual const status_t	on_unknown_directive( const pp::string & ident ) override;
protected:
//	directive_list		m_directive_list;
	root_parser_hooks	m_root_parser_hooks;
	core::Tweak_ref<pp::Idirective>	m_line_directive;

	// Supported pragmas
public:
	const status_t	append_pragma( const Ipragma & pragma );
	const status_t	append_pragmas( const pragma_list_t & list );
	const status_t	remove_pragma( const Ipragma & pragma );
	const status_t	remove_pragmas( const pragma_list_t & list );
	const status_t	remove_all_pragmas();

	const status_t	find_pragma( core::Tref<Ipragma> & res, const pp::string & ident, const const_pragma_list_t * pragma_list = nullptr ) const;

	const const_pragma_list_t &	get_pragma_list() const;
protected:
	const_pragma_list_t	m_pragma_list;

	// Known pragmas
public:
	const status_t	append_known_pragma( const char * name, size_t len = size_t(-1) );
	const status_t	remove_known_pragma( const char * name, size_t len = size_t(-1) );
	const status_t	remove_all_known_pragmas();

	const core::const_string_list_t &	get_known_pragma_list() const;
protected:
	core::const_string_list_t	m_known_pragma_list;

	// Included sources descriptors
	// Note that the same file can be included multiple times so specified_name will all be the same,
	// but pointers to source will differ.
public:
	///! flags that describe known information about an include file
	begin_scoped_enum( include_flags_t )
		fl_system			= 0x0001,	///< file is included as a system file (used only when m_source != nullptr)
		fl_user				= 0x0002,	///< file is included as a user file (used only when m_source != nullptr)
		fl_common			= 0x0003,
		fl_once_included	= 0x0004,	///< a #pragma once was used or --auto-once-includes was specified
		fl_pass_unmodified	= 0x0008,	///< do not include file; pass #include directive unmodified
		fl_report			= 0x0010,	///< report when the marked file gets included
		fl_silent			= 0x0020,	///< do not report error if the file wasn't found; usually used with fl_pass_unmodified
		fl_deprecated		= 0x0040,	///< an include file is deprecated and must be avoided
	end_scoped_enum;
	///! include specification. Note that some command line switches can automatically include files
	///! or specify flags for particular files which will be included during preprocessing.
	struct include_desc {
		core::string	m_specified_name;		///< file name as specified by user (can be empty if m_source is not nullptr)
		fs::path		m_full_path_name;		///< resolved file name as specified by user (can be empty)
		core::Tconst_ref<f1::parser::Istorage>	m_storage;	///< storage object that refers to the file data (nullptr if wasn't #include'd yet)
		include_flags_t	m_flags;	///< include file flags
	};
//	const status_t	find_or_create_include( include_desc *& desc, const char * specified_name, const f1::parser::Isource * src = nullptr );

	const status_t	register_include( include_desc *& desc, const char * specified_name, size_t length = size_t(-1), include_flags_t include_flags = null );
	const status_t	set_include_file( include_desc & desc, const f1::parser::Istorage & stg );
	const status_t	modify_include_flags( include_desc & desc, include_flags_t flags_to_set, include_flags_t flags_to_clear = include_flags_t::null );

//	const status_t	register_include( file_desc & file, const char * specified_name, const f1::parser::Isource * src = nullptr, include_flags_t flags = include_flags_t::null );
	const status_t	register_include( const char * specified_name, const f1::parser::Isource * src = nullptr, include_flags_t flags = include_flags_t::null );
//	const status_t	register_include( const char * specified_name, include_flags_t flags = include_flags_t::null );
	typedef core::Tarray<include_desc>	include_desc_list_t;
	const include_desc_list_t &	get_include_list() const;

	const status_t	find_include( const include_desc *& res, const f1::parser::Istorage & stg ) const;
	const status_t	find_include( const include_desc *& res, const char * specified_name, size_t length = size_t(-1) ) const;
	const status_t	find_include_full_path_name( const include_desc *& res, const char * full_path_name, size_t length = size_t(-1) ) const;

	virtual const status_t	dump_includes( msgout::context & ctx, dump_flags_t flags = dump_flags_t::null ) const override;
	virtual const status_t	dump_include_info( msgout::context & ctx, f1::parser::Isource & src, dump_flags_t flags = dump_flags_t::null ) const override;

	// Aliases that specified by #pragma include_alias
	struct alias_desc {
		core::string	m_alias_name;			///< source file name
		core::string	m_aliased_name;			///< target file name
	};
	const status_t	register_alias( alias_desc ** file, const core::static_string & source_file_name, const core::static_string & target_file_name );
	typedef core::Tarray<alias_desc>	alias_desc_list_t;
	const alias_desc_list_t &	get_alias_list() const;
	const status_t	find_alias( const alias_desc *& res, const char * source_name, size_t length = size_t(-1) ) const;

	// Included sources that were marked with with `pragma once'
	const status_t	register_once_include();
	bool	is_once_include( bool system, const char * name, size_t size = size_t(-1) ) const;
	bool	is_once_include( include_flags_t include_flags, const char * name, size_t size = size_t(-1) ) const;

	// Check existence of directories in the search directory list
	void	check_include_dirs( fs::search_dir_list & list );
	void	check_include_dirs( int separator, const core::string & name );
	void	check_include_dirs( int separator, const char * name, size_t len = size_t(-1) );
	void	check_include_dir( const core::string & name );
	void	check_include_dir( const char * name, size_t len = size_t(-1) );

	// Check whether path is allowed
	const status_t	append_allowed_base_dir( const core::static_string & base_path );
	const status_t	remove_allowed_base_dir( const core::static_string & base_path );
	const status_t	remove_all_allowed_base_dirs();
	const core::Tarray<fs::path> &	get_allowed_base_dirs() const;

	const status_t	append_disallowed_base_dir( const core::static_string & base_path );
	const status_t	remove_disallowed_base_dir( const core::static_string & base_path );
	const status_t	remove_all_disallowed_base_dirs();
	const core::Tarray<fs::path> &	get_disallowed_base_dirs() const;

	bool	is_path_allowed( const core::static_string & path ) const;
protected:
	alias_desc_list_t		m_alias_desc_list;
	include_desc_list_t		m_include_desc_list;
	core::string_list_t		m_once_include_list;
	stdext::hash_set<std::string>	m_once_include_set;
	core::Tarray<fs::path>	m_allowed_base_dirs;
	core::Tarray<fs::path>	m_disallowed_base_dirs;

	// Trigraphs and digraphs
public:
	const status_t	append_digraph( char first_char, char second_char, char replacement_char );
	const status_t	append_digraphs( const char * digraphs, const char * replacement_chars, size_t count );
	const status_t	remove_digraph( char first_char, char second_char );
	const status_t	remove_digraphs( const char * digraphs, size_t count );

	const status_t	append_trigraph( char third_char, char replacement_char );
	const status_t	append_trigraphs( const char * third_chars, const char * replacement_chars, size_t count );
	const status_t	remove_trigraph( char third_char );
	const status_t	remove_trigraphs( const char * third_chars, size_t count );
	const status_t	get_trigraph( char third_char, char & result );

	const status_t	process_multigraphs( core::buffer & dst_buf, const core::stringslice & src_buf );
protected:
	core::string	m_digraph_map_first;
	core::string	m_digraph_map_second;

	core::string	m_trigraph_map_first;
	core::string	m_trigraph_map_second;

	// Universal character names (UCN)
public:
	//virtual const status_t	parse_ucn16_character( intmax_t & ch );
	//virtual const status_t	parse_ucn32_character( intmax_t & ch );
	const status_t	process_ucns( core::buffer & dst_buf, const core::stringslice & src_buf );

	// Parsing
public:
//	virtual const status_t	parse_text( pp::string & buf );

	int		peek_char( const char *& s, const char * end_s ) const;
	int		fetch_char( const char *& s, const char * end_s ) const;

	virtual bool			skip_spaces_and_comments( const char * & start, const char * end, f1::parser::Iparser * parser = nullptr );
	virtual const status_t	skip_spaces_and_comments( bool * skipped_something = nullptr );
	virtual bool			skip_real_spaces_and_comments( const char * & start, const char * end, f1::parser::Iparser * parser = nullptr );
	virtual const status_t	skip_real_spaces_and_comments( bool * skipped_something = nullptr );
	virtual bool			parse_comment( const char *& s, const char * end_s, f1::parser::Iparser * parser = nullptr );
	virtual bool			parse_comment();
	virtual const status_t	trim_spaces_and_comments( pp::string & buf, bool trim_newlines = false );

	virtual bool			is_ident_first_char( int c ) const;
	virtual bool			is_ident_char( int c ) const;
	virtual const status_t	parse_macro_identifier( pp::string & buf );
	virtual bool			parse_macro_identifier( pp::string & buf, const char *& s, const char * end_s, f1::parser::Iparser * parser = nullptr );
			const status_t	parse_macro_identifier( pp::string & buf, f1::parser::Iparser & parser );
			const status_t	on_unknown_macro_identifier( core::Tconst_ref<pp::Imacro> & res, const core::stringslice & ident ) const override;
	virtual bool			skip_directive_identifier( const char * string, const char *& s, const char * end_s );
	virtual const status_t	parse_directive_identifier( pp::string & buf );
	virtual bool			parse_directive_identifier( pp::string & buf, const char *& s, const char * end_s, f1::parser::Iparser * parser = nullptr );
			const status_t	parse_directive_identifier( pp::string & buf, f1::parser::Iparser & parser );
			//const status_t	on_unknown_directive_identifier( core::Tconst_ref<pp::Idirective> & res, const core::stringslice & ident ) const override;
	virtual bool			skip_definition_identifier( const char * string, const char *& s, const char * end_s );
	virtual const status_t	parse_definition_identifier( pp::string & buf );
	virtual bool			parse_definition_identifier( pp::string & buf, const char *& s, const char * end_s, f1::parser::Iparser * parser = nullptr );
			const status_t	parse_definition_identifier( pp::string & buf, f1::parser::Iparser & parser );
	virtual const status_t	parse_identifier();

	virtual const status_t	parse_directive();

	virtual const status_t	parse_integer( pp::variant & res, bool & parsed );
	virtual bool			parse_integer( pp::variant & res );
	virtual bool			parse_integer( pp::variant & res, const char *& s, const char * end_s );
	virtual bool			parse_integer( pp::variant & res, const pp::string & buf );
	virtual const status_t	parse_hex_integer( pp::variant & res, unsigned num_digits, bool & parsed );
	virtual bool			parse_hex_integer( pp::variant & res, unsigned num_digits, const char *& s, const char * end_s );
	virtual bool			parse_hex_integer( pp::variant & res, unsigned num_digits, const pp::string & buf );

	virtual const status_t	parse_float( pp::variant & res, bool & parsed );
	virtual bool			parse_float( pp::variant & res, const pp::string & buf );
	virtual bool			parse_float( pp::variant & res, const char *& s, const char * end_s );


	virtual const char *	get_directive_start() const override { return m_directive_start; }
	virtual const char *	get_directive_name_start() const override { return m_directive_name_start; }
protected:
	const char *	m_directive_start;
	const char *	m_directive_name_start;

	// String and character related methods
public:
	begin_scoped_enum( string_parse_flags_t )
		fl_parse_string_no_escapes		= 0x0001,
		fl_parse_string_ignore_unterm	= 0x0002,
		fl_parse_string_allow_nl		= 0x0004,
		fl_parse_string_raw				= 0x0008,	// raw literals in form "prefix(...)prefix"
	end_scoped_enum;
	enum char_encoding_t {
		ce_none,		// default (multi-byte)
		ce_widechar,	// wide char L prefix
		ce_utf8,		// UTF-8 char u8 prefix
		ce_utf16,		// UTF-16 char u prefix
		ce_utf32,		// UTF-32 char U prefix
		ce_count,		// number of encoding types
		ce_invalid = -1	// invalid string marker
	};
	const nls::Iencoding *	get_encoding( char_encoding_t encoding ) const;
		  nls::Iencoding *	get_encoding( char_encoding_t encoding );

			const status_t	parse_string( pp::string & buf, const char *& s, const char * end_s, string_parse_flags_t flags = string_parse_flags_t::null, char_encoding_t encoding = ce_none );
			const status_t	parse_string( pp::string & buf, int end_ch = 0, string_parse_flags_t flags = string_parse_flags_t::null, char_encoding_t encoding = ce_none );
			const status_t	skip_string( string_parse_flags_t flags = string_parse_flags_t::null, char_encoding_t encoding = ce_none );
			const status_t	skip_string( int end_ch, string_parse_flags_t flags = string_parse_flags_t::null, char_encoding_t encoding = ce_none );
			const status_t	skip_string( const char *& s, const char * end_s, pp::string * buf = nullptr, string_parse_flags_t flags = string_parse_flags_t::null, char_encoding_t encoding = ce_none );
	virtual const status_t	skip_string( const char *& s, const char * end_s, int end_ch, pp::string * buf, string_parse_flags_t flags = string_parse_flags_t::null, char_encoding_t encoding = ce_none );
	virtual const status_t	parse_string( int end_ch = 0, string_parse_flags_t flags = string_parse_flags_t::null, char_encoding_t encoding = ce_none );
	static const unsigned fl_stringify_slashes	= 0x0001;	//! stringify slashes, independently of quotes
	virtual const status_t	stringify_chars( pp::string & buf, const pp::string & src, int end_ch = 0, unsigned flags = 0 ) const;
	virtual const status_t	unstringify_chars( pp::string & buf, const pp::string & src, bool allow_non_strings = true ) const;
			const status_t	unstringify_chars( pp::string & buf, const char * s, size_t length, bool allow_non_strings = true ) const;
			//const status_t	format_string( pp::string & buf, const char *& s, const char * end_s, string_parse_flags_t flags = string_parse_flags_t::null, char_encoding_t encoding = ce_none );

	virtual bool	parse_string_header( const char * & s, const char * end_s, char_encoding_t & enc, int & end_ch, string_parse_flags_t & parse_flags );
	virtual bool	parse_string_header( char_encoding_t & enc, int & end_ch, string_parse_flags_t & parse_flags );
	virtual const status_t	merge_strings( string_parse_flags_t flags = string_parse_flags_t::null );
	virtual const status_t	merge_strings( char_encoding_t initial_encoding, int initial_end_ch, string_parse_flags_t flags = string_parse_flags_t::null );
	virtual int		get_end_quote( int ch ) const;

	//virtual const status_t	parse_escape( intmax_t & buf, size_t & nbytes );
	//typedef char char8_buf_t[8];
	virtual const status_t	parse_char( intmax_t & ch, const nls::Iencoding * target_encoding = nullptr );
	virtual const status_t	parse_char( intmax_t & ch, const char *& s, const char * end_s, const nls::Iencoding * target_encoding = nullptr );
	//typedef char char16_buf_t[2];
	virtual const status_t	parse_wchar( intmax_t & ch );
	virtual const status_t	parse_wchar( intmax_t & ch, const char *& s, const char * end_s );

	virtual const status_t	convert_char_to_integer( max_int_t & result, const pp::string & buf, const nls::Iencoding * target_encoding = nullptr );
	virtual const status_t	convert_char_to_variant( pp::variant & result, const pp::string & buf, const nls::Iencoding * target_encoding = nullptr );

	virtual const status_t	load_default_escapes();
	virtual const status_t	set_esc_char_value( int ch, int val );
	typedef const status_t (*esc_char_callback)( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );
	virtual const status_t	set_esc_char_callback( int ch, esc_char_callback fn );
	virtual const status_t	parse_esc_char( pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding = ce_none ) const;
protected:
	struct esc_desc_t {
		int		m_value;
		esc_char_callback	m_callback;
	};
	esc_desc_t	m_esc_chars[127];	// 0x00-0x7f
	bool		m_custom_esc_seqs;	// TODO: unused

	pp::string		m_char_parse_buffer;
	unicode::utf16le_string	m_char_parse_wbuffer;
	core::mbbuffer	m_char_encode_buffer;

	core::Tconst_ref<nls::Iencoding>	m_nls_utf8_encoding;
	core::Tconst_ref<nls::Iencoding>	m_nls_utf16_encoding;
	core::Tconst_ref<nls::Iencoding>	m_nls_utf32_encoding;

	const status_t	_write_numeric_with_encoding( pp::string & buf, char_encoding_t encoding, uint32_t uch );
	const status_t	_reverse_encoded_string( pp::string & str, char_encoding_t encoding );

	static const status_t	_esc_nl( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );
	static const status_t	_esc_cr( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );
	static const status_t	_esc_ctrl( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );
	static const status_t	_esc_html( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );
	static const status_t	_esc_ucn( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );
	static const status_t	_esc_u16( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );
	static const status_t	_esc_u32( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );
	static const status_t	_esc_dec( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );
	static const status_t	_esc_hex( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );
	static const status_t	_esc_oct( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );
	static const status_t	_esc_oct_new( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding );
	static const status_t	_esc_num( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, int radix, size_t nchars, char_encoding_t encoding );

	const status_t	find_ucn( pp::string & buf, const char * s, size_t len ) const;

public:
	const status_t	fetch_newline();
	virtual bool	skip_newline();
	virtual bool	skip_newline( const char *& s, const char * end_s, f1::parser::Iparser * parser = nullptr );
	virtual const status_t	skip_to_newline();
	virtual const status_t	skip_to_newline( const char *& s, const char * end_s );
	virtual const status_t	parse_to_newline( pp::string & buf, bool count_parenthesis = false );
	virtual const status_t	parse_newline();
	virtual bool	set_newline( bool newline = true );

	virtual const status_t	parse_delimited_string( const char * & s, const char * end_s, int term_ch );
	virtual const status_t	parse_delimited_string( pp::string & buf, int term_ch );

	virtual int		get_end_bracket( int ch ) const;

	static const unsigned fl_parse_args_allow_newlines	= 0x0001;
	//virtual const status_t	parse_arg( pp::string & arg, unsigned flags = 0 );
	//virtual const status_t	parse_arg_list( pp::arg_list & args, unsigned flags = 0 );
	virtual const status_t	parse_arg( pp::string & arg, const char *& s, const char * end_s, int end_ch, unsigned flags = 0 );
	virtual const status_t	parse_arg_list( pp::arg_list & args, const pp::param_list * params, const char *& s, const char * end_s, int end_ch = 0, unsigned flags = 0 );
			const status_t	parse_arg_list( pp::arg_list & args, const pp::param_list * params, int end_ch = 0, unsigned flags = 0 );
			const status_t	parse_arg_list( core::Tref<pp::arg_list> & args, const pp::param_list * params, const char *& s, const char * end_s, int end_ch = 0, unsigned flags = 0 );
			const status_t	parse_arg_list( core::Tref<pp::arg_list> & args, const pp::param_list * params, int end_ch = 0, unsigned flags = 0 );
	//virtual const status_t	parse_arg_list( pp::arg_list & args, pp::string & src );
	static const unsigned fl_parse_param_list_silent	= 0x0001;
	virtual const status_t	parse_param_list( pp::param_list & params, int end_ch = 0, unsigned flags = 0 );
	virtual const status_t	parse_param_list( core::Tref<pp::param_list> & params, int end_ch = 0, unsigned flags = 0 );
	virtual const status_t	on_invalid_param( unsigned flags = 0 );

	virtual const status_t	parse_include_multi_filename( pp::string & buf, c_pp::parser::include_flags_t & include_flags, const char * & s, const char * end_s, const char * string_separator = nullcstr );
	virtual const status_t	parse_include_multi_filename( pp::string & buf, c_pp::parser::include_flags_t & include_flags, const char * string_separator = nullcstr );
	virtual const status_t	parse_include_filename( pp::string & buf, c_pp::parser::include_flags_t & include_flags );
	virtual const status_t	parse_include_filename( pp::string & buf, c_pp::parser::include_flags_t & include_flags, const char *& s, const char * end_s );
	virtual const status_t	resolve_include_filename( core::string & full_file_name, const pp::string & file_name, include_flags_t include_flags ) const;
	virtual const status_t	resolve_include_filename( core::string & full_file_name, const core::stringslice & file_name, include_flags_t include_flags ) const;
	virtual const status_t	pass_on_include( include_flags_t include_flags, const pp::string & filename );
	//virtual const status_t	include_file( bool system_path, const char * file_path_name, size_t len = size_t(-1), core::Tref<f1::parser::Isource> * source = nullptr );
	//virtual const status_t	include_file( const core::string & full_file_path_name, bool system_path, const char * file, size_t len = size_t(-1), core::Tref<f1::parser::Isource> * source = nullptr );
	//virtual const status_t	include_file( const core::string & full_file_path_name, bool system_path, const pp::string & file_path_name, core::Tref<f1::parser::Isource> * source = nullptr );
	virtual const status_t	include_file( include_flags_t include_flags, const char * file_path_name, size_t len = size_t(-1), size_t offset = size_t(-1), include_desc ** inc_desc = nullptr );
	virtual const status_t	include_file( include_flags_t include_flags, const pp::string & file_path_name, size_t offset = size_t(-1), include_desc ** inc_desc = nullptr );
	virtual const status_t	include_file( const include_desc & inc_desc, size_t offset = size_t(-1) );
	virtual const status_t	on_include_not_found( include_flags_t include_flags, const pp::string & filename );

	//virtual const status_t	expand_macros( core::string & buf );
	virtual const status_t	start_char_seq();
	virtual const status_t	flush_chars();

	virtual const status_t	expand_macros( pp::string & buf, const pp::string & src );
	//virtual const status_t	expand_macros( pp::string & buf, const pp::string & src, pp::macro_list_t & macro_list );
	virtual const status_t	expand_macro( pp::Imacro & mac, const pp::arg_list * args = nullptr );
	//virtual const status_t	on_macro_expanded( pp::string & buf, pp::Imacro & mac, const pp::arg_list * args = nullptr );
	virtual const status_t	display_macro_location( const pp::Imacro & mac, unsigned msgid = syntax::IMSG_INFO_DEFINED_HERE );
	virtual const status_t	on_macro_expanded( pp::Imacro & mac ) override;

	// Expressions
public:
	/*begin_scoped_enum( operand_type_t )
		operand_type_null,
		operand_type_identifier,
		operand_type_numeric,
		operand_type_character,
		operand_type_expression,
	end_scoped_enum;*/
	struct operand {
		pp::variant			m_value;
		//core::stringrange	m_range;
		//operand_type_t		m_type;
		//core::Tref<pp::Imacro>	m_macro;
	};
	struct binary_operator_desc_t;

	virtual const status_t	parse_expression( pp::variant & result, const char * & string, const char * end, int end_ch = 0 );
	virtual const status_t		parse_integer_expression( uint64_t & result, const char * & string, const char * end = nullptr, int end_ch = 0 );
	virtual const status_t		parse_boolean_expression( bool & result, const char * & string, const char * end = nullptr, int end_ch = 0 );
	virtual const status_t	parse_expression( pp::variant & result, int end_ch = 0 );
	virtual const status_t	parse_expression( pp::variant & result, const pp::variant & left_value, const binary_operator_desc_t * left_op, const char * & string, const char * end, int end_ch );
	virtual const status_t	parse_operand( pp::variant & result, const char * & string, const char * end );

	static const unsigned fl_parse_value_ignore_unknown_idents	= 0x0001;
	virtual const status_t	parse_value( operand & opnd, const char * & s, const char * end_s, unsigned flags = 0 );
	virtual const status_t	parse_value( pp::variant & value, const char * & s, const char * end_s, unsigned flags = 0 );

	static const unsigned fl_compare_values_ignore_misspellings = 0x0001;	//! compare values ignoring parameters misspellings
	virtual bool	compare_values( const pp::string & value1, const pp::param_list * params1,
		const pp::string & value2, const pp::param_list * params2,
		unsigned flags = 0 );
	virtual const status_t	calc_value_hash( core::md5hash::digest_t & digest, const pp::string & value, const pp::param_list * params, unsigned flags = 0 );

	virtual const status_t	on_unknown_expression_macro( const pp::string & ident, pp::variant & value );


	typedef const status_t	execute_unary_operator_fn( c_pp::parser & par, operand & opnd );
	static const unsigned fl_uop_deprecated		= 0x0001;	///< operator is deprecated and issues a warning upon use
//	static const unsigned fl_op_disabled		= 0x0002;	///< operator is recognized but disabled
	struct unary_operator_desc_t {
		core::string	m_name;		// TODO: small_string?
		execute_unary_operator_fn *	m_execute;	// if points to unary_invalid_op, this operator is disabled for use
		unsigned		m_flags;
	};
	typedef core::Tarray<unary_operator_desc_t>	unary_operator_desc_list_t;

	static const status_t		unary_invalid_op( c_pp::parser & par, operand & opnd );
	static const status_t		unary_lnot( c_pp::parser & par, operand & opnd );
	static const status_t		unary_not( c_pp::parser & par, operand & opnd );
	static const status_t		unary_minus( c_pp::parser & par, operand & opnd );
	static const status_t		unary_plus( c_pp::parser & par, operand & opnd );
	const status_t	add_unary_operator( unary_operator_desc_list_t & op_list, execute_unary_operator_fn * execute, const char * name, size_t len = size_t(-1), unsigned flags = 0 );
	const status_t	remove_unary_operator( unary_operator_desc_list_t & op_list, execute_unary_operator_fn * execute );
	const status_t	remove_unary_operator( unary_operator_desc_list_t & op_list, const char * name, size_t len = size_t(-1) );
	virtual const unary_operator_desc_t *	parse_unary_operator( const unary_operator_desc_list_t & op_list, const char * s, const char * end );
			const unary_operator_desc_t *	parse_prefix_unary_operator( const char * s, const char * end );
			const unary_operator_desc_t *	parse_postfix_unary_operator( const char * s, const char * end );
			const unary_operator_desc_t *	parse_prefix_unary_operator();
			const unary_operator_desc_t *	parse_postfix_unary_operator();

	virtual const binary_operator_desc_t *	parse_binary_operator( const char * s, const char * end, f1::pp::variant::type_t left_operand_type );
	typedef const status_t	execute_operator_fn( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	typedef const status_t	preexecute_operator_fn( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const char * & s, const char * end );
	typedef const status_t	postexecute_operator_fn( c_pp::parser & par, pp::variant & result, const pp::variant & left_value );
	struct binary_operator_desc_t {
		const char *			m_name;
		uint16_t				m_len;
		uint16_t				m_priority;
		//const f1::guid_t	&	m_left_arg_type_guid;
		//const f1::guid_t	&	m_right_arg_type_guid;
		execute_operator_fn *		m_execute;		///< if points to binary_invalid_op, this operator is disabled for use
		preexecute_operator_fn *	m_preexecute;
		postexecute_operator_fn *	m_postexecute;
		unsigned				m_flags;
		pp::variant::type_t		m_left_arg_type;
		pp::variant::type_t		m_right_arg_type;
	};
	static const unsigned fl_bop_deprecated	= 0x0001;

	const status_t	add_binary_operators( const binary_operator_desc_t * descs, size_t count );
	const status_t	remove_binary_operators( const binary_operator_desc_t * descs, size_t count );

	static const status_t	binary_invalid_op( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );

	typedef  intmax_t	 int_binary_arith_op_fn	(  intmax_t		left_value,  intmax_t	right_value );
	typedef uintmax_t	uint_binary_arith_op_fn	( uintmax_t		left_value, uintmax_t	right_value );
	typedef max_float_t	float_binary_arith_op_fn( max_float_t	left_value, max_float_t	right_value );
	typedef const char *string_binary_arith_op_fn( const char *	left_value, const char *right_value );
	static const status_t	execute_binary_arith_op( c_pp::parser & par,
		int_binary_arith_op_fn * int_fn, uint_binary_arith_op_fn * uint_fn, float_binary_arith_op_fn * float_fn, 
		pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static const status_t	execute_add( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static intmax_t				execute_add_int( intmax_t left_value, intmax_t right_value );
	static uintmax_t			execute_add_uint( uintmax_t left_value, uintmax_t right_value );
	static max_float_t			execute_add_float( max_float_t left_value, max_float_t right_value );
	static const status_t	execute_sub( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static intmax_t				execute_sub_int( intmax_t left_value, intmax_t right_value );
	static uintmax_t			execute_sub_uint( uintmax_t left_value, uintmax_t right_value );
	static max_float_t			execute_sub_float( max_float_t left_value, max_float_t right_value );
	static const status_t	execute_mul( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static intmax_t				execute_mul_int( intmax_t left_value, intmax_t right_value );
	static uintmax_t			execute_mul_uint( uintmax_t left_value, uintmax_t right_value );
	static max_float_t			execute_mul_float( max_float_t left_value, max_float_t right_value );
	static const status_t	execute_div( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static intmax_t				execute_div_int( intmax_t left_value, intmax_t right_value );
	static uintmax_t			execute_div_uint( uintmax_t left_value, uintmax_t right_value );
	static max_float_t			execute_div_float( max_float_t left_value, max_float_t right_value );

	static const status_t	execute_cat( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );

	static const status_t	execute_seq( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );

	typedef intmax_t	int_binary_bitwise_op_fn(	intmax_t	left_value,  intmax_t	right_value );
	typedef uintmax_t	uint_binary_bitwise_op_fn( uintmax_t	left_value, uintmax_t	right_value );
	static const status_t	execute_binary_bitwise_op(
		int_binary_bitwise_op_fn * int_fn, uint_binary_bitwise_op_fn * uint_fn,
		pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static const status_t	execute_shl( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static intmax_t				execute_shl_int( intmax_t left_value, intmax_t right_value );
	static uintmax_t			execute_shl_uint( uintmax_t left_value, uintmax_t right_value );
	static const status_t	execute_shr( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static intmax_t				execute_shr_int( intmax_t left_value, intmax_t right_value );
	static uintmax_t			execute_shr_uint( uintmax_t left_value, uintmax_t right_value );
	static const status_t	execute_rem( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static intmax_t				execute_rem_int( intmax_t left_value, intmax_t right_value );
	static uintmax_t			execute_rem_uint( uintmax_t left_value, uintmax_t right_value );
	static const status_t	execute_and( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static intmax_t				execute_and_int( intmax_t left_value, intmax_t right_value );
	static uintmax_t			execute_and_uint( uintmax_t left_value, uintmax_t right_value );
	static const status_t	execute_or( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static intmax_t				execute_or_int( intmax_t left_value, intmax_t right_value );
	static uintmax_t			execute_or_uint( uintmax_t left_value, uintmax_t right_value );
	static const status_t	execute_xor( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static intmax_t				execute_xor_int( intmax_t left_value, intmax_t right_value );
	static uintmax_t			execute_xor_uint( uintmax_t left_value, uintmax_t right_value );

	typedef bool	bool_binary_compare_op_fn	( bool			left_value, bool		right_value );
	typedef bool	int_binary_compare_op_fn	( intmax_t		left_value, intmax_t	right_value );
	typedef bool	uint_binary_compare_op_fn	( uintmax_t		left_value, uintmax_t	right_value );
	typedef bool	float_binary_compare_op_fn	( max_float_t	left_value, max_float_t	right_value );
	static const status_t	execute_binary_compare_op( c_pp::parser & par,
		bool_binary_compare_op_fn * bool_fn, int_binary_compare_op_fn * int_fn, uint_binary_compare_op_fn * uint_fn, float_binary_compare_op_fn * float_fn, 
		pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static const status_t	execute_equ( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static bool		execute_equ_bool( bool left_value, bool right_value );
	static bool		execute_equ_int( intmax_t left_value, intmax_t right_value );
	static bool		execute_equ_uint( uintmax_t left_value, uintmax_t right_value );
	static bool		execute_equ_float( max_float_t left_value, max_float_t right_value );
	static const status_t	execute_not_equ( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static bool		execute_not_equ_bool( bool left_value, bool right_value );
	static bool		execute_not_equ_int( intmax_t left_value, intmax_t right_value );
	static bool		execute_not_equ_uint( uintmax_t left_value, uintmax_t right_value );
	static bool		execute_not_equ_float( max_float_t left_value, max_float_t right_value );
	static const status_t	execute_greater( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static bool		execute_greater_bool( bool left_value, bool right_value );
	static bool		execute_greater_int( intmax_t left_value, intmax_t right_value );
	static bool		execute_greater_uint( uintmax_t left_value, uintmax_t right_value );
	static bool		execute_greater_float( max_float_t left_value, max_float_t right_value );
	static const status_t	execute_greater_equ( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static bool		execute_greater_equ_bool( bool left_value, bool right_value );
	static bool		execute_greater_equ_int( intmax_t left_value, intmax_t right_value );
	static bool		execute_greater_equ_uint( uintmax_t left_value, uintmax_t right_value );
	static bool		execute_greater_equ_float( max_float_t left_value, max_float_t right_value );
	static const status_t	execute_less( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static bool		execute_less_bool( bool left_value, bool right_value );
	static bool		execute_less_int( intmax_t left_value, intmax_t right_value );
	static bool		execute_less_uint( uintmax_t left_value, uintmax_t right_value );
	static bool		execute_less_float( max_float_t left_value, max_float_t right_value );
	static const status_t	execute_less_equ( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static bool		execute_less_equ_bool( bool left_value, bool right_value );
	static bool		execute_less_equ_int( intmax_t left_value, intmax_t right_value );
	static bool		execute_less_equ_uint( uintmax_t left_value, uintmax_t right_value );
	static bool		execute_less_equ_float( max_float_t left_value, max_float_t right_value );
	static const status_t	execute_land( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static bool		execute_land_bool( bool left_value, bool right_value );
	static bool		execute_land_int( intmax_t left_value, intmax_t right_value );
	static bool		execute_land_uint( uintmax_t left_value, uintmax_t right_value );
	static bool		execute_land_float( max_float_t left_value, max_float_t right_value );
	static const status_t	execute_lor( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );
	static bool		execute_lor_bool( bool left_value, bool right_value );
	static bool		execute_lor_int( intmax_t left_value, intmax_t right_value );
	static bool		execute_lor_uint( uintmax_t left_value, uintmax_t right_value );
	static bool		execute_lor_float( max_float_t left_value, max_float_t right_value );
	static const status_t	execute_select( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const pp::variant & right_value );

	static const status_t	preexecute_land( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const char * & s, const char * end );
	static const status_t	postexecute_land( c_pp::parser & par, pp::variant & result, const pp::variant & left_value );

	static const status_t	preexecute_lor( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const char * & s, const char * end );
	static const status_t	postexecute_lor( c_pp::parser & par, pp::variant & result, const pp::variant & left_value );

	static const status_t	preexecute_select( c_pp::parser & par, pp::variant & result, const pp::variant & left_value, const char * & s, const char * end );
	static const status_t	postexecute_select( c_pp::parser & par, pp::variant & result, const pp::variant & left_value );
protected:
	unary_operator_desc_list_t		m_prefix_unary_operator_desc_list;
	unary_operator_desc_list_t		m_postfix_unary_operator_desc_list;

	core::Tarray<binary_operator_desc_t>	m_binary_operator_desc_list;
	static const binary_operator_desc_t		m_default_binary_operator_desc[28];

	// Commands
public:
	struct F1_PP_API modify_c_flags_parser_command : public pp::Iparser::Icommand {
		explicit modify_c_flags_parser_command( c_flags_t flags_to_set, c_flags_t flags_to_clear = c_flags_t::null );
		virtual const status_t	execute( pp::Iparser & parser ) override;
		c_flags_t	m_flags_to_set, m_flags_to_clear;
	};
	struct F1_PP_API append_known_pragma_command : public pp::Iparser::Icommand {
		explicit append_known_pragma_command( const core::const_string & ident, bool append );
		virtual const status_t	execute( pp::Iparser & parser ) override;
		core::const_string	m_ident;
		bool m_append;
	};
	struct F1_PP_API append_allowed_base_path_command : public pp::Iparser::Icommand {
		explicit append_allowed_base_path_command( const core::const_string & path, bool append );
		virtual const status_t	execute( pp::Iparser & parser ) override;
		core::const_string	m_path;
		bool m_append;
	};
	struct F1_PP_API append_disallowed_base_path_command : public pp::Iparser::Icommand {
		explicit append_disallowed_base_path_command( const core::const_string & path, bool append );
		virtual const status_t	execute( pp::Iparser & parser ) override;
		core::const_string	m_path;
		bool m_append;
	};
};

/*END OF parser.hxx*/
