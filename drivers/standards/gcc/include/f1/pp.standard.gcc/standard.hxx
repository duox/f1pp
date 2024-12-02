/*standard.hxx*/

class F1_PP_API f1::c_pp::gcc::standard: public f1::pp::Istandard
{
public:
	typedef f1::pp::Istandard			super;
	typedef f1::c_pp::gcc::standard	self;

	const status_t	initialize() override;

	// Properties
public:
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;
	virtual const char *	get_ident_string() const override;
	virtual const char *	get_description_string() const override;

	virtual const f1::guid_t *	get_base_guid_list( __out size_t & count ) const override;

	// Operations
public:
	virtual const status_t	on_attach_to_parser( pp::Iparser & parser ) override;
	virtual const status_t	on_detach_from_parser( pp::Iparser & parser ) override;

protected:
	macro_BASE_FILE		m_macro_BASE_FILE;
	macro_FILE_NAME		m_macro_FILE_NAME;
	macro_INCLUDE_LEVEL	m_macro_INCLUDE_LEVEL;

	pragma_GCC			m_pragma_GCC;
};

/*END OF standard.hxx*/
