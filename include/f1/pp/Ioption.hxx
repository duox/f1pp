/*Ioption.hxx*/
/** @file
 *
 *	Global options.
 */

interface F1_PP_API f1::pp::Ioption : public f1::pp::object
{
	typedef f1::pp::object		super;
	typedef f1::pp::Ioption		self;

	virtual const core::stringslice &	get_name() const abstract;
	virtual const f1::guid_t &	get_guid() const abstract;

	virtual const pp::variant::type_t	get_expected_type() const abstract;
	virtual const pp::variant &	get_value() const abstract;
	virtual const status_t		set_value( const pp::variant & val ) abstract;
	virtual const pp::variant &	get_default_value() const;
};

/*END OF Ioption.hxx*/
