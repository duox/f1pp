/*channel.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::channel_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective				super;
	typedef f1::f1pp::channel_directive		self;

	explicit channel_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;
protected:
	unsigned	m_additional_caps;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;

	static const unsigned cmd_f_accepts_id		= 0x01;	///< channel sub-command can use channel name
	static const unsigned cmd_f_requires_id		= 0x03;	///< channel sub-command requires channel name
	static const unsigned cmd_f_chan_name_only	= 0x04;	///< channel sub-command requires channel name only, not the object itself
	struct channel_subcommand {
		const status_t (f1::f1pp::channel_directive::*parse)( f1pp::parser & par, channel * chan, const pp::string & channel_name );
		const char *	m_name;
		uint8_t			m_length;
		uint8_t			m_flags;
	};
	static channel_subcommand	m_channel_subcommands[];

	const status_t	cm_channel_activate( f1pp::parser & f1par, channel * chan, const pp::string & channel_name );
	const status_t	cm_channel_close( f1pp::parser & f1par, channel * chan, const pp::string & channel_name );
	const status_t	cm_channel_define( f1pp::parser & f1par, channel * chan, const pp::string & channel_name );
	const status_t	cm_channel_destroy( f1pp::parser & f1par, channel * chan, const pp::string & channel_name );
	const status_t	cm_channel_flush( f1pp::parser & f1par, channel * chan, const pp::string & channel_name );
	const status_t	cm_channel_footer( f1pp::parser & f1par, channel * chan, const pp::string & channel_name );
	const status_t	cm_channel_header( f1pp::parser & f1par, channel * chan, const pp::string & channel_name );
	const status_t	cm_channel_include( f1pp::parser & f1par, channel * chan, const pp::string & channel_name );
	const status_t	cm_channel_pop( f1pp::parser & f1par, channel * chan, const pp::string & channel_name );
	const status_t	cm_channel_prefix( f1pp::parser & f1par, channel * chan, const pp::string & channel_name );
	const status_t	cm_channel_push( f1pp::parser & f1par, channel * chan, const pp::string & channel_name );
	const status_t	cm_channel_suffix( f1pp::parser & f1par, channel * chan, const pp::string & channel_name );
	const status_t	cm_channel_write( f1pp::parser & f1par, channel * chan, const pp::string & channel_name );
};

/*END OF channel.hxx*/
