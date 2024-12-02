/*Iparser.hxx*/

interface F1_PP_API f1::pp::Iframe
{
	typedef Iframe		self;

	explicit Iframe( pp::Iparser & par );
	virtual ~Iframe();
	virtual void	delete_self();

	static const unsigned fl_initially_disabled	= 0x0001;	///< this frame execution was initially disabled
	static const unsigned fl_restart_iteration	= 0x0002;	///< restart iteration
	static const unsigned fl_abort_statement	= 0x0004;	///< abort statement's execution
	static const unsigned fl_idle				= 0x0008;	///< the frame is idle
	const status_t	modify_flags( unsigned flags_to_set, unsigned flags_to_clear = 0, unsigned * flags_prev = nullptr );
	unsigned	get_flags( unsigned mask = unsigned(-1) ) const;

	void	add_ref();
	void	release();

	void	inc_nesting();
	void	dec_nesting();
	unsigned	get_nesting() const;

	virtual const char *	get_openning_directive_name() const abstract;
	virtual const char *	get_closing_directive_name() const abstract;
	virtual const f1::guid_t & get_openning_directive_guid() const abstract;
	virtual const f1::guid_t & get_closing_directive_guid() const abstract;
	virtual const status_t	enter();
	virtual const status_t	leave();
	const f1::parser::Iparser::location &	get_initial_location() const { return m_location; }
		  f1::parser::Iparser::location &	get_initial_location()		 { return m_location; }

	const status_t	set_warning_flags( unsigned flags );
	unsigned		get_warning_flags( unsigned mask ) const;
protected:
	core::Tweak_ref<pp::Iparser>	m_parser;
	parser::Iparser::location		m_location;
	unsigned	m_ref_count;		// parser is always single-threaded, so no atomics
	unsigned	m_nesting_count;	// number of ignored top-level directives
	bitflags_t	m_warning_flags;	// flagged errors
private:
	bitflags_t	m_flags;			// state flags
};

/*END OF Iparser.hxx*/
