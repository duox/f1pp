/*channel.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::channel : public f1::core::named_object
{
public:
	typedef f1::core::named_object	super;
	typedef f1::f1pp::channel		self;

	explicit channel( const core::Iidentifier * ident = nullptr );
			~channel();

	const status_t	initialize( pp::Iparser & par,
		const pp::string & name, const pp::string & storage_name, const f1::guid_t * output_guid );

	// Properties
public:
	const pp::Iparser *		get_parser() const;
		  pp::Iparser *		get_parser();

	const pp::Istorage *	get_storage() const;
		  pp::Istorage *	get_storage();

	static const unsigned fl_no_newline	= 0x0001;
	const status_t	modify_flags( unsigned flags_to_set, unsigned flags_to_clear = 0, unsigned * flags_prev = nullptr );
	unsigned	get_flags( unsigned mask = unsigned(-1) ) const;

	const pp::string &	get_name() const;
	const pp::string &	get_storage_name() const;

	const status_t	set_definition_location( f1::parser::Isource & src, f1::parser::lineno_t line );

	const status_t	set_header( const pp::string & str );
	const status_t	set_header_move( pp::string & str );
	const pp::string &	get_header() const;

	const status_t	set_footer( const pp::string & str );
	const status_t	set_footer_move( pp::string & str );
	const pp::string &	get_footer() const;

	const status_t	set_prefix( const pp::string & str );
	const status_t	set_prefix_move( pp::string & str );
	const pp::string &	get_prefix() const;

	const status_t	set_suffix( const pp::string & str );
	const status_t	set_suffix_move( pp::string & str );
	const pp::string &	get_suffix() const;
protected:
	f1::pp::string			m_name;
	f1::pp::string			m_storage_name;
	core::Tweak_ref<pp::Iparser>	m_parser;
	core::Tref<pp::Istorage>		m_storage;
	core::Tweak_ref<pp::Istorage>	m_storage_previous;
	bitflags_t	m_flags;

	core::Tweak_ref<f1::parser::Isource>	m_definition_source;
	f1::parser::lineno_t					m_definition_line;

	pp::string	m_header;
	pp::string	m_footer;
	pp::string	m_prefix;
	pp::string	m_suffix;

	struct write_hook : public pp::Istorage::Ihook {
		write_hook() noexcept;
		virtual const status_t	on_before_write( const pp::string & str ) override;
		virtual const status_t	on_after_write( const pp::string & str ) override;
		virtual const status_t	on_close() override;
		core::Tweak_ref<channel>	m_channel;
		bool		m_first;
		unsigned	m_self;
	};
	write_hook	m_write_hook;

	// Operations
public:
	const status_t	activate();
	const status_t	deactivate();

	const status_t	write( const pp::string & data );
	const status_t	flush();

	const status_t	display_channel_location( pp::Iparser & par, msgout::msgid_t msgid = c_pp::syntax::IMSG_INFO_DEFINED_HERE );
protected:
};

/*END OF channel.hxx*/
