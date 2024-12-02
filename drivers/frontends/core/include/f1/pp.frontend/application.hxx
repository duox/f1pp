/*application.hxx*/

class F1_PP_FRONTEND_API f1::pp::frontend::application : public f1::util::application
{
public:
	typedef f1::pp::frontend::application	self;
	typedef f1::util::application			super;

	explicit application( const char * name );

	virtual status_t preinitialize_application() __noexcept override;
	virtual status_t uninitialize_application() __noexcept override;
protected:
//	f1::pp::context	m_pp_context;
	f1::pp::frontend::context	m_frontend_context;

	// Command line
public:
	virtual status_t	on_filename( const f1::core::const_string & filename ) noexcept override;

private:
	// Execution
public:
	virtual const status_t	new_context( core::Tref<context> & res, const core::Iidentifier * ident = nullptr );
protected:
};

/*END OF application.hxx*/
