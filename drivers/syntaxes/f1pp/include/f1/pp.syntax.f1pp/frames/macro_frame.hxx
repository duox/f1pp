/*macro_frame.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::macro_frame : public f1::pp::Iframe
{
public:
	typedef f1::pp::Iframe				super;
	typedef f1::f1pp::macro_frame		self;

	explicit macro_frame( f1pp::parser & parser );

	virtual const char *	get_openning_directive_name() const override { return nullcstr; }
	virtual const char *	get_closing_directive_name() const override { return nullcstr; }
	virtual const f1::guid_t & get_openning_directive_guid() const override { return f1::guid_t::null; }
	virtual const f1::guid_t & get_closing_directive_guid() const override { return f1::guid_t::null; }
	virtual const status_t	enter() override;
	virtual const status_t	leave() override;

	f1::pp::variant		m_returned_value;
};

/*END OF macro_frame.hxx*/
