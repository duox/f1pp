/*Istorage_class.hxx*/

interface F1_PP_API f1::pp::Istorage_class : public f1::pp::object
{
public:
	typedef f1::pp::Istorage_class	self;
	typedef f1::pp::object			super;

	explicit Istorage_class( const core::Iidentifier * ident = nullptr ) __noexcept;

	// Properties
public:
	virtual const f1::guid_t &	get_guid() const abstract;
	virtual const char *		get_name() const abstract;
protected:

	// Operations
public:
	virtual const status_t	new_storage( core::Tref<pp::Istorage> & res, const core::Iidentifier * ident = nullptr ) abstract;
};

/*END OF Istorage_class.hxx*/
