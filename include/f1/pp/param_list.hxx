/*param_list.hxx*/

class F1_PP_API f1::pp::param_list : public f1::pp::object
{
public:
	typedef f1::pp::object			super;
	typedef f1::pp::param_list		self;

	~param_list() noexcept;

	// Properties
public:
protected:

	// Parameters
public:
	static const unsigned	fl_vararg			= 0x0001;	///< parameter is vararg
	static const unsigned	fl_default_value	= 0x0002;	///< m_default_value is valid
	struct param_desc_t {
		pp::string	m_name;
		pp::string	m_default_value;
		unsigned	m_flags;
	};
	typedef core::Tarray<param_desc_t>	param_desc_list_t;

	const status_t	append_param( unsigned flags, const char * name, size_t len = size_t(-1) );
	const status_t	append_param( unsigned flags, const core::string & name );
	const status_t	append_param( unsigned flags, const core::stringslice & name );
	const status_t	append_param( const param_desc_t & desc, size_t count = 1 );
	const status_t	remove_all_params();

	const param_desc_list_t &	get_param_list() const;
	const param_desc_t * 		get_param_desc( size_t index ) const;

	signed	find_param( const pp::string & name ) const;

	virtual bool	__nodiscard	equal( const core::object * obj ) const noexcept override;
			bool	__nodiscard	equal( const param_list & params ) const;

	bool	match_param_list( const param_list & params ) const;
	bool	match_arg_list( const arg_list & args ) const;
	static bool	match_arg_list( const param_list * params, const arg_list * args );

	const status_t	format( core::string & buf ) const noexcept;
protected:
	param_desc_list_t	m_params;
};

/*END OF param_list.hxx*/
