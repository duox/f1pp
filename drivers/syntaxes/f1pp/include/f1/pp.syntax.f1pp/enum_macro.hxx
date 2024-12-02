/*enum_macro.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::enum_macro : public f1::f1pp::macro
{
public:
	typedef f1::f1pp::macro			super;
	typedef f1::f1pp::enum_macro	self;

	explicit enum_macro( const core::Iidentifier * ident = nullptr );

	virtual const status_t	initialize( f1::f1pp::enumeration & en,
		const pp::string & name, const f1::pp::attribute_list * attrs = nullptr,
		const pp::param_list * params = nullptr, const pp::string * value = nullptr, macro_flags_t flags = null );

	// Properties
public:
	const f1::f1pp::parser *	get_parser() const;
		  f1::f1pp::parser *	get_parser();

	const f1::f1pp::enumeration *	get_enumeration() const;
		  f1::f1pp::enumeration *	get_enumeration();
protected:
	core::Tweak_ref<f1::f1pp::enumeration>	m_enumeration;

	// Operations
public:
	virtual void	on_all_names_removed();
};

/*END OF enum_macro.hxx*/
