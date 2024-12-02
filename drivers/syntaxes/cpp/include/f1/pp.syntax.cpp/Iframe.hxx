/*parser.hxx*/

interface F1_PP_API f1::c_pp::Iframe : public f1::pp::Iframe
{
	typedef f1::pp::Iframe		super;
	typedef Iframe				self;

	explicit Iframe( c_pp::parser & par );

	void	check_different_sources();

	static const unsigned	fl_warned_different_sources	= 0x0001;
	static const unsigned	fl_warned_end_tag_mistach	= 0x0002;

	virtual const status_t	enter() override;
	virtual const status_t	leave() override;

	const status_t	allow_exec( bool allow, bool force = false );
protected:
//	bool	m_was_allowed;
};
/*class F1_PP_API f1::c_pp::idle_frame : public f1::c_pp::Iframe
{
	typedef f1::c_pp::Iframe		super;
	typedef f1::c_pp::idle_frame	self;

	explicit idle_frame( Idirective & drc, c_pp::parser & par );

	virtual const char *	get_openning_directive_name() const override;
	virtual const char *	get_closing_directive_name() const override;
	virtual const f1::guid_t & get_openning_directive_guid() const override;
	virtual const f1::guid_t & get_closing_directive_guid() const override;
public:
	core::Tref<Idirective>	m_directive;
};*/

/*END OF parser.hxx*/
