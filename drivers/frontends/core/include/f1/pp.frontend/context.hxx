/*context.hxx*/

class F1_PP_FRONTEND_API f1::pp::frontend::context : public f1::core::named_object
{
public:
	typedef f1::pp::frontend::context	self;
	typedef f1::core::named_object		super;

	explicit context( const core::Iidentifier * ident = nullptr );
			~context();

	// Parameters
public:
//		  mt::context *	get_mt_context();
//	const mt::context *	get_mt_context() const;
//	virtual const status_t	set_mt_context( mt::context * ctx );

	virtual const status_t	set_thread_count( unsigned count );
	virtual unsigned		get_thread_count() const;

	const f1::guid_t &	get_syntax_guid() const;
	const status_t		set_syntax_guid( const f1::guid_t & guid );

	const f1::guid_t &	get_architecture_guid() const;
	const status_t		set_architecture_guid( const f1::guid_t & guid );
private:
//	f1::core::Tref<mt::context>		m_mt_context;
	core::Tref<mt::Ithread_pool>	m_thread_pool;
	f1::guid_t		m_syntax_guid;	///< initial syntax GUID, used by frontend
	f1::guid_t		m_arch_guid;	///< initial architecture GUID, used by frontend

	struct thread_context_factory : public mt::Ifactory {
		virtual const status_t	new_thread_context( f1::core::Tref<mt::Ithread_context> & res ) override;
		core::Tweak_ref<frontend::context>	m_frontend_context;
	} m_thread_pool_factory;

	// Operations
public:
	virtual const status_t	parse_file( const char * filename, size_t len = size_t(-1) );
	virtual const status_t	parse_data( const void * data, size_t size, const char * filename = nullcstr );
	virtual const status_t	parse( f1::core::buffer & buf, const char * filename = nullcstr );
private:
	struct pp_task : public f1::mt::Itask {
		explicit pp_task( const f1::guid_t & syntax_guid, const f1::guid_t & arch_guid, const core::string & filename ):
			m_syntax_guid( syntax_guid ), m_arch_guid( arch_guid ), m_filename( filename ), m_files_processed( 0 ) {
		}
		explicit pp_task( const f1::guid_t & syntax_guid, const f1::guid_t & arch_guid, const char * filename, size_t len ):
			m_syntax_guid( syntax_guid ), m_arch_guid( arch_guid ), m_filename( filename, len ), m_files_processed( 0 ) {
		}
		virtual void			query_stop( mt::Ithread_context * tc ) override;
	protected:
		f1::guid_t		m_syntax_guid;
		f1::guid_t		m_arch_guid;
		core::string	m_filename;
		unsigned		m_files_processed;
	};
	class pp_buffer_task : public pp_task {
	public:
		explicit pp_buffer_task( const f1::guid_t & syntax_guid, const f1::guid_t & arch_guid, const core::buffer & buf, const char * filename ):
			pp_task( syntax_guid, arch_guid, filename ),
			m_buffer( buf ) {
		}
		virtual const status_t	run( void * param, mt::Ithread_context * tc ) override;
	protected:
		core::buffer	m_buffer;
	};
	class pp_mem_task : public pp_task {
	public:
		explicit pp_mem_task( const f1::guid_t & syntax_guid, const f1::guid_t & arch_guid, const void * ptr, size_t size, const char * filename ):
			pp_task( syntax_guid, arch_guid, filename ),
			m_ptr( ptr ), m_size( size ) {
		}
		virtual const status_t	run( void * param, mt::Ithread_context * tc ) override;
	protected:
		const void *	m_ptr;
		size_t			m_size;
	};
	class pp_file_task : public pp_task {
	public:
		explicit pp_file_task( const f1::guid_t & syntax_guid, const f1::guid_t & arch_guid, const core::string & filename ):
			pp_task( syntax_guid, arch_guid, filename ) {
		}
		explicit pp_file_task( const f1::guid_t & syntax_guid, const f1::guid_t & arch_guid, const char * filename, size_t len ):
			pp_task( syntax_guid, arch_guid, filename, len ) {
		}
		virtual const status_t	run( void * param, mt::Ithread_context * tc ) override;
	protected:
		core::string	m_filename;
	};
	core::Tobject_array<pp_task>	m_task_list;
};

/*END OF context.hxx*/
