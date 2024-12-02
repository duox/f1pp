/*attribute_list.hxx*/

class F1_PP_API f1::pp::attribute_list : public f1::core::named_object
{
public:
	typedef f1::core::named_object		super;
	typedef f1::pp::attribute_list	self;

	explicit attribute_list( const core::Iidentifier * ident = nullptr );

	// Attributes
public:
	const status_t	append_attribute( const pp::attribute & attr );
	const status_t	append_attributes( const pp::attribute_list & attrs );
	const status_t	remove_attribute( const pp::attribute & attr );
	const status_t	remove_attributes( const pp::attribute_list & attrs );
	const status_t	remove_all_attributes();

	bool	exists_attribute( const f1::guid_t & guid ) const;
	bool	exists_attribute( const char * name, size_t len = size_t(-1) ) const;
	bool	exists_attribute( const pp::string & name ) const;

//	const status_t	find_attribute( core::Tconst_ref<pp::attribute> & res,
//		const f1::guid_t &  guid, const pp::arg_list * args = nullptr ) const;
	const status_t	find_attribute( core::Tconst_ref<pp::attribute> & res,
		const pp::string & ident, const pp::arg_list * args = nullptr ) const;

	const const_attribute_list_t &	get_attribute_list() const;
protected:
	const_attribute_list_t	m_attribute_list;
//	bool	_check_attribute( const pp::attribute & attr, const Iattribute_class & attr_class, const pp::arg_list * args ) const;

	// Operations
public:
	const status_t	output( pp::Iparser & parser ) const;
};

/*END OF attribute_list.hxx*/
