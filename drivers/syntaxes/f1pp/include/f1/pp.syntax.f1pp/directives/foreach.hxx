/*foreach.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::foreach_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective			super;
	typedef f1::f1pp::foreach_directive	self;

	explicit foreach_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
protected:
	const status_t			execute( f1::pp::Iparser & parser, const core::buffer & out_buf );
	static const status_t	line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );
public:
	struct frame : public f1::f1pp::Iscoped_frame {
		typedef f1::f1pp::Iscoped_frame		super;
		typedef frame						self;

		explicit frame( f1pp::parser & par );

		core::Tweak_ref<pp::Imacro>		m_control_macro;
		f1::parser::Iparser::position	m_position;
		core::buffer				m_buf;
		core::Tarray<pp::string>	m_elements;
		core::Tarray<pp::variant>	m_values;
		size_t		m_index;
		size_t		m_count;
		uint32_t	m_iteration_count;
		bool		m_raw;
		bool		m_backward;

		uint8_t	m_iterator_area[__max(sizeof( pp::variant::const_iterator ), sizeof( pp::variant::const_reverse_iterator ))];
		union {
			pp::variant::const_iterator	*			m_forward_iterator;
			pp::variant::const_reverse_iterator	*	m_reverse_iterator;
		};


		virtual const char *	get_openning_directive_name() const override;
		virtual const char *	get_closing_directive_name() const override;
		virtual const f1::guid_t & get_openning_directive_guid() const override;
		virtual const f1::guid_t & get_closing_directive_guid() const override;

		const status_t	_update( f1::f1pp::parser & f1par );
		bool	get_next_item( pp::variant & value );
	};
	virtual pp::Iframe *	alloc_idle_frame( f1::pp::Iparser & parser ) override;
};

/*END OF foreach.hxx*/
