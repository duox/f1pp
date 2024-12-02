/*label.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::label : public f1::ns::named_object
{
public:
	typedef f1::ns::named_object	super;
	typedef f1::f1pp::label			self;

	explicit label( const ns::identifier * ident = nullptr );

	const status_t	initialize( pp::Iparser & par, const pp::string	& name );

	// Properties
public:
	const pp::string &	get_name() const;
	const f1::parser::Iparser::position &	get_position() const;
protected:
	f1::pp::string		m_name;
	f1::parser::Iparser::position	m_position;
	core::Tarray<pp::Iframe *>		m_frame_stack;
	core::Tconst_weak_ref<goto_directive>	m_goto_directive;

	// Operations
public:
	const status_t	jump( pp::Iparser & par ) const;
};

/*END OF label.hxx*/
