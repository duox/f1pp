/*enumeration.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::enumeration : public f1::ns::name_space
{
public:
	typedef f1::ns::name_space		super;
	typedef f1::f1pp::enumeration	self;

	explicit enumeration(/* const core::identifier * ident = nullptr */);

	const status_t	initialize( f1pp::parser & par, const pp::string & name );

	// Properties
public:
	const f1pp::parser *	get_parser() const;
		  f1pp::parser *	get_parser();

	static const unsigned	fl_deprecated	= 0x0001;	///< the enumeration and all its members are deprecated
	static const unsigned	fl_sealed		= 0x0002;	///< this enumeration cannot be used as the base enumeration
	static const unsigned	fl_open			= 0x0004;	///< members of the enumeration are used without namespace specification (bare)
	static const unsigned	fl_bitwise		= 0x0008;	///< bitwise increment
	const status_t	modify_flags( unsigned flags_to_set, unsigned flags_to_clear = 0, unsigned * prev_flags = nullptr );
	unsigned	get_flags( unsigned mask = unsigned(-1) ) const;

	const f1::ns::name_space &	get_element_namespace() const;
		  f1::ns::name_space &	get_element_namespace();

	const status_t	set_definition_location( f1pp::parser & parser );
	const status_t	set_definition_location( f1::parser::Isource & src, f1::parser::lineno_t line );
protected:
	core::Tweak_ref<f1pp::parser>	m_parser;
	//f1::ns::name_space				m_ns;		///< namespace where all elements are located

	core::Tweak_ref<f1::parser::Isource>	m_definition_source;
	f1::parser::lineno_t					m_definition_line;

	bitflags_t	m_flags;

	// Base enumerations
public:
	const status_t	append_base_enumeration( const char * name, size_t len = size_t(-1) );
	const status_t	append_base_enumeration( const pp::string & name );
	const status_t	append_base_enumeration( const enumeration & en );

	const const_enumeration_list_t &	get_base_enumerations() const;
protected:
	const_enumeration_list_t	m_base_enums;
	bool _is_base_enumeration( const enumeration & en ) const;

	// Elements
public:
	const status_t	append_element( const pp::string & name, const pp::variant & value, const pp::attribute_list * attrs = nullptr );
	const status_t	append_element( const char * name, size_t len, const pp::variant & value, const pp::attribute_list * attrs = nullptr );
	const status_t	remove_element( pp::Imacro & macro );
	const status_t	remove_all_elements();

	const pp::Imacro *	get_element( const char * name, size_t len = size_t(-1) ) const;
		  pp::Imacro *	get_element( const char * name, size_t len = size_t(-1) );
	const pp::Imacro *	get_element( const pp::variant & value ) const;
		  pp::Imacro *	get_element( const pp::variant & value );
	
	const pp::macro_list_t &	get_element_list() const;
protected:
	pp::macro_list_t	m_element_list;

	// Operations
public:
	const status_t	display_enumeration_location( pp::Iparser & par, msgout::msgid_t msgid ) const;
	bool			get_next_value( f1pp::parser & par, pp::variant & value, status_t & status ) const;
protected:
};

/*END OF enumeration.hxx*/
