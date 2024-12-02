/*Iswitch_numeric_arg.hxx*/

interface F1_PP_FRONTEND_F1PP_API f1::pp::frontend::f1pp::Iswitch_numeric_arg : public f1::util::cmdline::Iswitch
{
public:
	typedef f1::util::cmdline::Iswitch					super;
	typedef f1::pp::frontend::f1pp::Iswitch_numeric_arg	self;

	explicit Iswitch_numeric_arg( bitflags_t flags = bitflags_t::null ) NOEXCEPT;

	virtual status_t	process_numeric_arg( intmax_t & value, char switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) NOEXCEPT;
};


/*END OF Iswitch_numeric_arg.hxx*/
