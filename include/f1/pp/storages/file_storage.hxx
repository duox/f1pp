/*file_storage.hxx*/

class F1_PP_API f1::pp::file_storage_class : public f1::pp::Istorage_class
{
public:
	typedef f1::pp::file_storage_class	self;
	typedef f1::pp::Istorage_class		super;

	explicit file_storage_class( const core::Iidentifier * ident = nullptr ) noexcept;

	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const noexcept override;
	virtual const char *		get_name() const noexcept override;

	virtual const status_t	new_storage( core::Tref<pp::Istorage> & res, const core::Iidentifier * ident = nullptr ) noexcept override;
};

class F1_PP_API f1::pp::file_storage : public f1::pp::stream_storage
{
public:
	typedef f1::pp::file_storage		self;
	typedef f1::pp::stream_storage		super;

	explicit file_storage( const core::Iidentifier * ident = nullptr ) noexcept;

	virtual const status_t	initialize( const pp::context & context, const core::string & filename, storage_flags_t storage_flags = storage_flags_t::null ) override;
};

/*END OF file_storage.hxx*/
