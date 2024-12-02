/*macro_COUNTER.hxx*/

class F1_PP_API f1::c_pp::comext::macro_COUNTER : public f1::pp::Imacro
{
public:
	typedef f1::pp::Imacro					super;
	typedef f1::c_pp::comext::macro_COUNTER	self;

	macro_COUNTER();

	virtual const status_t	initialize( const f1::pp::attribute_list * attrs = nullptr,
		const pp::param_list * params = nullptr, const pp::string * value = nullptr, pp::Imacro::macro_flags_t flags = null ) override;

	// Properties
public:
	virtual const char *	get_name() const override;
protected:
	mutable unsigned	m_counter;	///< counter; special case, increments each time the macro is substituted

	// Operations
public:
	virtual const status_t	expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const override;
};

/*END OF macro_COUNTER.hxx*/
