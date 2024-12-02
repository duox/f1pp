/*stream_storage.hxx*/

class F1_PP_API f1::pp::stream_storage_class : public f1::pp::Istorage_class
{
public:
	typedef f1::pp::stream_storage_class	self;
	typedef f1::pp::Istorage_class			super;

	explicit stream_storage_class( const core::Iidentifier * ident = nullptr ) noexcept;

	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const noexcept override;
	virtual const char *		get_name() const noexcept override;

	virtual const status_t	new_storage( core::Tref<pp::Istorage> & res, const core::Iidentifier * ident = nullptr ) noexcept override;
};

class F1_PP_API f1::pp::stream_storage : public f1::pp::Istorage
{
public:
	typedef f1::pp::stream_storage		self;
	typedef f1::pp::Istorage			super;

	explicit stream_storage( const core::Iidentifier * ident = nullptr ) noexcept;

	virtual const status_t	initialize( const pp::context & context, storage_flags_t flags = storage_flags_t::null ) override;
	virtual const status_t	initialize( const pp::context & context, const core::string & output_filename, storage_flags_t flags = storage_flags_t::null ) override;
	const status_t	initialize( const pp::context & context, io::Iostream & str, storage_flags_t storage_flags = storage_flags_t::null );

			const status_t	move( core::object * obj ) noexcept override;
	virtual const status_t	move( self * obj );

	const io::Iostream *	get_stream() const;
		  io::Iostream *	get_stream();
	const status_t	set_stream( io::Iostream & str );

	virtual const status_t	on_write( const pp::string & str ) noexcept override;
protected:
	core::Tref<io::Iostream>	m_stream;
};

/*END OF stream_storage.hxx*/
