/*standard.hxx*/

class F1_PP_API f1::c_pp::c99::standard : public f1::pp::Istandard
{
public:
	typedef f1::pp::Istandard			super;
	typedef f1::c_pp::c99::standard		self;

	explicit standard( const core::Iidentifier * ident = nullptr );

	const status_t	initialize() override;

	// Properties
public:
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;
	virtual const char *	get_ident_string() const override;
	virtual const char *	get_description_string() const override;

	const f1::guid_t *	get_base_guid_list( __out size_t & count ) const override;

	// Directives
public:
protected:
	define_directive		m_define_directive;
	elif_directive			m_elif_directive;
	else_directive			m_else_directive;
	endif_directive			m_endif_directive;
	error_directive			m_error_directive;
	if_directive			m_if_directive;
	ifdef_directive			m_ifdef_directive;
	ifndef_directive		m_ifndef_directive;
	include_directive		m_include_directive;
	line_directive			m_line_directive;
	pragma_directive		m_pragma_directive;
	undef_directive			m_undef_directive;

	// Predefined macros
public:
protected:
	macro_DATE		m_macro_DATE;
	macro_defined	m_macro_defined;
	macro_FILE		m_macro_FILE;
	macro_LINE		m_macro_LINE;
	macro_Pragma	m_macro_Pragma;
	macro_TIME		m_macro_TIME;
	macro_VA_ARGS	m_macro_VA_ARGS;

	// Operations
public:
	virtual const status_t	on_attach_to_parser( pp::Iparser & parser ) override;
	virtual const status_t	on_detach_from_parser( pp::Iparser & parser ) override;

	virtual bool	is_item_supported( const f1::guid_t & item_class_guid, const pp::string & name ) const override;
protected:
};

/*END OF standard.hxx*/
