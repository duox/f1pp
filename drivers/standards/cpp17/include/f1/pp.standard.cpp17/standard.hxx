/*standard.hxx*/

class F1_PP_API f1::c_pp::cpp17::standard: public f1::pp::Istandard
{
public:
	typedef f1::pp::Istandard			super;
	typedef f1::c_pp::cpp17::standard	self;

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
	c_pp::cpp17::macro_has_include		m_macro_has_include;
};

/*END OF standard.hxx*/
