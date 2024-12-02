/*shell.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::shell_directive : public f1::f1pp::exec_directive
{
public:
	typedef f1::f1pp::exec_directive	super;
	typedef f1::f1pp::shell_directive	self;

	explicit shell_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

protected:

	// Operations
public:
	virtual const status_t	execute_commands( f1::f1pp::parser & f1par, const pp::string & cmd ) override;
};

/*END OF shell.hxx*/
