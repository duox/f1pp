/*Ipragma.hxx*/

interface F1_PP_API f1::c_pp::Ipragma : public f1::pp::object
{
	typedef f1::pp::object		super;
	typedef f1::c_pp::Ipragma	self;

	explicit Ipragma( const core::Iidentifier * ident = nullptr ) __noexcept;

	static const f1::guid_t m_item_class_guid;

	// Properties
public:
	virtual const char *	get_name() const abstract;
	virtual const f1::guid_t &	get_guid() const abstract;

	static const unsigned fl_self_parse_arglist	= 0x0001;
	virtual unsigned get_caps( unsigned mask = unsigned(-1) ) const;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser, const pp::string & param_str ) const abstract;
};

/*END OF pragma.hxx*/
