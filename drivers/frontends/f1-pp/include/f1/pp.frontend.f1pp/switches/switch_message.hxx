/*switch_message.hxx*/

class F1_PP_FRONTEND_F1PP_API f1::pp::frontend::f1pp::switch_message : public f1::util::cmdline::Iswitch
{
public:
	typedef f1::util::cmdline::Iswitch				super;
	typedef f1::pp::frontend::f1pp::switch_message	self;

	explicit switch_message( unsigned flags = 0 ) NOEXCEPT;

	virtual const char *	get_name() const NOEXCEPT override;
	virtual const char *	get_long_name() const NOEXCEPT override;
	virtual const char *	get_desc() const NOEXCEPT override;
	//virtual const char *	get_params() const NOEXCEPT override;
	virtual const char *	get_help_text() const NOEXCEPT override;

	virtual status_t	process( char switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) NOEXCEPT override;

	//const status_t	apply( pp::context & ctx );

protected:
	struct cmd_message_group_state : public pp::Iparser::Icommand {
		virtual const status_t	execute( pp::Iparser & parser ) override;

		struct message_range_desc {
			msgout::msgid_t	m_first_message;
			unsigned		m_count;
			static const unsigned fl_enable_message		= 0x0001;
			static const unsigned fl_disable_message	= 0x0002;
			unsigned		m_flags;
			msgout::severity_t	m_severity;
		};
		core::Tarray<message_range_desc>	m_message_ranges;
	};

public:
	//bool	m_value;
};

/*END OF switch_message.hxx*/
