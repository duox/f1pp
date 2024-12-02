/*pragma_includedir.hxx*/

class F1_PP_API f1::f1pp::pragma_includedir : public f1::c_pp::Ipragma
{
public:
	typedef f1::c_pp::Ipragma			super;
	typedef f1::f1pp::pragma_includedir	self;

	explicit pragma_includedir( const core::Iidentifier * ident = nullptr );

	// Properties
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	// Operations
	virtual const status_t	parse( pp::Iparser & parser, const pp::string & param_str ) const override;
};

/*END OF pragma_includedir.hxx*/
