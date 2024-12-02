/*user_attribute_class.hxx*/

class F1_PP_API f1::pp::user_attribute_class : public f1::pp::Iattribute_class
{
public:
	typedef f1::pp::Iattribute_class		super;
	typedef f1::pp::user_attribute_class	self;

	explicit user_attribute_class( const core::Iidentifier * ident = nullptr );
	const status_t	initialize( const f1::guid_t & m_guid, const pp::string & name );

	virtual const f1::guid_t &		__nodiscard	get_guid() const override;
	virtual const pp::string &		__nodiscard	get_name() const override;

	virtual const status_t	new_attribute( core::Tref<pp::Iattribute> & res, const core::Iidentifier * ident = nullptr ) override;
protected:
	f1::guid_t	m_guid;
	pp::string	m_name;
};

/*END OF user_attribute_class.hxx*/
