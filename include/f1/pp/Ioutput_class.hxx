/*Ioutput_class.hxx*/

interface F1_PP_API f1::pp::Ioutput_class : public f1::pp::object
{
public:
	typedef f1::pp::Ioutput_class	self;
	typedef f1::pp::object			super;

	explicit Ioutput_class( const core::Iidentifier * ident = nullptr ) __noexcept;

	// Properties
public:
	virtual const f1::guid_t &	get_guid() const abstract;
	virtual const char *		get_name() const abstract;
protected:

	// Operations
public:
	virtual const status_t	new_output( core::Tref<pp::Ioutput> & res, const core::Iidentifier * ident = nullptr ) abstract;
};

/*END OF Ioutput_class.hxx*/
