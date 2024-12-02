/*Iattribute.hxx*/

class F1_PP_API f1::pp::Iattribute : public f1::pp::object
{
public:
	typedef f1::pp::object		super;
	typedef f1::pp::Iattribute	self;

	explicit Iattribute( const core::Iidentifier * ident = nullptr );

	const status_t	initialize( Iattribute_class & cls, unsigned flags = 0, pp::arg_list * args = nullptr );

	// Properties
public:
	virtual const f1::guid_t &	get_class_guid() const;
	virtual bool	supports_guid( const f1::guid_t & guid ) const noexcept;

	const pp::Iattribute_class *	get_attribute_class() const;
		  pp::Iattribute_class *	get_attribute_class();

	static const unsigned	fl_inverted		= 0x0001;
	const status_t	modify_flags( unsigned flags_to_set, unsigned flags_to_clear = 0, unsigned * flags_prev = nullptr ) noexcept;
	unsigned __nodiscard	get_flags( unsigned mask = unsigned(-1) ) const noexcept;

	const pp::arg_list *	get_arg_list() const noexcept;
protected:
	core::Tweak_ref<pp::Iattribute_class>	m_attribute_class;
	core::Tref<pp::arg_list>	m_arg_list;
	bitflags_t	m_flags;
};

/*END OF Iattribute.hxx*/
