/*pragma_GCC_warning.hxx*/

class F1_PP_API f1::c_pp::gcc::pragma_GCC_warning : public f1::c_pp::gcc::pragma_GCC
{
public:
	typedef f1::c_pp::gcc::pragma_GCC			super;
	typedef f1::c_pp::gcc::pragma_GCC_warning	self;

	explicit pragma_GCC_warning( const core::Iidentifier * ident = nullptr );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;
protected:

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser, const pp::string & param_str ) const override;
};

/*END OF pragma_GCC_warning.hxx*/
