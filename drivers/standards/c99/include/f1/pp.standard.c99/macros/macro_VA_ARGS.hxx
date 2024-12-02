/*macro_VA_ARGS.hxx*/

class F1_PP_API f1::c_pp::c99::macro_VA_ARGS : public f1::pp::Imacro
{
public:
	typedef f1::pp::Imacro			super;
	typedef f1::c_pp::c99::macro_VA_ARGS	self;

	virtual const status_t	initialize( const f1::pp::attribute_list * attrs = nullptr,
		const pp::param_list * params = nullptr, const pp::string * value = nullptr, macro_flags_t flags = null ) override;

	// Properties
public:
	static const char * m_name;
	virtual const char *	get_name() const override;
protected:

	// Operations
public:
	virtual const status_t	expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const override;
};

/*END OF macro_VA_ARGS.hxx*/
