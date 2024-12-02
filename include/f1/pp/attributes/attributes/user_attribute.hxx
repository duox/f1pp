/*user_attribute.hxx*/

class F1_PP_API f1::pp::user_attribute : public f1::pp::Iattribute
{
public:
	typedef f1::pp::Iattribute		super;
	typedef f1::pp::user_attribute	self;

	explicit user_attribute( const core::Iidentifier * ident = nullptr );
};

/*END OF user_attribute.hxx*/
