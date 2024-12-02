/*pragma_option.hxx*/

class F1_PP_API f1::f1pp::pragma_option : public f1::c_pp::Ipragma
{
public:
	typedef f1::c_pp::Ipragma		super;
	typedef f1::f1pp::pragma_option	self;

	explicit pragma_option( const core::Iidentifier * ident = nullptr );

	// Properties
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	// Operations
	virtual const status_t	parse( pp::Iparser & parser, const pp::string & param_str ) const override;
protected:
	const status_t	parse_options( f1pp::parser & f1par, const char *& s, const char * end_s, bool parenthesis ) const;
};

/*END OF pragma_option.hxx*/
