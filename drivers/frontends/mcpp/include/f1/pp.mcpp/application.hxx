/*application.hxx*/

class f1::pp::frontends::mcpp::application : public f1::util::application
{
public:
	typedef f1::util::application					super;
	typedef f1::pp::frontends::mcpp::application	self;

	application();

	status_t preinitialize_application() override;
	status_t run_application() override;
	status_t uninitialize_application() override;

//	void	print( pcsz_t file, long line, message_id message_id, va_list ap );

	status_t	on_filename( pcsz_t filename ) override;

	status_t switch_help( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_nologo( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_output( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_quiet( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_ver( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_verbose( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_warn( char_t switchar, pcsz_t name, pcsz_t rest );

	status_t switch_comments( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_define( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_encoding( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_include( char_t switchar, pcsz_t name, pcsz_t rest );

	status_t switch_backup( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_case( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_date( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_extended( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_newer( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_pagesize( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_purge( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_short( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_strip( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_unmangle( char_t switchar, pcsz_t name, pcsz_t rest );

	status_t switch_add( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_delete( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_dump( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_export( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_extract( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_list( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_modify( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_out( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_outdir( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_remove( char_t switchar, pcsz_t name, pcsz_t rest );
	status_t switch_replace( char_t switchar, pcsz_t name, pcsz_t rest );

private:
	f1::core::Tref<f1::pp::context>	m_context;
	//lib_exe_message_sink m_message_sink;
};

/*END OF application.hxx*/
