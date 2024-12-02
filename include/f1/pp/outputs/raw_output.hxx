/*raw_output.hxx*/

class F1_PP_API f1::pp::raw_output_class : public f1::pp::Ioutput_class
{
public:
	typedef f1::pp::raw_output_class	self;
	typedef f1::pp::Ioutput_class		super;

	explicit raw_output_class( const core::Iidentifier * ident = nullptr ) __noexcept;

	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const __noexcept override;

	virtual const status_t	new_output( core::Tref<pp::Ioutput> & res, const core::Iidentifier * ident = nullptr ) __noexcept override;
};

class F1_PP_API f1::pp::raw_output : public f1::pp::Ioutput
{
public:
	typedef f1::pp::raw_output		self;
	typedef f1::pp::Ioutput			super;

	explicit raw_output( const core::Iidentifier * ident = nullptr ) __noexcept;

	virtual const status_t	write( const char * string, size_t len = size_t(-1) ) __noexcept override;
	virtual const status_t	on_line_changed( f1::parser::lineno_t lineno ) noexcept override;
};

/*END OF raw_output.hxx*/
