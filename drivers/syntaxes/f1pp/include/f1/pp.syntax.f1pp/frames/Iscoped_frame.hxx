/*Iscoped_frame.hxx*/

interface F1_SYNTAX_F1PP_API f1::f1pp::Iscoped_frame : public f1::c_pp::Iframe
{
	typedef f1::c_pp::Iframe	super;
	typedef f1::f1pp::Iscoped_frame		self;

	explicit Iscoped_frame( f1pp::parser & par );

	virtual const status_t	enter() override;
	virtual const status_t	leave() override;

	const status_t	undefine_locals();
public:

	static const unsigned fl_def_global		= 0x0001;
	static const unsigned fl_def_local		= 0x0002;
	static const unsigned fl_def_public		= 0x0004;
	unsigned					m_def_flags;
	pp::Imacro::macro_flags_t	m_macro_flags;

	f1::ns::name_space			m_own_ns;
	f1::ns::name_space			m_locals_ns;
protected:
	core::Tweak_ref<f1::ns::name_space>	m_parent_ns;
};

/*END OF Iscoped_frame.hxx*/
