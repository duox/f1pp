/*output.hxx*/

class F1_PP_OUTPUT_PURETEXT_API f1::pp::output::puretext::output : public f1::pp::Ioutput
{
public:
	typedef f1::pp::Ioutput						super;
	typedef f1::pp::output::puretext::output	self;

	explicit output( const core::Iidentifier * ident = nullptr ) noexcept;

	virtual const status_t	write( const char * string, size_t len = size_t(-1) ) override;

//	virtual const status_t	on_file_changed( const char * filename, f1::parser::lineno_t lineno ) noexcept override;
	virtual const status_t	on_line_changed( f1::parser::lineno_t lineno ) noexcept override;

protected:
	f1::parser::lineno_t m_last_lineno;
	bool m_was_nl;
	bool m_was_space;
};

/*END OF output.hxx*/
