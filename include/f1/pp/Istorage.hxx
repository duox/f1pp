/*Istorage.hxx*/
/** @file
 *
 * @brief Storage interface definition.
 *
 *	Storages actually keep physical data formatted by outputs.
 */

interface F1_PP_API f1::pp::Istorage : public f1::pp::object
{
	typedef f1::pp::Istorage	self;
	typedef f1::pp::object		super;

	explicit Istorage( const core::Iidentifier * ident = nullptr ) __noexcept;
			~Istorage() __noexcept;

	begin_scoped_enum( storage_flags_t )
		fl_append_mode	= 0x0001,
	end_scoped_enum;
	virtual const status_t	initialize( const pp::context & context, storage_flags_t flags = storage_flags_t::null );
	virtual const status_t	initialize( const pp::context & context, const core::string & output_filename, storage_flags_t flags = storage_flags_t::null );
	using super::initialize;

			const status_t	move( self * obj );
	virtual const status_t	move( core::object * obj ) noexcept override;

	// Properties
public:
	const pp::context *	get_context() const __noexcept;

	const status_t	modify_flags( storage_flags_t flags_to_set, storage_flags_t flags_to_clear = storage_flags_t::null, storage_flags_t * prev_flags = nullptr );
	storage_flags_t	get_flags( storage_flags_t mask = storage_flags_t::all ) const;

	virtual bool			get_newline_flag() const;
	virtual const status_t	set_newline_flag( bool on_newline );

	interface Ihook;
	const status_t	append_hook( Ihook & hk );
	const status_t	remove_hook( Ihook & hk );
protected:
	core::Tconst_weak_ref<pp::context>	m_context;
	core::string	m_output_filename;
	storage_flags_t	m_storage_flags;
	bool			m_was_nl;

	core::Tarray<Ihook *>	m_hook_list;

	// Operations
public:
	const status_t	write( const char * string, size_t len = size_t(-1) );
	const status_t	write( const pp::string & str );

	const status_t	flush();

	const status_t	open();
	const status_t	close();

//	virtual const status_t	set_pos( f1::io::abs_offset_t pos ) abstract;
//	virtual f1::io::abs_offset_t	get_pos() const abstract;
protected:
	virtual const status_t	on_write( const pp::string & str ) abstract;
	virtual const status_t	on_flush();
	virtual const status_t	on_open();
	virtual const status_t	on_close();
};

interface F1_PP_API f1::pp::Istorage::Ihook
{
	virtual ~Ihook() noexcept;
	virtual const status_t	on_attach( Istorage & stg );
	virtual const status_t	on_detach();

	virtual const status_t	on_before_write( const pp::string & str );
	virtual const status_t	on_after_write ( const pp::string & str );
	virtual const status_t	on_flush();
	virtual const status_t	on_open();
	virtual const status_t	on_close();

protected:
	core::Tweak_ref<pp::Istorage>	m_storage;
};

/*END OF Istorage.hxx*/
