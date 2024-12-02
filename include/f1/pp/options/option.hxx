/*option.hxx*/
/** @file
 *
 *	Options that can be defined by the user.
 */

class F1_PP_API f1::pp::option : public f1::pp::Ioption
{
	using super::initialize;
public:
	typedef f1::pp::Ioption		super;
	typedef f1::pp::option		self;

	explicit option() __noexcept;

	const status_t	initialize( pp::variant::type_t ty,
		const f1::guid_t & guid,
		const char * name, size_t len = size_t(-1) );

	// Properties
public:
	virtual const status_t		set_name( const char * name, size_t len = size_t(-1) );
	virtual const status_t		set_name( const pp::string & name );
	virtual const status_t		set_name( const core::string & name );
	virtual const status_t		set_name_move( pp::string & name );
	virtual const core::stringslice &	get_name() const override;

	const status_t	set_guid( const f1::guid_t & guid );
	virtual const f1::guid_t &	get_guid() const override;

	virtual const status_t	set_expected_type( f1::pp::variant::type_t ty );
	virtual const pp::variant::type_t	get_expected_type() const override;

	virtual const status_t		set_value( const pp::variant & val ) override;
	virtual const pp::variant &	get_value() const override;
protected:
	pp::string		m_name;
	f1::guid_t		m_guid;
	pp::variant		m_value;
	pp::variant::type_t	m_expected_type;
};

/*END OF option.hxx*/
