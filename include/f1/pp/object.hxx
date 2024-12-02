/*object.hxx*/

class F1_PP_API f1::pp::object : public f1::core::named_object
{
public:
	typedef f1::core::named_object	super;
	typedef f1::pp::object			self;

	explicit object( const core::Iidentifier * ident = nullptr );

	// Properties
public:
	const f1::core::Iidentifier &	get_identifier() const;
private:
	core::Tref<core::Iidentifier>	m_ident;
};

/*END OF object.hxx*/
