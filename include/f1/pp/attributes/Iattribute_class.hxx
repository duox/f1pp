/*Iattribute_class.hxx*/

interface F1_PP_API f1::pp::Iattribute_class : public f1::pp::object
{
public:
	typedef f1::pp::object				super;
	typedef f1::pp::Iattribute_class	self;

	explicit Iattribute_class( const core::Iidentifier * ident = nullptr );

	virtual const f1::guid_t &		__nodiscard	get_guid() const abstract;
	virtual const pp::string &		__nodiscard	get_name() const abstract;

	static const unsigned	cap_can_invert	= 0x0001;
	virtual unsigned				__nodiscard	get_caps( unsigned mask = unsigned(-1) ) const noexcept;
	virtual const pp::param_list *	__nodiscard	get_param_list() const noexcept;

	virtual const status_t	new_attribute( core::Tref<pp::Iattribute> & res, const core::Iidentifier * ident = nullptr ) abstract;
};

/*END OF Iattribute_class.hxx*/
