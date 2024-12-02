/*context.hxx*/

class F1_PP_DBG_API f1::pp::dbg::context : public core::named_object
{
	context( const context & );
public:
	typedef f1::core::named_object	super;
	typedef f1::pp::dbg::context	self;

	explicit context( const core::Iidentifier * ident = nullptr ) noexcept;
			~context();

	const status_t	initialize() noexcept;

	// Properties
public:
	const f1::pp::context *		get_pp_context() const noexcept;
		  f1::pp::context *		get_pp_context()	   noexcept;

	const f1::pp::Iparser *		get_pp_parser() const noexcept;
		  f1::pp::Iparser *		get_pp_parser()		  noexcept;

	const f1::parser::context *	get_parser_context() const;
		  f1::parser::context *	get_parser_context();

	enum working_state_t {
		ws_uninitialized,
		ws_launched,	// session is started
		ws_finished,	// session is finished
		ws_stopped,		// preprocessing is not started
		ws_paused,		// preprocessing is started and paused
		ws_running,		// preprocessing is currently performing
		ws_single_step,	// stop at the next event
		ws_abort,		// pending abort
		ws_state_count
	};
	working_state_t		get_state() const noexcept;
	virtual const status_t	set_state( working_state_t state ) noexcept;
protected:
	core::Tref<mt::Ithread>		m_thread;
	working_state_t				m_state;

	struct F1_PP_DBG_API pp_context : public pp::context
	{
		typedef pp::context	super;
		typedef pp_context	self;
		explicit pp_context( dbg::context & dbg_context );
		virtual const status_t	on_parser_attached( pp::Iparser & parser ) override;
		virtual const status_t	on_parser_detached( pp::Iparser & parser ) override;
		core::Tweak_ref<dbg::context> m_dbg_context;
	};
	core::Tref<pp_context>		m_pp_context;
	core::Tref<pp::Iparser>		m_pp_parser;

	class F1_PP_DBG_API server_task : public mt::Itask
	{
		virtual const status_t	run( void * param, mt::Ithread_context * tc ) override;
		virtual void			query_stop( mt::Ithread_context * tc ) override;
	};
	server_task					m_server_task;

	struct pp_event_handler : public pp::context::Ievent_handler
	{
		virtual void	invoke( pp::context::event_id_t id, void * param ) override;
		virtual void	on_detach( pp::context & ctx );
		core::Tweak_ref<dbg::context> m_dbg_context;
	};
	pp_event_handler	m_pp_event_handler;

	// Sources
public:
	struct option_desc {
		f1::guid_t		m_guid;		///< option GUID
		core::string	m_name;		///< option name, preserved in case the option is deleted
		pp::variant		m_value;	///< option value
	};
	struct source_desc {
		core::string	m_filename;

		// pp::Ifrontend information
		f1::guid_t		m_frontend_guid;
		core::string	m_frontend_switches;

		// pp::context information
		unsigned		m_pp_context_flags;
		f1::guid_t		m_syntax_guid;		//+
		f1::guid_t		m_default_standard;	//+
		core::Tarray<f1::guid_t>	m_disabled_syntaxes;	//+
		core::Tarray<f1::guid_t>	m_standards;	//+
		pp::context::newline_type_t	m_newline_type;	//+
		core::string	m_message_base_path;	//+

		intmax_t		m_limit[pp::limit_t::li_count];	//+
		f1::nls::encoding_id_t		m_character_encoding;		//+
		f1::nls::encoding_id_t		m_default_input_encoding;	//+
		f1::nls::encoding_id_t		m_input_encoding;	// +pp::Iparser
		core::Tarray<pp::context::include_file_t>	m_include_files;	//+
		core::Tarray<pp::context::macro_def_t>		m_macros_to_define;	//+
		core::Tarray<core::string>					m_macros_to_undefine;	//+
		core::Tarray<core::string>					m_macros_to_deprecate;	//+ TODO
		core::Tarray<option_desc>	m_options;

		// Output
		pp::context::output_flags_t	m_output_flags;	//+
		core::string	m_output_base_path;		//+
		core::string	m_output_file_name;	//+
		core::string	m_output_file_ext;
		core::string	m_output_directory;	//+
		f1::guid_t		m_output_class;		//+
		f1::nls::encoding_id_t		m_default_output_encoding;	//+
		f1::nls::encoding_id_t		m_output_encoding;	// +pp::Iparser
//		f1::guid_t		m_storage_class;	//-

		core::Tarray<pp::context::dump_task>	m_dump_tasks;
		pp::context::Efilename_print			m_filename_print;

		// c_pp::parser information
		f1::c_pp::parser::c_flags_t	m_c_pp_parser_flags;
		core::Tarray<c_pp::parser::include_desc>	m_include_descs;
		fs::search_dir_list		m_sys_search_dir_list;
		fs::search_dir_list		m_usr_search_dir_list;
	};
	typedef core::Tarray<source_desc>	source_desc_list_t;

	const status_t	append_source( const core::string & name );
	const status_t	remove_source( const core::string & name );
	const status_t	remove_all_sources();

	source_desc_list_t::const_iterator	find_source_desc( const core::string & name ) const;
	source_desc_list_t::iterator		find_source_desc( const core::string & name );

	const source_desc *	get_source_desc( const core::string & name ) const;
		  source_desc *	get_source_desc( const core::string & name );

	const source_desc_list_t &	get_source_desc_list() const;
		  source_desc_list_t &	get_source_desc_list();
protected:
	source_desc_list_t	m_source_desc_list;

	// Breakpoints
public:
protected:

	// Operations
public:
	const status_t	begin_session( f1::parser::Isource & src );	// ws_uninitialized -> ws_stopped
//	const status_t		continue_session();						// ws_stopped -> ws_paused
	const status_t		abort_session();						// ws_* -> ws_stopped
	const status_t		resume_session();						// ws_paused -> ws_running
	const status_t		step_session();							// ws_paused -> ws_running -> ws_paused
	const status_t	end_session();								// * -> ws_uninitialized

//	const status_t	reset();
//	const status_t	save( const char * pathname );
//	const status_t	load( const char * pathname );
protected:
};

/*END OF context.hxx*/
