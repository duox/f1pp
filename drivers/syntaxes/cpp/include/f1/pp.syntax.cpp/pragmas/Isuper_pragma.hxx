/*Isuper_pragma.hxx*/

interface F1_PP_API f1::c_pp::Isuper_pragma : public f1::c_pp::Ipragma
{
	typedef f1::c_pp::Ipragma		super;
	typedef f1::c_pp::Isuper_pragma	self;

	explicit Isuper_pragma( const core::Iidentifier * ident = nullptr ) __noexcept;

	// Sub-pragmas
public:
	const status_t	append_subpragma( const Ipragma & sub );
	const status_t	remove_subpragma( const Ipragma & sub );
	const status_t	remove_all_subpragmas();

	const status_t	find_subpragma( core::Tconst_ref<Ipragma> & sub, const pp::string & name ) const;

	const const_pragma_list_t &	get_subpragma_list() const;

	const status_t	append_known_subpragma( const char * name, size_t len = size_t(-1) );
	const status_t	remove_known_subpragma( const char * name, size_t len = size_t(-1) );
	const status_t	remove_all_known_subpragmas();

	const core::const_string_list_t &	get_known_subpragma_list() const;
protected:
	const_pragma_list_t			m_subpragma_list;
	core::const_string_list_t	m_known_subpragma_list;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser, const pp::string & param_str ) const override;
};

/*END OF pragma.hxx*/
