/*syntax.hxx*/

class F1_PP_API f1::c_pp::common::syntax : public f1::pp::Isyntax
{
public:
	typedef f1::pp::Isyntax		super;
	typedef f1::c_pp::syntax	self;

	explicit syntax( const core::Iidentifier * ident = nullptr );

	static const f1::guid_t	m_guid;
	virtual const f1::guid_t &	get_guid() const override;
	virtual const char *	get_ident_string() const override;
	virtual const char *	get_description_string() const override;

	virtual const status_t	attach_to_context( pp::context & ctx ) override;
	virtual const status_t	detach_from_context( pp::context & ctx ) override;

	virtual const status_t	new_parser( f1::core::Tref<pp::Iparser> & res, const core::Iidentifier * ident = nullptr ) override;
	virtual const status_t	new_macro( f1::core::Tref<pp::Imacro> & res, ns::name * nm ) override;

public:
	enum {
		// Fatal errors
		IMSG_FATAL_ERROR_FILE_NOT_FOUND = f1::msgout::fatal_error_group::DEFAULT_MIN,
		IMSG_FATAL_ERROR_INVALID_FILE_NAME,
		IMSG_FATAL_ERROR_USER_N,
		// Errors
		IMSG_ERROR_EXPECTED_DIRECTIVE_NAME = f1::msgout::error_group::DEFAULT_MIN,
		IMSG_ERROR_UNKNOWN_DIRECTIVE,
		IMSG_ERROR_UNKNOWN_FUNCTION,
		IMSG_ERROR_EXPECTED_S,
		IMSG_ERROR_EXPECTED_IDENTIFIER,
		IMSG_ERROR_EXPECTED_INTEGER,
		IMSG_ERROR_EXPECTED_OPERATOR,
		IMSG_ERROR_EXPECTED_EXPRESSION,
		IMSG_ERROR_EXPECTED_INCLUDE_FILENAME,
		IMSG_ERROR_UNEXPECTED,
		IMSG_ERROR_UNEXPECTED_DIRECTIVE,
		IMSG_ERROR_UNEXPECTED_EOF,
		IMSG_ERROR_UNEXPECTED_MACRO_END,
		IMSG_ERROR_UNTERMINATED_COMMENT,
		IMSG_ERROR_UNTERMINATED_STRING,
		IMSG_ERROR_UNTERMINATED_DIRECTIVE,
		IMSG_ERROR_TOO_LONG_CHAR_CONSTANT,
		IMSG_ERROR_INVALID_INTEGER_OPERAND,
		IMSG_ERROR_INVALID_ESCAPE_SEQUENCE,
		IMSG_ERROR_INVALID_CHAR_CONSTANT,
		IMSG_ERROR_DIVISION_BY_ZERO,
		IMSG_ERROR_OPERATOR_IS_NOT_SUPPORTED,
		IMSG_ERROR_MACRO_ALREADY_DEFINED,
		IMSG_ERROR_MACRO_IS_IMMUTABLE,
		IMSG_ERROR_USER_N,
		// Warnings
		IMSG_WARNING_EXPECTED_S = f1::msgout::warning_group::DEFAULT_MIN,
		IMSG_WARNING_UNEXPECTED_CHARACTERS,
		IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER,
		IMSG_WARNING_UNEXPECTED_DIRECTIVE,
		IMSG_WARNING_UNKNOWN_PRAGMA,
		IMSG_WARNING_UNDEFINED_IDENTIFIER_IN_EXPR,
		IMSG_WARNING_UNDEFINED_IDENTIFIER_IN_UNDEF,
		IMSG_WARNING_NEGATIVE_SHIFT_AMOUNT,
		IMSG_WARNING_EXCESSIVE_INTEGER_SUFFIX,
		IMSG_WARNING_UNARY_MINUS_APPLIED_TO_UINT,
		IMSG_WARNING_UNARY_PLUS_APPLIED_TO_UINT,
		IMSG_WARNING_DEPRECATED_DIRECTIVE,
		IMSG_WARNING_DEPRECATED_MACRO,
		IMSG_WARNING_EMPTY_MACRO_STACK,
		IMSG_WARNING_DIFFERENT_SOURCES,
		// Information messages
		IMSG_INFO_DEFINED_HERE = f1::msgout::information_group::DEFAULT_MIN,
		IMSG_INFO_STARTED_HERE,
		// Raw messages
		IMSG_RAW = f1::msgout::raw_group::DEFAULT_MIN,	// "%s"
		IMSG_RAW_N,			// "%.*s"
		IMSG_RAW_INDENT,	// "%*s"
		IMSG_RAW_INDENT_N,	// "%*.*s"
	};
	static const f1::msgout::group::message_desc m_msg_fatal_error_descs[];
	static const f1::msgout::group::message_desc m_msg_error_descs[];
	static const f1::msgout::group::message_desc m_msg_warning_descs[];
	static const f1::msgout::group::message_desc m_msg_info_descs[];
	static const f1::msgout::group::message_desc m_msg_message_descs[];
protected:
	f1::msgout::fatal_error_group	m_msgout_fatal_error_group;
	f1::msgout::error_group			m_msgout_error_group;
	f1::msgout::warning_group		m_msgout_warning_group;
	f1::msgout::information_group	m_msgout_info_group;
	f1::msgout::raw_group			m_msgout_message_group;

	f1::msgout::source_memory	m_msgout_string_source;

	// Directives
/*public:
	virtual const directive_list *	get_directive_list() const;
				  directive_list *	get_directive_list();
protected:
	directive_list	m_directive_list;

	define_directive	m_define_directive;
	elif_directive		m_elif_directive;
	else_directive		m_else_directive;
	endif_directive		m_endif_directive;
	error_directive		m_error_directive;
	if_directive		m_if_directive;
	ifdef_directive		m_ifdef_directive;
	ifndef_directive	m_ifndef_directive;
	include_directive	m_include_directive;
	line_directive		m_line_directive;
	pragma_directive	m_pragma_directive;
	undef_directive		m_undef_directive;
*/
	defined_function	m_defined_function;

	// Macro expansion operators
public:
/*	const status_t	append_macro_operator( Imacro_operator & op );
	const status_t	remove_macro_operator( Imacro_operator & op );
	const status_t	remove_all_macro_operators();

	const macro_operator_list_t &	get_macro_operator_list() const;

	const status_t	parse_operator( core::Tref<Imacro_operator> & op, const char *& start, const char * end ) const;
protected:
	macro_operator_list_t	m_macro_operator_list;*/
};

/*END OF syntax.hxx*/
