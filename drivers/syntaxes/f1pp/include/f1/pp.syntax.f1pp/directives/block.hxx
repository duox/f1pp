/*block.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::block_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective		super;
	typedef f1::f1pp::block_directive	self;

	explicit block_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;

	struct frame : public f1::f1pp::Iscoped_frame {
		typedef f1::f1pp::Iscoped_frame		super;
		typedef frame						self;

		explicit frame( f1pp::parser & par );

		virtual const char *	get_openning_directive_name() const override;
		virtual const char *	get_closing_directive_name() const override;
		virtual const f1::guid_t & get_openning_directive_guid() const override;
		virtual const f1::guid_t & get_closing_directive_guid() const override;
	};
};

/*END OF block.hxx*/
