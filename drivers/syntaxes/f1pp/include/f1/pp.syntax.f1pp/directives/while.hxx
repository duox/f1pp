/*while.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::while_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective		super;
	typedef f1::f1pp::while_directive	self;

	struct frame;

	explicit while_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
			const status_t	_update_condition( f1::c_pp::parser & parser, frame & frm );
protected:
	const status_t			execute( f1::pp::Iparser & parser, const core::buffer & out_buf );
	static const status_t	line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );
public:
	struct frame : public f1::f1pp::Iscoped_frame {
		typedef f1::f1pp::Iscoped_frame	super;
		typedef frame							self;

		explicit frame( f1pp::parser & par );

		core::Tref<while_directive>		m_while_directive;
		core::Tref<pp::Imacro>			m_control_macro;
		//f1::parser::Iparser::position	m_position;		// position where expression is started
		//f1::parser::Iparser::position	m_end_position;	// position after the `#end while' directive
		const char *	m_position;
		const char *	m_end_position;
		uint32_t		m_iteration_count;

		virtual const char *	get_openning_directive_name() const override;
		virtual const char *	get_closing_directive_name() const override;
		virtual const f1::guid_t & get_openning_directive_guid() const override;
		virtual const f1::guid_t & get_closing_directive_guid() const override;
	};
	virtual pp::Iframe *	alloc_idle_frame( f1::pp::Iparser & parser ) override;
};

/*END OF while.hxx*/
