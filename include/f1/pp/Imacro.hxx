/*Imacro.hxx*/
/** @file
 *
 *	Macros are independent to parsers and contexts; all context that they must store in parser
 * they are attached to. That allows us to use static objects.
 */

interface F1_PP_API f1::pp::Imacro : public f1::pp::object
{
	typedef f1::pp::object		super;
	typedef f1::pp::Imacro		self;

	forward_scoped_enum( macro_flags_t );

	explicit Imacro( const core::Iidentifier * ident = nullptr ) NOEXCEPT;
			~Imacro() NOEXCEPT;

	virtual const status_t	initialize( const f1::pp::attribute_list * attrs,
		const pp::param_list * params, const pp::string * value, macro_flags_t flags = null );
	const status_t	initialize( macro_flags_t flags = null );
	const status_t	initialize( const f1::pp::attribute_list * attrs,
		const pp::param_list * params = nullptr, macro_flags_t flags = null );

	static const f1::guid_t	m_item_class_guid;

	// Properties
public:
//	const pp::context *	get_context() const;
//		  pp::context *	get_context();

	begin_scoped_enum( macro_flags_t )
		fl_being_expanded		= 0x0001,	//< macro is currently being expanded (TODO: move to internal flags)
		fl_undefined			= 0x0002,	//< macro is temporary in "undefined" state
		fl_do_not_expand		= 0x0004,	//< lock macro from expanding
		fl_immutable			= 0x0008,	//< cannot be [re]defined or undefined
		fl_disabled				= 0x0010,	//< macro is disabled and cannot be used
		fl_self_parse_arglist	= 0x0020,	//< macro parses arglist itself (used by defined() pseudo-macro)
		fl_predefined			= 0x0040,	//< macro is predefined by syntax driver
		fl_deprecated			= 0x0080,	//< macro is deprecated and issues a warning each time being used
		fl_ignore				= 0x0100,	//< do not expand macro, as if it was defined empty
		fl_expression_only		= 0x0200,	//< macro can only be used inside expressions (e.g. in #if/#elif)
		fl_keep_new_lines		= 0x0400,	//< do not remove new-lines from the macro body
		fl_weak					= 0x0800,	//< allow redefine macros without complains
		fl_operator				= 0x1000,	//< can be used in expressions but cannot be used in #ifdef/#ifndef
		fl_recursive_locked		= 0x2000,	//< recursive macro blocked in rescan step
		fl_unique				= 0x4000,	//< the macro is unique and cannot be used w/o arguments
		fl_function				= 0x8000,	//< functions expsnd macros and then scan for closing parenthesis
	end_scoped_enum;
	const status_t	modify_flags( macro_flags_t flags_to_set, macro_flags_t flags_to_clear = null, macro_flags_t * prev_flags = nullptr );
	macro_flags_t	get_flags	( macro_flags_t mask = macro_flags_t::all ) const;

	virtual const char *	get_name() const abstract;

	const f1::pp::attribute_list *	get_attribute_list() const;
		  f1::pp::attribute_list *	get_attribute_list();
	const status_t	set_attribute_list( const f1::pp::attribute_list * al );

	const pp::param_list *	get_param_list() const;
		  pp::param_list *	get_param_list();
	const status_t		set_param_list( const pp::param_list * pl );

	const parser::Isource *	get_definition_source() const;
		  parser::Isource *	get_definition_source();
		  parser::lineno_t	get_definition_line() const;
	const status_t	set_definition_location( parser::Isource & src, parser::lineno_t line );

	virtual const f1::pp::string &		get_value() const;
	virtual const f1::pp::variant &		get_value_variant() const;
	virtual const f1::core::md5hash::digest_t &		get_value_hash() const;
	const status_t		set_value_hash( core::md5hash::digest_t & hash );
	const status_t		set_value_variant( const f1::pp::variant & value );
	const status_t		set_value_variant_move( f1::pp::variant & value );
	const status_t		set_value_variant( const f1::pp::variant & value, const f1::pp::string & value_string );
	const status_t		set_value_variant_move( f1::pp::variant & value, f1::pp::string & value_string );
	const status_t		set_value( const char * value, size_t len = size_t(-1) );
	const status_t		set_value( const f1::pp::string & value );
	const status_t		set_value_move( f1::pp::string & value );
	const status_t		append_value( const char * value, size_t len = size_t(-1) );
	const status_t		append_value( const f1::pp::string & value );
	const status_t		append_value_move( f1::pp::string & value );
	virtual bool	pre_change_value( f1::pp::string & string_value, f1::pp::variant * variant_value = nullptr );
	virtual void	post_change_value( const f1::pp::string & string_value, const f1::pp::variant & variant_value );

	const status_t	on_macro_referenced();
	uint32_t	get_reference_count() const;

	const status_t	set_deprecation_text( const f1::pp::string & text );
	const f1::pp::string &	get_deprecation_text() const;

	virtual const status_t	on_attach_to_named_object( ns::named_object & nobj );
	virtual const status_t	on_detach_from_named_object( ns::named_object & nobj );
	const ns::named_object_weak_list_t &	get_named_object_list() const;
		  ns::named_object_weak_list_t &	get_named_object_list();
protected:
	uint32_t	m_expand_count;
	pp::string	m_deprecation_text;
	ns::named_object_weak_list_t	m_named_object_list;

	// Macro value
public:
	const status_t	push_value( const parser::Isource * src = nullptr, parser::lineno_t line = 0 );
	const status_t	pop_value();

	struct definition {
		pp::variant		m_variant;			///< macro value as it was stored or calculated
		pp::string		m_value;			///< macro value textual representation

		core::md5hash::digest_t	m_digest;	///< macro value hash
		bool					m_digest_valid;

		core::Tconst_ref<pp::attribute_list>m_attribute_list;
		core::Tconst_ref<pp::param_list>	m_param_list;
		macro_flags_t	m_macro_flags;		///< flags set manually
		macro_flags_t	m_attr_flags;		///< flags set with the attribute list

		core::Tconst_ref<parser::Isource>	m_definition_source;
		parser::lineno_t					m_definition_line;

		core::Tconst_ref<parser::Isource>	m_alloc_source;		///< file the definition was pushed on the stack
		parser::lineno_t					m_alloc_line;		///< line the definition was pushed on the stack (ignore if m_file is empty)
		bool								m_was_undefined;	// true if fl_undefined flag was set

		definition() noexcept;
	};
	const core::Tarray<definition> &	get_definition_stack() const;
	const definition *	get_first_definition() const;
	const definition *	get_last_definition() const;
protected:
	core::Tarray<definition>	m_def_stack;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser, pp::string & buf ) const;
	virtual const status_t	expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const abstract;
	virtual bool	can_undefine() const;
	virtual void	on_all_names_removed();

	const status_t format_macro_signature( core::string & buf, const char * ident = nullcstr ) const;
protected:
};

/*END OF Imacro.hxx*/
