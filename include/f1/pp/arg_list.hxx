/*arg_list.hxx*/

class F1_PP_API f1::pp::arg_list : public f1::pp::object
{
public:
	typedef f1::pp::object			super;
	typedef f1::pp::arg_list		self;

	~arg_list() noexcept;

	// Properties
public:
protected:

	// Parameters
public:
	struct arg_desc_t {
		pp::string	m_value;
		pp::string	m_expanded_value;
	};
	typedef core::Tarray<arg_desc_t>	arg_desc_list_t;

	const status_t	append_arg( const char * value, size_t len = size_t(-1) );
	const status_t	append_arg( const pp::string & value, size_t count = 1 );
	const status_t	append_arg( const arg_desc_t & desc, size_t count = 1 );
	const status_t	append_expanded_arg( const pp::string & value, const pp::string & expanded_value );
	const status_t	remove_all_args();

	const pp::string *	get_arg_value( const pp::param_list & params, size_t index ) const;
	const pp::string *	get_arg_expanded_value( const pp::param_list & params, size_t index ) const;
	const arg_desc_t *	get_arg_desc( size_t index ) const;

	const arg_desc_list_t &	get_arg_list() const;
		  arg_desc_list_t &	get_arg_list();

	const status_t	format( core::string & buf ) const noexcept;
protected:
	arg_desc_list_t	m_args;

	// Operations
public:
	const status_t	output( pp::Iparser & parser ) const;
};

/*END OF arg_list.hxx*/
