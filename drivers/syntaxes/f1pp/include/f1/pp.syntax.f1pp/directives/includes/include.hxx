/*include.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::include_directive : public f1::c_pp::c99::include_directive
{
public:
	typedef f1::c_pp::c99::include_directive	super;
	typedef f1::f1pp::include_directive		self;

	explicit include_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
	virtual const status_t	execute( f1::c_pp::parser & c_pp_par, const char * & s, const char * end_s ) override;
protected:
			const status_t	parse( pp::Iparser & parser, bool first_run );

	f1::core::Tref<f1::pp::Imacro>	m_target_macro;
	begin_scoped_enum( attribute_flags_t )
		fl_attr_append		= 0x0001,
		fl_attr_once		= 0x0002,
		fl_attr_multi		= 0x0004,	// TODO: ignored
		fl_attr_next		= 0x0008,	// TODO
		fl_attr_no_output	= 0x0010,
		fl_attr_raw			= 0x0020,
		fl_attr_silent		= 0x0040,
	end_scoped_enum;
	unsigned	m_attributes;

	struct include_task
	{
		f1::pp::string						m_file_path_name;
		f1::core::Tref<f1::pp::Imacro>		m_target_macro;
		f1::core::Tref<f1::pp::arg_list>	m_args;
		c_pp::parser::include_flags_t		m_include_flags;
		unsigned	m_attributes;
	};
	core::Tarray<include_task> m_include_tasks;
	const status_t	execute_tasks( f1::f1pp::parser & f1par, f1::parser::Iparser & par );

	struct parser_event_handler : public f1::parser::Ievent_handler
	{
		virtual const status_t	invoke( f1::parser::event_type_t type_mask, void * handler_param, void * event_param, f1::parser::Iparser & parser ) override;
		core::Tweak_ref<f1pp::include_directive>	m_include;
		core::Tweak_ref<f1::parser::Isource>		m_parent_source;
		core::Tref<pp::Imacro>			m_macro;
		core::Tref<pp::arg_list>		m_args;
		core::Tweak_ref<pp::Iparser>	m_parser;
		//size_t	m_last_source_end_offset;
		core::Tarray<include_task>		m_include_tasks;
		core::Tarray<include_task>::iterator	m_include_tasks_iterator;
		unsigned	m_attributes;
	} m_parser_event_handler;

	// Hooks
protected:
	virtual const status_t	include_file( f1::c_pp::parser & par, const pp::string & file_path_name, c_pp::parser::include_flags_t include_flags, c_pp::parser::include_desc ** inc_desc_ptr = nullptr ) override;
//	virtual const status_t	on_complete( f1::c_pp::parser & par, const char *& s, const char * end_s ) override;
};

/*END OF include.hxx*/
