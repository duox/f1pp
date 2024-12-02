/*application.hxx*/

class F1_PP_FRONTEND_F1PP_API f1::pp::frontend::f1pp::application : public f1::pp::frontend::application
{
public:
	typedef f1::pp::frontend::application			super;
	typedef f1::pp::frontend::f1pp::application		self;

	application();

	status_t preinitialize_application() noexcept override;
	status_t initialize_application() noexcept override;
	status_t run_application() noexcept override;
	status_t uninitialize_application() noexcept override;

	const f1::pp::context *	get_context() const;
		  f1::pp::context *	get_context();

	static f1pp::application * get_app() { return static_cast<f1pp::application *>(g_app); }

	virtual const status_t	get_version( core::version & ver ) const NOEXCEPT override;

//	void	print( pcsz_t file, long line, message_id message_id, va_list ap );

	status_t	parse_encoding( core::Tconst_ref<nls::Iencoding> & enc, const char * name, size_t len );

	status_t	on_unrecognized_switch( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & param ) noexcept override;
	status_t	on_filename( const f1::core::const_string & filename ) noexcept override;

	status_t switch_help( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_nologo( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_quiet( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_ver( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_verbose( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_warn( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );

	status_t switch_out( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_outdir( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );

	status_t switch_comments( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_compress_whitespace( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	//status_t switch_define( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	//status_t switch_endianness( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	//status_t switch_input_encoding( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	//status_t switch_output_encoding( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	//status_t switch_full_paths( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	//status_t switch_include( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	//status_t switch_include_dir( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	//status_t switch_keep_comments( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_maximum_char_len( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_maximum_ident_len( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_maximum_errors( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_maximum_warnings( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_maximum_iterations( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_maximum_recursion( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	status_t switch_nested_comments( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );
	//status_t switch_undefine( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest );

	const status_t	set_default_output_guid( const f1::guid_t & guid );
	const f1::guid_t & get_default_output_guid() const;
	const status_t	set_output_class( pp::Ioutput_class & output_class );
	const status_t	set_output_class_guid( const f1::guid_t & guid );
private:
	f1::util::cmdline::posix_parser m_cmdline_parser;

	f1::guid_t	m_default_output_class_guid;

	uint64_t	m_total_time;

	//f1::pp::output::gnucpp::output_class	m_gnucpp_output_class;
	f1::pp::output::nullout::output_class	m_nullout_output_class;
	f1::pp::output::linenum::output_class	m_linenum_output_class;
	f1::pp::output::ppline::output_class	m_ppline_output_class;
 	f1::pp::output::puretext::output_class	m_puretext_output_class;

	f1::pp::service		m_pp_service;
	core::Tref<f1::pp::context>	m_context;
//	core::Tref<f1::pp::Isyntax>	m_syntax;
//	core::Tref<f1::pp::Ioutput>	m_output;
	f1::c_pp::syntax	m_syntax_cpp;
	f1::f1pp::syntax	m_syntax_f1pp;

	class pp_context_event_handler : public f1::pp::context::Ievent_handler {
		virtual void	invoke( pp::context::event_id_t event_id, void * param ) override;
	} m_pp_context_event_handler;

	c_pp::c99::standard		m_standard_c99;
	c_pp::c11::standard		m_standard_c11;
	c_pp::c23::standard		m_standard_c23;
	c_pp::cpp98::standard	m_standard_cpp98;
	c_pp::cpp11::standard	m_standard_cpp11;
	c_pp::cpp14::standard	m_standard_cpp14;
	c_pp::cpp17::standard	m_standard_cpp17;
	c_pp::cpp20::standard	m_standard_cpp20;
	c_pp::cpp23::standard	m_standard_cpp23;
	c_pp::clang::standard	m_standard_clang;
	c_pp::comext::standard	m_standard_comext;
	c_pp::gcc::standard		m_standard_gcc;
	c_pp::msvc::standard	m_standard_msvc;

	core::Tref<util::cmdline::switch_group> m_group_general;
		switch_debug					m_switch_debug;
		switch_syntax_check_only		m_switch_syntax_check_only;
		switch_timings					m_switch_timings;
	core::Tref<util::cmdline::switch_group>	m_group_include;
		switch_auto_once_includes		m_switch_auto_once_includes;
		switch_base_dir_allow			m_switch_base_dir_allow;
		switch_base_dir_disallow		m_switch_base_dir_disallow;
		switch_deprecate_include		m_switch_deprecate_include;
		switch_deprecate_system_include	m_switch_deprecate_system_include;
		switch_include					m_switch_include;
		switch_include_dir				m_switch_include_dir;
		switch_system_include_base		m_switch_system_include_base;
		switch_system_include_dir		m_switch_system_include_dir;
		switch_user_include_dir			m_switch_user_include_dir;
	core::Tref<util::cmdline::switch_group> m_group_input;
		switch_atsigns_in_identifiers	m_switch_atsigns_in_identifiers;
		switch_character_encoding		m_switch_character_encoding;
		switch_digraphs					m_switch_digraphs;
		switch_directive				m_switch_directive;
		switch_dollars_in_identifiers	m_switch_dollars_in_identifiers;
		switch_endianness				m_switch_endianness;
		switch_input_encoding			m_switch_input_encoding;
		switch_long_wide_char			m_switch_long_wide_char;
		switch_known_pragma				m_switch_known_pragma;
		switch_standard					m_switch_standard;
		switch_trigraphs				m_switch_trigraphs;
		switch_unsigned_char			m_switch_unsigned_char;
	core::Tref<util::cmdline::switch_group> m_group_macros;
		switch_define					m_switch_define;
		switch_ignore_macros			m_switch_ignore_macros;
		switch_macro					m_switch_macro;
		switch_undefine					m_switch_undefine;
	core::Tref<util::cmdline::switch_group> m_group_messages;
		switch_dump_stack_on_error		m_switch_dump_stack_on_error;
		switch_dump_stack_on_warning	m_switch_dump_stack_on_warning;
		switch_dump_macros_on_error		m_switch_dump_macros_on_error;
		switch_dump_macros_on_warning	m_switch_dump_macros_on_warning;
		switch_export_messages			m_switch_export_messages;
		switch_full_paths				m_switch_full_paths;
		switch_import_messages			m_switch_import_messages;
		switch_message					m_switch_message;
		switch_message_filename_base	m_switch_message_filename_base;
		switch_message_filename_only	m_switch_message_filename_only;
		switch_message_severity			m_switch_message_severity;
	core::Tref<util::cmdline::switch_group> m_group_output;
		switch_disable_output			m_switch_disable_output;
		switch_dump						m_switch_dump;
		switch_output_filename			m_switch_output_filename;
		switch_output_filename_base		m_switch_output_filename_base;
		switch_output_filename_only		m_switch_output_filename_only;
		switch_keep_comments			m_switch_keep_comments;
		switch_keep_doxygen_comments	m_switch_keep_doxygen_comments;
		switch_merge_strings			m_switch_merge_strings;
		switch_newline					m_switch_newline;
		switch_output_encoding			m_switch_output_encoding;
		switch_output_format			m_switch_output_format;
		switch_output_full_paths		m_switch_output_full_paths;
		switch_print_filename			m_switch_print_filename;
		switch_write_bom				m_switch_write_bom;
};

/*END OF application.hxx*/
