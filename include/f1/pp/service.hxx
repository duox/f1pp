/*service.hxx*/

class F1_PP_API f1::pp::service : public f1::core::Tservice<pp::context, pp::Iprovider, pp::service>
{
public:
			 service() __noexcept;
	virtual ~service() __noexcept;

	virtual const status_t	preinitialize() override;
	virtual const status_t	deinitialize() noexcept override;

	static const f1::guid_t	m_guid;
	const f1::guid_t &	get_guid() const __noexcept override;

	static const unsigned VERSION_MAJOR		= 0;
	static const unsigned VERSION_MINOR		= 6;
	static const unsigned VERSION_BUILD		= 0;
	static const unsigned VERSION_SUBVERSION= 0;
	static const unsigned VERSION_FLAGS		= core::version::fl_preliminary;
	const status_t	get_version( f1::core::version & ver ) const __noexcept override;

	//pp_provider *	get_internal_provider() const;

	static const unsigned fl_debug_output				= 0x0001;	///< advanced debug output
	const status_t	modify_flags( unsigned flags_to_set, unsigned flags_to_clear = 0, unsigned * flags_prev = nullptr ) noexcept;
	unsigned		get_flags( unsigned mask = unsigned(-1) ) const noexcept;
private:
	f1::core::plugin_set			m_plugin_set;
	f1::core::Tref<pp::Iprovider>	m_internal_provider;
	bitflags_t	m_flags;

	// Syntaxes
public:
	const status_t	append_syntax( pp::Isyntax & syntax ) __noexcept;
	const status_t	remove_syntax( pp::Isyntax & syntax ) __noexcept;
	const status_t	remove_all_syntaxes() __noexcept;

	const pp::Isyntax * get_default_syntax() const;
	const status_t	set_default_syntax( pp::Isyntax * syntax );

	const status_t	get_syntax( core::Tconst_ref<pp::Isyntax> & res, const f1::guid_t & guid ) const;

	const syntax_list_t &	get_syntax_list() const __noexcept;
		  syntax_list_t &	get_syntax_list()		__noexcept;
private:
	syntax_list_t	m_syntaxes;
	f1::core::Tref<pp::Isyntax>	m_default_syntax;

	// Standards
public:
	const status_t	append_standard( pp::Istandard & standard ) __noexcept;
	const status_t	remove_standard( pp::Istandard & standard ) __noexcept;
	const status_t	remove_all_standards() __noexcept;

	const pp::Istandard * get_default_standard() const;
	const status_t	set_default_standard( pp::Istandard * standard );

	const status_t	get_standard( core::Tconst_ref<pp::Istandard> & res, const f1::guid_t & guid ) const;
	const status_t	get_standard( core::Tconst_ref<pp::Istandard> & res, const char * name, ptrdiff_t len = -1 ) const;

	const standard_list_t &	get_standard_list() const __noexcept;
		  standard_list_t &	get_standard_list()		  __noexcept;

	const Istandard *	find_item_standard( const f1::guid_t & item_class_guid, const pp::string & name ) const;
private:
	standard_list_t	m_standards;
	f1::core::Tref<pp::Istandard>	m_default_standard;

	// Outputs
public:
	const status_t	create_output( core::Tref<pp::Ioutput> & res, const f1::guid_t & output_class_guid, /*pp::context & ctx, */const core::Iidentifier * ident = nullptr ) __noexcept;
	const status_t	create_output( core::Tref<pp::Ioutput> & res, Ioutput_class & output_class, /*pp::context & ctx, */const core::Iidentifier * ident = nullptr ) __noexcept;

	const status_t	append_output_class( pp::Ioutput_class & output_class ) __noexcept;
	const status_t	remove_output_class( pp::Ioutput_class & output_class ) __noexcept;
	const status_t	remove_all_output_classes() __noexcept;

	const pp::Ioutput_class * get_default_output_class() const;
	const status_t	set_default_output_class( pp::Ioutput_class * output_class );

	const status_t	get_output_class( core::Tconst_ref<pp::Ioutput_class> & res, const f1::guid_t & guid ) const;

	const output_class_list_t &	get_output_class_list() const __noexcept;
		  output_class_list_t &	get_output_class_list()		  __noexcept;
private:
	output_class_list_t	m_output_classes;
	f1::core::Tref<pp::Ioutput_class>	m_default_output_class;

	// Storages
public:
	const status_t	create_storage( core::Tref<pp::Istorage> & res,
		const f1::guid_t & storage_class_guid, pp::context & ctx,
		const core::string & output_filename, Istorage::storage_flags_t flags = Istorage::storage_flags_t::null,
		const core::Iidentifier * ident = nullptr ) __noexcept;
	const status_t	create_storage( core::Tref<pp::Istorage> & res,
		Istorage_class & storage_class, pp::context & ctx,
		const core::string & output_filename, Istorage::storage_flags_t flags = Istorage::storage_flags_t::null,
		const core::Iidentifier * ident = nullptr ) __noexcept;

	const status_t	append_storage_class( pp::Istorage_class & storage_class ) __noexcept;
	const status_t	remove_storage_class( pp::Istorage_class & storage_class ) __noexcept;
	const status_t	remove_all_storage_classes() __noexcept;

	const pp::Istorage_class * get_default_storage_class() const;
	const status_t	set_default_storage_class( pp::Istorage_class * storage_class );

	const status_t	get_storage_class( core::Tconst_ref<pp::Istorage_class> & res, const f1::guid_t & guid ) const;

	const storage_class_list_t &	get_storage_class_list() const __noexcept;
		  storage_class_list_t &	get_storage_class_list()	   __noexcept;
private:
	storage_class_list_t	m_storage_classes;
	f1::core::Tref<pp::Istorage_class>	m_default_storage_class;
	file_storage_class					m_file_storage_class;

	// Helper functions
public:
	static const status_t compact_spaces( pp::string & res, const char * str, size_t len );

	static const status_t trim_spaces( pp::string & str, bool * was_space_in_the_end = nullptr );
	static const status_t trim_spaces( const pp::string & str, size_t & new_offset, size_t & new_len, bool * was_space_in_the_end = nullptr );
	static const status_t trim_spaces( const char * & str, size_t & new_len, bool * was_space_in_the_end = nullptr );

	core::md5hash &	get_hash_function();
	const status_t	calc_blob_hash( core::md5hash::digest_t & res, const void * data, size_t size );
protected:
	core::md5hash	m_hash_function;
};

/*END OF service.hxx*/
