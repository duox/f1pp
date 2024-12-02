/*standard.hxx*/

class F1_PP_API f1::c_pp::c23::standard: public f1::pp::Istandard
{
public:
	typedef f1::pp::Istandard			super;
	typedef f1::c_pp::c23::standard		self;

	explicit standard( const core::Iidentifier * ident = nullptr );

	const status_t	initialize() override;

	// Properties
public:
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;
	virtual const char *	get_ident_string() const override;
	virtual const char *	get_description_string() const override;

	virtual const f1::guid_t *	get_base_guid_list( __out size_t & count ) const override;

	// Directives
public:
protected:
	embed_directive				m_embed_directive;
	comext::warning_directive	m_warning_directive;
	cpp23::elifdef_directive	m_elifdef_directive;
	cpp23::elifndef_directive	m_elifndef_directive;

	// Macros
public:
protected:
	macro_has_embed				m_macro_has_embed;
	macro_STDC_EMBED_EMPTY		m_macro_STDC_EMBED_EMPTY;
	macro_STDC_EMBED_FOUND		m_macro_STDC_EMBED_FOUND;
	macro_STDC_EMBED_NOT_FOUND	m_macro_STDC_EMBED_NOT_FOUND;

	// Operations
public:
	virtual const status_t	on_attach_to_parser( pp::Iparser & parser ) override;
	virtual const status_t	on_detach_from_parser( pp::Iparser & parser ) override;

protected:
};

/*END OF standard.hxx*/
