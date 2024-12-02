/*Ioutput.hxx*/

interface F1_PP_API f1::pp::Ioutput : public f1::pp::object
{
public:
	typedef f1::pp::Ioutput		self;
	typedef f1::pp::object		super;

	explicit Ioutput( const core::Iidentifier * ident = nullptr ) noexcept;
			~Ioutput() noexcept;

	begin_scoped_enum( output_flags_t )
		fl_compress_whitespace	= 0x0001,
	end_scoped_enum;

	const status_t	initialize( /*const pp::context & context, f1::io::Iostream * str = nullptr, */output_flags_t flags = output_flags_t::null );

			const status_t	move( self * obj );
	virtual const status_t	move( core::object * obj ) noexcept override;

	// Properties
public:
	const status_t	modify_flags( output_flags_t flags_to_set, output_flags_t flags_to_clear = output_flags_t::null, output_flags_t * prev_flags = nullptr );
	output_flags_t	get_flags( output_flags_t mask = output_flags_t::all ) const;

	const pp::context *	get_context() const noexcept;

	virtual const status_t	on_attach_to_parser( pp::Iparser & parser ) noexcept;
	virtual const status_t	on_detach_from_parser( pp::Iparser & parser ) noexcept;
	const pp::Iparser *	get_parser() const noexcept;
		  pp::Iparser *	get_parser()	   noexcept;

//	virtual const status_t	set_output_filename( const char * filename, size_t length = size_t(-1) );
//	virtual const status_t	set_output_stream( f1::io::Iostream & str );
//	f1::io::Iostream *	get_output_stream();
protected:
//	core::Tconst_weak_ref<pp::context>	m_context;
//	core::Tref<io::Iostream>			m_stream;
	core::Tweak_ref<pp::Iparser>	m_parser;
	output_flags_t	m_output_flags;

	// Operations
public:
	virtual const status_t	begin_output() noexcept;
	virtual const status_t	end_output() noexcept;

	virtual const status_t	begin_subsection( const char * ident, size_t len = size_t(-1) ) noexcept;
	virtual const status_t	begin_subsection( const core::static_string & ident ) noexcept;
	virtual const status_t	begin_subsection( const pp::string & ident ) noexcept;
	virtual const status_t	end_subsection() noexcept;

	virtual const status_t	write( const char * string, size_t len = size_t(-1) ) abstract;
	virtual const status_t	writefv( const char * format, va_list ap );
	virtual const status_t	writef( const char * format, ... );

	virtual const status_t	on_file_changed( const char * filename, f1::parser::lineno_t lineno ) noexcept;
	virtual const status_t	on_line_changed( f1::parser::lineno_t lineno ) noexcept;
	const core::string &	get_filename() const;
	parser::lineno_t		get_line() const;
	parser::lineno_t		get_global_line() const;

	const core::buffer &	get_buffer() const noexcept;
		  core::buffer &	get_buffer()	   noexcept;
protected:
	core::buffer	m_buffer;
	core::string	m_filename;
	parser::lineno_t	m_lineno;
	parser::lineno_t	m_global_lineno;
	unsigned		m_subsection_count;
};

/*END OF Ioutput.hxx*/
