/*switch_max_include_depth.hxx*/

class F1_PP_FRONTEND_F1PP_API f1::pp::frontend::f1pp::switch_max_include_depth : public f1::pp::frontend::f1pp::Iswitch_numeric_arg
{
public:
	typedef f1::pp::frontend::f1pp::Iswitch_numeric_arg			super;
	typedef f1::pp::frontend::f1pp::switch_max_include_depth	self;

	explicit switch_max_include_depth( unsigned flags = 0 ) NOEXCEPT;

	virtual const char *	get_name() const NOEXCEPT override;
	virtual const char *	get_long_name() const NOEXCEPT override;
	virtual const char *	get_desc() const NOEXCEPT override;
	//virtual const char *	get_params() const NOEXCEPT override;
	virtual const char *	get_help_text() const NOEXCEPT override;

	virtual status_t	process( char switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) NOEXCEPT override;
};

/*END OF switch_max_include_depth.hxx*/
