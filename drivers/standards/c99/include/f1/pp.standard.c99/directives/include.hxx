/*include.hxx*/

class F1_PP_API f1::c_pp::c99::include_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective		super;
	typedef f1::c_pp::c99::include_directive	self;

	explicit include_directive( unsigned flags = 0 ) __noexcept;

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
protected:
	static const status_t	line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );
	virtual const status_t	execute( f1::pp::Iparser & parser, const core::buffer & out_buf );
	virtual const status_t	execute( f1::c_pp::parser & c_pp_par, const char * & s, const char * end_s );

	// Hooks
protected:
	virtual const status_t	parse_include_filename( f1::c_pp::parser & par, pp::string & buf, c_pp::parser::include_flags_t & include_flags, const char *& s, const char * end_s );
	//virtual const status_t	include_file( f1::c_pp::parser & par, c_pp::parser::include_desc & inc_desc );
	virtual const status_t	include_file( f1::c_pp::parser & par, const pp::string & file_path_name, c_pp::parser::include_flags_t include_flags, c_pp::parser::include_desc ** inc_desc_ptr = nullptr );
	virtual const status_t	on_include_not_found( f1::c_pp::parser & par, const pp::string & file_path_name, c_pp::parser::include_flags_t include_flags );
	//virtual const status_t	update_once_include( f1::c_pp::parser & par );
	virtual const status_t	on_complete( f1::c_pp::parser & par, const char *& s, const char * end_s );
	virtual const status_t	check_for_newline( f1::c_pp::parser & par, const char *& s, const char * end_s );
	virtual const status_t	on_unfinished_line( f1::c_pp::parser & par, const char *& s, const char * end_s );
};

/*END OF include.hxx*/
