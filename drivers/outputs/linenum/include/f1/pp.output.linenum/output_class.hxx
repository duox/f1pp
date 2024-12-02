/*output_class.hxx*/

class F1_PP_OUTPUT_LINENUM_API f1::pp::output::linenum::output_class : public f1::pp::Ioutput_class
{
public:
	typedef f1::pp::Ioutput_class					super;
	typedef f1::pp::output::linenum::output_class	self;

	explicit output_class( const core::Iidentifier * ident = nullptr ) noexcept;

	static const f1::guid_t	m_guid;
	virtual const f1::guid_t &	get_guid() const override;
	virtual const char *		get_name() const override;

	virtual const status_t	new_output( core::Tref<pp::Ioutput> & res, const core::Iidentifier * ident ) override;
};

/*END OF output_class.hxx*/
