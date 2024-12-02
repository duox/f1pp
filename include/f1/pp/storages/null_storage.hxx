/*null_storage.hxx*/

class F1_PP_API f1::pp::null_storage_class : public f1::pp::Istorage_class
{
public:
	typedef f1::pp::null_storage_class	self;
	typedef f1::pp::Istorage_class		super;

	explicit null_storage_class( const core::Iidentifier * ident = nullptr ) noexcept;

	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const noexcept override;
	virtual const char *		get_name() const noexcept override;

	virtual const status_t	new_storage( core::Tref<pp::Istorage> & res, const core::Iidentifier * ident = nullptr ) noexcept override;
};

class F1_PP_API f1::pp::null_storage : public f1::pp::Istorage
{
public:
	typedef f1::pp::null_storage		self;
	typedef f1::pp::Istorage			super;

	explicit null_storage( const core::Iidentifier * ident = nullptr ) noexcept;

	const status_t	initialize( const pp::context & context, storage_flags_t flags = storage_flags_t::null ) override;

	virtual const status_t	on_write( const pp::string & str ) noexcept override;
//	virtual const status_t	set_pos( f1::io::abs_offset_t pos ) noexcept override;
//	virtual f1::io::abs_offset_t	get_pos() const noexcept override;
};

/*END OF null_storage.hxx*/
