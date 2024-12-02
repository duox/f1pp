/*tryinclude.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::tryinclude_directive : public f1::c_pp::c99::include_directive
{
public:
	typedef f1::c_pp::c99::include_directive	super;
	typedef f1::f1pp::tryinclude_directive		self;

	explicit tryinclude_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:

	// Hooks
protected:
	virtual const status_t	on_include_not_found( f1::c_pp::parser & par, const pp::string & file_path_name, c_pp::parser::include_flags_t include_flags ) override;
};

/*END OF tryinclude.hxx*/
