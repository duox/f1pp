/*Idirective.hxx*/
/** @file
 *
 *	Directives are independent to parsers and contexts; all context that they must store in parser
 * they are attached to. That allows us to use static objects.
 */

interface F1_PP_API f1::pp::Idirective : public f1::pp::object
{
public:
	typedef f1::pp::object			super;
	typedef f1::pp::Idirective	self;

	explicit Idirective( unsigned flags = 0 ) __noexcept;
			~Idirective() __noexcept;

	static const f1::guid_t m_item_class_guid;

	// Properties
public:
	virtual const char *	get_name() const abstract;
	virtual const f1::guid_t &	get_guid() const abstract;

	static const unsigned cap_invariant		= 0x0001UL;	///< always should be executed even in skipped blocks (e.g. #if 0)
	static const unsigned cap_c_extension	= 0x0002UL;	///< C preprocessor extension
	static const unsigned cap_keep_text_eol	= 0x0004UL;	///< do not automatically check end-of-line condition when Idirective::parse is finished
	static const unsigned cap_unimplemented	= 0x0008UL;	///< directive is not implemented and must be passed through
	static const unsigned cap_hard_edge		= 0x0010UL;	///< the `#continue' directive can start new iteration and the `#break' directive can leave iteration
	static const unsigned cap_forced		= 0x0020UL;	///< directive is processed in any case, even in restricted modes (like execute-top-only)
	static const unsigned cap_structured	= 0x0040UL;	///< structure directive, ended with #end
	static const unsigned cap_override		= 0x0080UL;	///< the directive overrides another one with the same GUID
	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept;

	static const unsigned bf_disabled		= 0x0001UL;	///< directive is disabled
	static const unsigned bf_deprecated		= 0x0002UL;	///< directive usage invokes "deprecated" warning
	static const unsigned bf_pass_through	= 0x0004UL;	///< do not invoke parse() for the directive, send it to output unchanged
	static const unsigned bf_ignore			= 0x0008UL;	///< silently ignore directive
	virtual const status_t	modify_flags( unsigned flags_to_set, unsigned flags_to_clear = 0, unsigned * flags_prev = nullptr ) __noexcept;
	unsigned	get_flags( unsigned mask = unsigned(-1) ) const __noexcept;
protected:
	bitflags_t		m_flags;		// directive flags

	// Supported attributes
/*public:
	const attribute_manager *	get_attribute_manager() const;
		  attribute_manager *	get_attribute_manager();

	const status_t	append_attribute_class( const f1::guid_t & guid, const char * name, size_t len = size_t(-1) );
	const status_t	append_attribute_class( const f1::guid_t & guid, const pp::string & name );
	const status_t	append_attribute_class( const Iattribute_class & cls );
	const status_t	remove_attribute_class( const Iattribute_class & cls );
	const status_t	remove_all_attribute_classes();

	const status_t	find_attribute_class( core::Tconst_ref<pp::Iattribute_class> & res,
		const f1::guid_t & guid,
		const pp::arg_list * args = nullptr ) const;
	const status_t	find_attribute_class( core::Tconst_ref<pp::Iattribute_class> & res,
		const pp::string & name,
		const pp::arg_list * args = nullptr ) const;
	const status_t	find_attribute_class( core::Tconst_ref<pp::Iattribute_class> & res,
		const char * name, size_t len = size_t(-1),
		const pp::arg_list * args = nullptr ) const;
protected:
	core::Tref<pp::attribute_manager>	m_attribute_manager;*/

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) abstract;
	virtual Iframe *		alloc_idle_frame( f1::pp::Iparser & parser );
};

#define f1_directive_definition( class_name )	\
	class class_name : public f1::pp::directive\
	{\
	public:\
		static class_name		instance;\
		static const f1::guid_t	guid;\
		\
		const f1::guid_t &	get_guid() const override;\
		unsigned			get_caps( unsigned mask = unsigned(-1) ) const override;\
		const char *		get_name() const override;\
		\
		status_t			parse( pp::Iparser & parser ) override;\
	private:\
	}
#define f1_directive_implementation( class_name, class_guid, class_name_string, class_caps )	\
	const f1::guid_t class_name::guid = class_guid;\
	\
	class_name	class_name::instance;\
	\
	const f1::guid_t & class_name::get_guid() const\
	{\
		return guid;\
	}\
	unsigned class_name::get_caps( unsigned mask ) const override\
	{\
		return (class_caps) & mask;\
	}\
	const char * class_name::get_name() const override\
	{\
		return class_name_string;\
	}

/*END OF Idirective.hxx*/
