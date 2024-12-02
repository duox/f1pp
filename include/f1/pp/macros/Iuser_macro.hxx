/*Iuser_macro.hxx*/
/** @file
 *
 */

class F1_PP_API f1::pp::Iuser_macro : public f1::pp::Imacro
{
public:
	typedef f1::pp::Imacro		super;
	typedef f1::pp::Iuser_macro	self;

	explicit Iuser_macro( const core::Iidentifier * ident = nullptr ) NOEXCEPT;

	virtual const status_t	initialize( const pp::string & name, const f1::pp::attribute_list * attrs = nullptr,
		const pp::param_list * params = nullptr, const pp::string * value = nullptr, macro_flags_t flags = null );

private:
	using super::initialize;

	// Properties
public:
	virtual const char *	get_name() const override;
protected:
	core::string	m_name;

};

/*END OF Iuser_macro.hxx*/
