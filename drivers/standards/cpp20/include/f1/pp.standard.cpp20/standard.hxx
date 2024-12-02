/*standard.hxx*/

class F1_PP_API f1::c_pp::cpp20::standard: public f1::pp::Istandard
{
public:
	typedef f1::pp::Istandard			super;
	typedef f1::c_pp::cpp20::standard	self;

	explicit standard( const core::Iidentifier * ident = nullptr );

	const status_t	initialize() override;

	// Properties
public:
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;
	virtual const char *	get_ident_string() const override;
	virtual const char *	get_description_string() const override;

	virtual const f1::guid_t *	get_base_guid_list( __out size_t & count ) const override;

	// Predefined macros
	macro_VA_OPT	m_macro_VA_OPT;
	// Operations
public:
	virtual const status_t	on_attach_to_parser( pp::Iparser & parser ) override;
	virtual const status_t	on_detach_from_parser( pp::Iparser & parser ) override;

	virtual bool	is_item_supported( const f1::guid_t & item_class_guid, const pp::string & name ) const override;
protected:
};

/*END OF standard.hxx*/
