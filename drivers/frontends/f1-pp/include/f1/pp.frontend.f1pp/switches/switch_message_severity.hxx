/*switch_message_severity.hxx*/

class F1_PP_FRONTEND_F1PP_API f1::pp::frontend::f1pp::switch_message_severity : public f1::util::cmdline::Iswitch
{
public:
	typedef f1::util::cmdline::Iswitch					super;
	typedef f1::pp::frontend::f1pp::switch_message_severity	self;

	explicit switch_message_severity( unsigned flags = 0 ) NOEXCEPT;

	virtual const char *	get_name() const NOEXCEPT override;
	virtual const char *	get_long_name() const NOEXCEPT override;
	virtual const char *	get_desc() const NOEXCEPT override;
	//virtual const char *	get_params() const NOEXCEPT override;
	virtual const char *	get_help_text() const NOEXCEPT override;

	virtual status_t	process( char switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) NOEXCEPT override;

	//const status_t	apply( pp::context & ctx );

	struct change_message_severity : public pp::Iparser::Icommand {
		change_message_severity( unsigned msgid, msgout::severity_t severity );
		virtual const status_t	execute( pp::Iparser & parser ) override;
		msgout::msgid_t		m_msgid;
		msgout::severity_t	m_severity;
	};
protected:

public:
	//bool	m_value;
};

/*END OF switch_message_severity.hxx*/
