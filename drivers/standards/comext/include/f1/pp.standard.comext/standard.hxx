/*standard.hxx*/

class F1_PP_API f1::c_pp::comext::standard: public f1::pp::Istandard
{
public:
	typedef f1::pp::Istandard			super;
	typedef f1::c_pp::comext::standard	self;

	explicit standard( const core::Iidentifier * ident = nullptr );

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

	virtual bool	is_item_supported( const f1::guid_t & item_class_guid, const pp::string & name ) const override;
protected:
	file_directive			m_file_directive;
	ident_directive			m_ident_directive;
	include_next_directive	m_include_next_directive;
	includepath_directive	m_includepath_directive;
	info_directive			m_info_directive;
	sccs_directive			m_sccs_directive;
	warning_directive		m_warning_directive;

	macro_COUNTER			m_macro_COUNTER;
	macro_TIMESTAMP			m_macro_TIMESTAMP;

	pragma_message			m_pragma_message;
	pragma_once				m_pragma_once;
	pragma_include_alias	m_pragma_include_alias;
	pragma_pop_macro		m_pragma_pop_macro;
	pragma_push_macro		m_pragma_push_macro;
};

/*END OF standard.hxx*/
