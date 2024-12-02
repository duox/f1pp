/*attribute.hxx*/

class F1_PP_API f1::pp::attribute : public f1::pp::object
{
	using super::initialize;
public:
	typedef f1::pp::object		super;
	typedef f1::pp::attribute	self;

	explicit attribute( const core::Iidentifier * ident = nullptr );

	const status_t	initialize( const pp::string & name, unsigned flags = 0, pp::arg_list * args = nullptr );

	// Properties
public:
	const pp::string &	get_name() const;

	static const unsigned	fl_inverted		= 0x0001;
	const status_t	modify_flags( unsigned flags_to_set, unsigned flags_to_clear = 0, unsigned * flags_prev = nullptr ) noexcept;
	unsigned __nodiscard	get_flags( unsigned mask = unsigned(-1) ) const noexcept;

	const pp::arg_list *	get_arg_list() const noexcept;
protected:
	pp::string	m_name;
	core::Tref<pp::arg_list>	m_arg_list;
	bitflags_t	m_flags;

	// Operations
public:
	const status_t	output( pp::Iparser & parser ) const;
};

/*END OF attribute.hxx*/
