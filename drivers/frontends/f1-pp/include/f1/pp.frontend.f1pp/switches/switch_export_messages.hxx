/*switch_export_messages.hxx*/

class F1_PP_FRONTEND_F1PP_API f1::pp::frontend::f1pp::switch_export_messages : public f1::util::cmdline::Iswitch
{
public:
	typedef f1::util::cmdline::Iswitch				super;
	typedef f1::pp::frontend::f1pp::switch_export_messages	self;

	explicit switch_export_messages( unsigned flags = 0 ) NOEXCEPT;

	virtual const char *	get_name() const NOEXCEPT override;
	virtual const char *	get_long_name() const NOEXCEPT override;
	virtual const char *	get_desc() const NOEXCEPT override;
	//virtual const char *	get_params() const NOEXCEPT override;
	virtual const char *	get_help_text() const NOEXCEPT override;

	virtual status_t	process( char switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) NOEXCEPT override;

	//const status_t	apply( pp::context & ctx );

protected:
	struct export_messages_command : public pp::Iparser::Icommand {
		explicit export_messages_command( const char * name, const char * name_end );
		virtual const status_t	execute( pp::Iparser & parser ) override;

		core::string	m_export_file_name;
	};

public:
	//bool	m_value;
};

/*END OF switch_export_messages.hxx*/
