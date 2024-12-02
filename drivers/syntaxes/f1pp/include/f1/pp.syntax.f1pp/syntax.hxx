/*syntax.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::syntax : public f1::c_pp::syntax
{
public:
	typedef f1::c_pp::syntax	super;
	typedef f1::f1pp::syntax	self;

	explicit syntax( const core::Iidentifier * ident = nullptr );

	const status_t	initialize();

	static const f1::guid_t	m_guid;
	virtual const f1::guid_t &	get_guid() const override;
	virtual const char *	get_ident_string() const override;
	virtual const char *	get_description_string() const override;

	virtual const f1::guid_t *	get_base_guid_list( __out size_t & count ) const override;
	virtual const f1::guid_t *	get_preferred_standard_list( __out size_t & count ) const override;

	virtual const status_t	attach_to_context( pp::context & frm ) override;
	virtual const status_t	detach_from_context( pp::context & frm ) override;

	virtual const status_t	on_attach_to_parser( pp::Iparser & par ) override;
	virtual const status_t	on_detach_from_parser( pp::Iparser & par ) override;
	virtual bool	is_item_supported( const f1::guid_t & item_class_guid, const pp::string & name ) const override;

	virtual const status_t	new_parser( f1::core::Tref<pp::Iparser> & res, const core::Iidentifier * ident = nullptr ) override;
	virtual const status_t	new_macro( f1::core::Tref<pp::Iuser_macro> & res, const core::Iidentifier * ident = nullptr ) override;

	virtual const status_t	process_command_line_switch( int switchar, const core::static_string & name, const core::static_string & param ) override;

public:
	enum {
		// Fatal errors
		IMSG_FATALERROR_TOO_MANY_ITERATIONS =	f1::c_pp::syntax::IMSG_FATAL_ERROR_LAST,
		IMSG_FATALERROR_ASSERTION_TRIGGERED,
		IMSG_FATALERROR_ASSERTION_TRIGGERED_S,
		// Errors
		IMSG_ERROR_UNKNOWN_ATTRIBUTE_S = 		f1::c_pp::syntax::IMSG_ERROR_LAST,
		IMSG_ERROR_UNTERMINATED_ATTRIBUTE_LIST,
		IMSG_ERROR_EXPECTED_ATTRIBUTE_NAME,
		IMSG_ERROR_UNEXPECTED_ATTRIBUTES,
		IMSG_ERROR_EXPECTED_LABEL_NAME,
		IMSG_ERROR_LABEL_ALREADY_DEFINED_S,
		IMSG_ERROR_LABEL_NOT_FOUND_S,
		IMSG_ERROR_EXPECTED_POSITIVE_VALUE,	// TODO: remove
		IMSG_ERROR_INVALID_S,
		IMSG_ERROR_CASE_ALREADY_EXECUTED,
		IMSG_ERROR_INVALID_IDENTIFIER,
		IMSG_ERROR_EXPECTED_SINGLE_IDENTIFIER,
		IMSG_ERROR_INVALID_CHARACTER_CODE_X,
		IMSG_ERROR_BAD_ARGUMENT,
		IMSG_ERROR_UNEXPECTED_DIRECTIVE_END,
		IMSG_ERROR_CHANNEL_NOT_FOUND_S,
		IMSG_ERROR_EXPECTED_CHANNEL_NAME,
		IMSG_ERROR_EXPECTED_CHANNEL_FILENAME,
		IMSG_ERROR_CHANNEL_STACK_EMPTY,
		IMSG_ERROR_CHANNEL_ALREADY_DEFINED_S,
		IMSG_ERROR_INVALID_OPERAND_TYPE,
		IMSG_ERROR_INVALID_VALUE,
		IMSG_ERROR_INVALID_VALUE_S,
		IMSG_ERROR_EXPECTED_TYPE_NAME,
		IMSG_ERROR_UNKNOWN_TYPE_NAME_S,
		IMSG_ERROR_INSUFFICIENT_ARGUMENTS,
		IMSG_ERROR_NAME_ALREADY_PRESENT_S,
		IMSG_ERROR_EXPECTED_STRING_OR_IDENT,
		IMSG_ERROR_ALREADY_DEFINED_S,
		IMSG_ERROR_ALREADY_USED_S,
		IMSG_ERROR_ENUM_REQUIRES_INITIALIZER,
		IMSG_ERROR_ENUM_IS_SEALED_S,
		IMSG_ERROR_CANNOT_DELETE_ACTIVE_CHANNEL,
		IMSG_ERROR_EXPECTED_POSITIVE_INTEGER,
		IMSG_ERROR_EXPECTED_COMMAND_LINE,
		IMSG_ERROR_CANNOT_START_PROCESS_S,
		IMSG_ERROR_CANNOT_CREATE_TEMPFILE_S,
		IMSG_ERROR_CANNOT_CREATE_TEMPFILE,
		IMSG_ERROR_CANNOT_OPEN_TEMPFILE_S,
		IMSG_ERROR_EXPECTED_IDENTIFIER_LIST,
		IMSG_ERROR_UNKNOWN_COMMAND_S,
		IMSG_ERROR_NOT_IMPLEMENTED,
		IMSG_ERROR_UNEXPECTED_IDENTIFIER,
		IMSG_ERROR_EXPECTED_MACRO_EXPRESSION,
		// Warnings
		IMSG_WARNING_DIRECTIVE_TAG_MISMATCH = 		f1::c_pp::syntax::IMSG_WARNING_LAST,
		IMSG_WARNING_CANNOT_INVERT_ATTRIBUTE_S,	// cannot invert Iattribute; ignoring
		IMSG_WARNING_ALREADY_USED_S,
		IMSG_WARNING_UNUSED_FALLTHROUGH,
		IMSG_WARNING_PARAM_LIST_DEFINED_WITH_PARENTHESIS,
		IMSG_WARNING_PARAM_LIST_DEFINED_WITHOUT_PARENTHESIS,
		IMSG_WARNING_PARAM_LISTS_DIFFER,
		IMSG_WARNING_EXPECTED_PARAMETER_LIST,
		IMSG_WARNING_NO_CHANNEL_ACTIVE,
		IMSG_WARNING_UNUSED_INCLUDE_ARGUMENTS_M_N,
		// Information messages
		//IMSG_INFO_PREDEFINED = f1::c_pp::syntax::IMSG_INFO_LAST,
		// Debug messages
		//IMSG_DEBUG_USER_N,
	};
	static const f1::msgout::Igroup::message_desc m_msg_message_descs_fe[];
	static const f1::msgout::Igroup::message_desc m_msg_message_descs_e[];
	static const f1::msgout::Igroup::message_desc m_msg_message_descs_w[];
protected:
	f1::msgout::raw_group	m_msgout_message_group_fe;
	f1::msgout::raw_group	m_msgout_message_group_e;
	f1::msgout::raw_group	m_msgout_message_group_w;

	pp::directive_list_t	m_directives;
	pp::macro_list_t		m_macros;
	c_pp::pragma_list_t		m_pragmas;
};

/*END OF syntax.hxx*/
