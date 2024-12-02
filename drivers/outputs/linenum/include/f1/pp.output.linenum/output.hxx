/*output.hxx*/

class F1_PP_OUTPUT_LINENUM_API f1::pp::output::linenum::output : public f1::pp::Ioutput
{
public:
	typedef f1::pp::Ioutput					super;
	typedef f1::pp::output::linenum::output	self;

	explicit output( const core::Iidentifier * ident = nullptr ) noexcept;

	virtual const status_t	begin_subsection( const char * ident, size_t len = size_t(-1) ) noexcept override;
	virtual const status_t	end_subsection() noexcept override;

	virtual const status_t	write( const char * string, size_t len = size_t(-1) ) override;

	virtual const status_t	on_file_changed( const char * filename, f1::parser::lineno_t lineno ) noexcept override;
	virtual const status_t	on_line_changed( f1::parser::lineno_t lineno ) noexcept override;

protected:
	f1::parser::lineno_t m_last_lineno;
	bool m_was_space;

	struct subsection_desc {
		size_t					m_prev_length;
		f1::parser::lineno_t	m_start_line;
	};
	core::Tarray<subsection_desc>	m_subsections;
	core::string					m_subsections_string;
	subsection_desc *				m_subsection_desc;

	mutable core::string	m_buffer;	// a buffer for miscellaneous formatting
};

/*END OF output.hxx*/
