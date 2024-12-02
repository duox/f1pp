/*pragma_clang.hxx*/

class F1_PP_API f1::c_pp::clang::pragma_clang : public f1::c_pp::Isuper_pragma
{
public:
	typedef f1::c_pp::Isuper_pragma			super;
	typedef f1::c_pp::clang::pragma_clang	self;

	explicit pragma_clang( const core::Iidentifier * ident = nullptr );

	virtual const status_t	initialize() override;

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;
protected:

	// Operations
public:
	//virtual const status_t	parse( pp::Iparser & parser, const pp::string & param_str ) const override;
protected:
};

/*END OF pragma_clang.hxx*/
