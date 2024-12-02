/*Istandard.hxx*/

interface F1_PP_API f1::pp::Istandard : public f1::pp::object
{
	typedef f1::pp::object			super;
	typedef f1::pp::Istandard		self;

	explicit Istandard( const core::Iidentifier * ident = nullptr );

	// Properties
public:
	virtual const f1::guid_t &	get_guid() const abstract;
	virtual const char *	get_ident_string() const abstract;
	virtual const char *	get_description_string() const abstract;

	virtual const f1::guid_t *	get_base_guid_list( __out size_t & count ) const;

	// Hierarchy
public:
/*	const status_t	append_base_standard( pp::Istandard & standard );
	const status_t	remove_base_standard( pp::Istandard & standard );
	const status_t	remove_all_base_standards();
	const pp::standard_list_t &	get_base_standard_list() const;

	const status_t	on_append_derived_standard( pp::Istandard & standard );
	const status_t	on_remove_derived_standard( pp::Istandard & standard );
	const status_t	on_remove_all_derived_standards();
	const pp::standard_list_t &	get_derived_standard_list() const;
protected:
	pp::standard_list_t		m_base_standard_list;
	pp::standard_list_t		m_derived_standard_list;
*/
	// Operations
public:
	const status_t	attach_to_parser( pp::Iparser & parser ) const;		// must not be virtual to keep attach algo intact
	const status_t	detach_from_parser( pp::Iparser & parser ) const;	// must not be virtual to keep attach algo intact

	virtual const status_t	on_attach_to_parser( pp::Iparser & parser );
	virtual const status_t	on_detach_from_parser( pp::Iparser & parser );

	virtual bool	is_item_supported( const f1::guid_t & item_class_guid, const pp::string & name ) const;

	virtual const status_t	process_command_line_switch( int switchar, const core::static_string & name, const core::static_string & param );
	virtual const status_t	process_configuration_param( const core::static_string & name, const core::static_string & param );
};

/*END OF Istandard.hxx*/
