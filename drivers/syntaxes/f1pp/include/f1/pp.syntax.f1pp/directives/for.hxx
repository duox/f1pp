/*for.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::for_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective		super;
	typedef f1::f1pp::for_directive	self;

	struct frame;

	explicit for_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
//			const status_t	_update_condition( f1::c_pp::parser & parser, frame & frm );
protected:
	friend class endfor_directive;
	const status_t			on_init_finished( frame & frm, f1::f1pp::parser & f1par );
	//const status_t			execute_init( f1::pp::Iparser & parser, const core::buffer & out_buf );
	const status_t			execute_cond( f1::pp::Iparser & parser, const core::buffer & out_buf );
	//const status_t			execute_step( f1::pp::Iparser & parser, const core::buffer & out_buf );
	const status_t			_update_condition( frame & frm, f1::f1pp::parser & f1par );
	//static const status_t	init_line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );
	static const status_t	cond_line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );
	//static const status_t	step_line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );
public:
	struct for_directive_breakpoint_callback : public f1::parser::Iparser::Ibreakpoint_callback {
		frame *			m_frm;
		core::Tweak_ref<f1pp::parser>	m_f1par;
	};
	struct init_breakpoint_callback : public for_directive_breakpoint_callback {
		virtual const status_t	on_breakpoint_reached() override;
	};
	struct cond_breakpoint_callback : public for_directive_breakpoint_callback {
		virtual const status_t	on_breakpoint_reached() override;
	};
	//struct set_breakpoint_callback : public for_directive_breakpoint_callback {
	//	virtual const status_t	on_breakpoint_reached() override;
	//};
	virtual pp::Iframe *	alloc_idle_frame( f1::pp::Iparser & parser ) override;
};

struct F1_SYNTAX_F1PP_API f1::f1pp::for_directive::frame : public f1::f1pp::Iscoped_frame {
	typedef f1::f1pp::Iscoped_frame		super;
	typedef frame						self;

	explicit frame( f1pp::parser & par );
			~frame();

	core::Tweak_ref<for_directive>	m_for_directive;
	const char * /*f1::parser::Iparser::position*/	m_position;		// position where expression is started
	const char * /*f1::parser::Iparser::position*/	m_end_position;	// position after the `#end for' directive
	cond_breakpoint_callback		m_cond_breakpoint_callback;
	init_breakpoint_callback		m_init_breakpoint_callback;
	//set_breakpoint_callback			m_set_finish_callback;
	const char *	m_cond_start;
	size_t			m_cond_length;
	const char *	m_step_start;
	size_t			m_step_length;
	bool		m_parenthesis;
	uint32_t	m_iteration_count;
	unsigned	m_error_count;

	virtual const char *	get_openning_directive_name() const override;
	virtual const char *	get_closing_directive_name() const override;
	virtual const f1::guid_t & get_openning_directive_guid() const override;
	virtual const f1::guid_t & get_closing_directive_guid() const override;
};

/*END OF for.hxx*/
