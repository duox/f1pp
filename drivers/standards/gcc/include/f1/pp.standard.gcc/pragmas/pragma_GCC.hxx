/*pragma_GCC.hxx*/

class F1_PP_API f1::c_pp::gcc::pragma_GCC : public f1::c_pp::Isuper_pragma
{
public:
	typedef f1::c_pp::Isuper_pragma		super;
	typedef f1::c_pp::gcc::pragma_GCC	self;

	explicit pragma_GCC( const core::Iidentifier * ident = nullptr );

	virtual const status_t	initialize() override;

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;
protected:
};

/*END OF pragma_GCC.hxx*/
