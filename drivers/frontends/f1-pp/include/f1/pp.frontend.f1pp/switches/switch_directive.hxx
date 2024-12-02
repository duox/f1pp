/*switch_directive.hxx*/

class F1_PP_FRONTEND_F1PP_API f1::pp::frontend::f1pp::switch_directive : public f1::util::cmdline::Iswitch
{
public:
	typedef f1::util::cmdline::Iswitch				super;
	typedef f1::pp::frontend::f1pp::switch_directive	self;

	explicit switch_directive( unsigned flags = 0 ) NOEXCEPT;

	virtual const char *	get_name() const NOEXCEPT override;
	virtual const char *	get_long_name() const NOEXCEPT override;
	virtual const char *	get_desc() const NOEXCEPT override;
	//virtual const char *	get_params() const NOEXCEPT override;
	virtual const char *	get_help_text() const NOEXCEPT override;

	virtual status_t	process( char switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) NOEXCEPT override;
//	virtual const status_t	apply( pp::context & ctx ) override;	// TODO

	/*struct directive_state {
		core::string	m_directive_name;
		unsigned		m_flags_to_clear;
		unsigned		m_flags_to_set;
	};
	core::Tarray<directive_state>	m_directive_states;*/
};

/*END OF switch_directive.hxx*/
