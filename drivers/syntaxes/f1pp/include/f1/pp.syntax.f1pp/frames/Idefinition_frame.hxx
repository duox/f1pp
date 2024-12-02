/*Idefinition_frame.hxx*/

interface F1_SYNTAX_F1PP_API f1::f1pp::Idefinition_frame : public f1::c_pp::Iframe
{
public:
	typedef f1::c_pp::Iframe			super;
	typedef f1::f1pp::Idefinition_frame	self;

	explicit Idefinition_frame( f1pp::parser & parser );

	pp::Imacro::macro_flags_t m_macro_flags;
	static const unsigned fl_def_global		= 0x0001;
	static const unsigned fl_def_local		= 0x0002;
	static const unsigned fl_def_public		= 0x0004;
	unsigned	m_def_flags;
};

/*END OF Idefinition_frame.hxx*/
