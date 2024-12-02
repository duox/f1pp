/*macro_has_embed.hxx*/

class F1_PP_API f1::c_pp::c23::macro_has_embed : public f1::pp::Imacro
{
public:
	typedef f1::pp::Imacro					super;
	typedef f1::c_pp::c23::macro_has_embed	self;

	virtual const status_t	initialize( const f1::pp::attribute_list * attrs = nullptr,
		const pp::param_list * params = nullptr, const pp::string * value = nullptr, macro_flags_t flags = null ) override;

	// Properties
public:
	virtual const char *	get_name() const override;
protected:

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser, pp::string & result ) const override;
	virtual const status_t	expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const override;
	virtual const status_t	execute( f1::c_pp::parser & parser, const embed_directive::resource_state & state, pp::string & result ) const;
};

/*END OF macro_has_embed.hxx*/
