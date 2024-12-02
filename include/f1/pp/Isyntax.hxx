/*Isyntax.hxx*/

interface F1_PP_API f1::pp::Isyntax : public f1::pp::Istandard
{
	typedef f1::pp::Istandard	super;
	typedef f1::pp::Isyntax		self;

	explicit Isyntax( const core::Iidentifier * ident = nullptr );

	virtual const f1::guid_t *	get_preferred_standard_list( __out size_t & count ) const;

	virtual const status_t	new_parser( f1::core::Tref<Iparser> & res, const core::Iidentifier * ident = nullptr ) abstract;
	virtual const status_t	new_macro( f1::core::Tref<Iuser_macro> & res, const core::Iidentifier * ident = nullptr ) abstract;

	virtual const status_t	attach_to_context( pp::context & ctx );
	virtual const status_t	detach_from_context( pp::context & ctx );
};

/*END OF Isyntax.hxx*/
