/*pragma_clang_deprecated.hxx*/

class F1_PP_API f1::c_pp::clang::pragma_clang_deprecated : public f1::c_pp::Ipragma
{
public:
	typedef f1::c_pp::Ipragma							super;
	typedef f1::c_pp::clang::pragma_clang_deprecated	self;

	explicit pragma_clang_deprecated( const core::Iidentifier * ident = nullptr );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;
	virtual unsigned get_caps( unsigned mask = unsigned(-1) ) const override;
protected:

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser, const pp::string & param_str ) const override;
};

/*END OF pragma_clang_deprecated.hxx*/
