/*syntax.cxx*/
#include "pch.h"
#include "main.h"

// disabled by default flag
#define DD	(f1::msgout::Igroup::message_flags_t::fl_disabled | f1::msgout::Igroup::message_flags_t::fl_default_disabled | f1::msgout::Igroup::message_flags_t::fl_can_disable)

// Fatal errors
const f1::msgout::Igroup::message_desc f1::c_pp::syntax::m_msg_message_descs_fe[] =
{
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_fatal_error, IMSG_FATAL_ERROR_USER_N,				"%.*s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_fatal_error, IMSG_FATAL_ERROR_INVALID_FILE_NAME,		"invalid file name" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_fatal_error, IMSG_FATAL_ERROR_FILE_NOT_FOUND,		"file not found: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_fatal_error, IMSG_FATAL_ERROR_CANNOT_OPEN_FILE,		"cannot open file: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_fatal_error, IMSG_FATAL_ERROR_INVALID_PARAMETER,		"invalid parameter" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_fatal_error, IMSG_FATAL_ERROR_INVALID_PARAMETER_S,	"invalid parameter %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_fatal_error, IMSG_FATAL_ERROR_LAST,					"" ),
};
// Errors
const f1::msgout::Igroup::message_desc f1::c_pp::syntax::m_msg_message_descs_e[] =
{
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_USER_N,							"%.*s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_DIRECTIVE_NAME,			"expected directive name" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNKNOWN_DIRECTIVE,					"unknown directive: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNKNOWN_FUNCTION,					"unknown function: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_S,						"expected %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_S_AFTER_S,				"expected %s after %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_S_OR_S,					"expected %s or %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_IDENTIFIER,				"expected identifier" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_INTEGER,					"expected integer" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_STRING,					"expected string" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_OPERATOR,					"expected operator" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_EXPRESSION,				"expected expression" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_INCLUDE_FILENAME,			"expected double-quoted or angle-bracketed file name" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_FILENAME,					"expected double-quoted file name" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_ALIAS_NAME,				"expected alias name" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_TARGET_NAME,				"expected target name" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_ARG_LIST,					"expected argument list" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_OPERAND,					"expected expression operand" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_PARAM_LIST,				"expected parameter list" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_PARAMETER_S,				"expected parameter, found '%s'" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_VALUE,					"expected value" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNEXPECTED_S,						"unexpected %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNEXPECTED_DIRECTIVE,				"unexpected directive" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNEXPECTED_EOF,					"unexpected end of file" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNEXPECTED_EOL,					"unexpected end of line" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNEXPECTED_MACRO_END,				"invalid macro argument list" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNTERMINATED_COMMENT,				"unterminated comment" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNTERMINATED_STRING,				"unterminated string" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNTERMINATED_DIRECTIVE,			"unterminated directive: \"#%s\"" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNTERMINATED_MACRO_CALL,			"unterminated macro call: \"%s\"" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_TOO_LONG_CHAR_CONSTANT,			"character constant is too long" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_INVALID_INTEGER_OPERAND,			"invalid integer operand" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_INVALID_ESCAPE_SEQUENCE,			"invalid escape sequence" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_INVALID_CHAR_CONSTANT,				"invalid character constant" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_DIVISION_BY_ZERO,					"division by zero" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_OPERATOR_IS_NOT_SUPPORTED,			"operator is not supported" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_OPERATOR_EXPECTS_MACRO_ARG,		"operator expects macro argument" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_MACRO_ALREADY_DEFINED_S,			"macro is already defined: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_MACRO_IS_IMMUTABLE,				"macro is immutable" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_INSUFFICIENT_MACRO_ARGUMENTS,		"insufficient number of macro arguments" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_CANNOT_DEFINE_MACRO_S,				"redefining predefined macro %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_NEWLINE_IN_STRING,					"newline in constant" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_INVALID_USE_OF,					"invalid use of %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_NAME_IS_ALREADY_OCCUPIED_S,		"name is already occupied: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_NOT_FOUND_S,						"not found: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNSUPPORTED_S,						"unsupported: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_ARG_LIST_PARAM_LIST_MISMATCH_S,	"%s: argument list doesn't match parameter list" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_AMBIGUOUS_MACRO_CALL_S,			"ambiguous macro call: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_ARG_LIST_S,				"expected argument list: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_LAST,								"" ),
};
// Warnings
const f1::msgout::Igroup::message_desc f1::c_pp::syntax::m_msg_message_descs_w[] =
{
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_USER_N,						"%.*s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_EXPECTED_S,					"expected %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_UNEXPECTED_S,					"unexpected %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_UNEXPECTED_CHARACTERS,			"unexpected characters" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER,	"unexpected characters after %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_UNEXPECTED_DIRECTIVE,			"unexpected directive" ),
	F1_MSGOUT_MESSAGE_SEV_FLS_ID_STR( severity_warning, DD, IMSG_WARNING_UNKNOWN_PRAGMA,				"unknown pragma" ),
	F1_MSGOUT_MESSAGE_SEV_FLS_ID_STR( severity_warning, DD, IMSG_WARNING_UNDEFINED_IDENTIFIER_IN_EXPR,	"undefined identifier, treated as 0: \"%s\"" ),
	F1_MSGOUT_MESSAGE_SEV_FLS_ID_STR( severity_warning, DD, IMSG_WARNING_UNDEFINED_IDENTIFIER_IN_UNDEF,	"undefined identifier, ignored: \"%s\"" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_NEGATIVE_SHIFT_AMOUNT,			"shift amount is negative" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_EXCESSIVE_INTEGER_SUFFIX,		"excessive integer suffixes" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_EXCESSIVE_CHARACTER_PREFIX,	"excessive character suffixes" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_EXCESSIVE_STRING_PREFIX,		"excessive string suffixes" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_UNARY_MINUS_APPLIED_TO_UINT,	"unary minus applied to unsigned; result is still unsigned" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_UNARY_PLUS_APPLIED_TO_UINT,	"unary plus applied to unsigned; result is still unsigned" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_MIXED_SIGNEDNESS,				"mixed signed and unsigned operands; result is unsigned" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_DEPRECATED_DIRECTIVE_S,		"deprecated directive: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_DEPRECATED_MACRO_S,			"deprecated macro: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_DEPRECATED_MACRO_S_S,			"deprecated macro %s: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_DEPRECATED_INCLUDE_S,			"deprecated include: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_DEPRECATED_OPERATOR_S,			"deprecated operator: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_EMPTY_MACRO_STACK,				"definition stack for specified macro is empty" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_TOO_MANY_ARGUMENTS_S,			"%s: too many arguments, ignoring extra" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_DIFFERENT_SOURCES,				"directive clauses are located in different sources" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_UNMATCHED_QUOTES,				"unmatched quote types; using the first one" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_INVALID_DIGRAPH_CC,			"invalid digraph %c%c" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_INVALID_TRIGRAPH_C,			"invalid trigraph ?""?""%c" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_RECURSIVE_MACRO_CALL_S,		"recursive macro call: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_NESTED_COMMENT,				"nested comment" ),
	F1_MSGOUT_MESSAGE_SEV_FLS_ID_STR( severity_warning, DD, IMSG_WARNING_REDUNANT_DEFINE,				"macro is already defined with the same value" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_POSSIBLY_DETACHED_PARAM_LIST,	"possibly detached parameter list" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_DIFFERENT_INCLUDE_TYPES,		"different include types in multi-string include" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_CONSTANT_OVERFLOW,				"constant value overflow" ),
	F1_MSGOUT_MESSAGE_SEV_FLS_ID_STR( severity_warning, DD, IMSG_WARNING_DIGRAPHS_FOUND,				"digraphs found in the source code" ),
	F1_MSGOUT_MESSAGE_SEV_FLS_ID_STR( severity_warning, DD, IMSG_WARNING_TRIGRAPHS_FOUND,				"trigraphs found in the source code" ),
	F1_MSGOUT_MESSAGE_SEV_FLS_ID_STR( severity_warning, DD, IMSG_WARNING_UCN_FOUND,						"UCNs found in the source code" ),
	F1_MSGOUT_MESSAGE_SEV_FLS_ID_STR( severity_warning, DD, IMSG_WARNING_MACRO_EXP_NON_REPRODUCIBLE,	"time/date macro use makes compiled code non-reproducible" ),
	F1_MSGOUT_MESSAGE_SEV_FLS_ID_STR( severity_warning, DD, IMSG_WARNING_DEFINE_WHITESPACE_REQUIRED,	"whitespace required between definition header and macro value" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_DIRECTIVE_NOT_AVAILABLE,		"directive is not available in the current mode: %" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_NOT_AVAILABLE_STANDARD_SS,		"not available: %s (requires %s standard)" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_OPERATOR_NOT_AVAILABLE,		"operator is not available in the current mode: %" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_MAXIMUM_NESTING_LEVEL_REACHED,	"include maximum nesting level is reached" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_INCLUDE_PATH_DIGIT_STARTED,	"include path name should not start with a digit" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_NONSTD_CHAR_IN_IDENTIFIER,		"non-standard character used in identifier: %c" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_OUTSIDE_OF_COMMENT,			"'*/' outside of comment" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_CHARACTER_CONSTANT_OVERFLOW,	"character constant overflow" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_TOO_LONG_CHARACTER_CONSTANT,	"too long character constant" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_TOO_LONG_IDENTIFIER,			"too long identifier" ),
	F1_MSGOUT_MESSAGE_SEV_FLS_ID_STR( severity_warning, DD, IMSG_WARNING_UNDEFINED_MACRO_IN_EXPRESSION,	"undefined macro in expression: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_SEARCH_PATH_NOT_FOUND,			"search path not found: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_MIXED_ENCODING,				"mixed encoding of literals" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_INVALID_RAW_STRING,			"invalid raw string" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_MACRO_IS_NEVER_USED_S,			"macro is never used: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_OPERATION_ORDER_UNDEFINED_S,	"operator order is undefined at: %s" ),
	F1_MSGOUT_MESSAGE_SEV_FLS_ID_STR( severity_warning, DD, IMSG_WARNING_OCTAL_CONSTANT,				"octal constant in deprecated format" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_EMPTY_PATHNAME,				"empty path name" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_DIGIT_NOT_PART_OF_NUMERIC_C,	"the digit is not part of numeric sequence: %c" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_RAW_STRING_TERM_IN_MIDDLE,		"unexpected raw string terminator in the middle of string" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_USER,							"%s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_EXPECTED_IDENTIFIER,			"expected identifier" ),	// copy of IMSG_ERROR_EXPECTED_IDENTIFIER, but non-fatal
	F1_MSGOUT_MESSAGE_SEV_FLS_ID_STR( severity_warning, DD, IMSG_WARNING_UNKNOWN_PRAGMA_S,				"unknown pragma: %s" ),		// TODO: move up to IMSG_WARNING_UNKNOWN_PRAGMA
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_LAST,							"" ),
};
// Information
const f1::msgout::Igroup::message_desc f1::c_pp::syntax::m_msg_message_descs_i[] =
{
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_informational, 	IMSG_INFO_USER_N,					"%.*s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_informational, 	IMSG_INFO_PREDEFINED,				"previously defined by standard" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_informational, 	IMSG_INFO_DEFINED_IN_COMMAND_LINE,	"previously defined in command line" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_informational, 	IMSG_INFO_DEFINED_HERE,				"previously defined here" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_informational, 	IMSG_INFO_STARTED_HERE,				"started here" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_informational, 	IMSG_INFO_USED_HERE,				"used here" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_informational,	IMSG_INFO_POSSIBLE_DEFINITION,		"possible definition" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_informational,	IMSG_INFO_LOCATED_HERE,				"located here" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_informational,	IMSG_INFO_LAST,						"" ),
};
// Debug
const f1::msgout::Igroup::message_desc f1::c_pp::syntax::m_msg_message_descs_d[] =
{
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_debug,			IMSG_DEBUG_USER_N,					"%.*s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_debug,			IMSG_DEBUG_LAST,					"" ),
};
// Just raw messages
const f1::msgout::Igroup::message_desc f1::c_pp::syntax::m_msg_message_descs_r[] =
{
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_none, 			IMSG_RAW,							"%s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_none, 			IMSG_RAW_N,							"%.*s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_none, 			IMSG_RAW_INDENT,					"%*s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_none, 			IMSG_RAW_INDENT_N,					"%*.*s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_none, 			IMSG_RAW_LAST,						"" ),
};

f1::c_pp::syntax::syntax( const core::Iidentifier * ident ):
	super( ident )
{
}
const status_t f1::c_pp::syntax::initialize()
{
	m_msgout_message_group_fe.initialize();
	m_msgout_message_group_e.initialize();
	m_msgout_message_group_w.initialize();
	m_msgout_message_group_i.initialize();
	m_msgout_message_group_d.initialize();
	m_msgout_message_group_r.initialize();

	m_msgout_message_group_fe.append_messages( m_msg_message_descs_fe, countof(m_msg_message_descs_fe) );
	m_msgout_message_group_e.append_messages ( m_msg_message_descs_e,  countof(m_msg_message_descs_e) );
	m_msgout_message_group_w.append_messages ( m_msg_message_descs_w,  countof(m_msg_message_descs_w) );
	m_msgout_message_group_i.append_messages ( m_msg_message_descs_i,  countof(m_msg_message_descs_i) );
	m_msgout_message_group_d.append_messages ( m_msg_message_descs_d,  countof(m_msg_message_descs_d) );
	m_msgout_message_group_r.append_messages ( m_msg_message_descs_r,  countof(m_msg_message_descs_r) );

	return super::initialize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// {9FFDE653-2B10-4e21-98E5-30B83FA98953}
const f1::guid_t f1::c_pp::syntax::m_guid =
{ 0x9ffde653, 0x2b10, 0x4e21, { 0x98, 0xe5, 0x30, 0xb8, 0x3f, 0xa9, 0x89, 0x53 } };
const f1::guid_t & f1::c_pp::syntax::get_guid() const
{
	return m_guid;
}

const char * f1::c_pp::syntax::get_ident_string() const
{
	return "c_pp";
}
const char * f1::c_pp::syntax::get_description_string() const
{
	return "C preprocessor";
}

/*static const f1::guid_t base_guids[] =
{
	f1::c_pp::clang::standard::m_guid,
	f1::c_pp::gcc::standard::m_guid,
	f1::c_pp::msvc::standard::m_guid,
};*/
const f1::guid_t * f1::c_pp::syntax::get_base_guid_list( __out size_t & count ) const
{
	count = 0;//_countof(base_guids);
	return nullptr;//base_guids;
}
const f1::guid_t * f1::c_pp::syntax::get_preferred_standard_list( __out size_t & count ) const
{
	count = 0;//_countof(base_guids);
	return nullptr;//base_guids;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::syntax::attach_to_context( pp::context & ctx )
{
	__unreferenced_parameter( ctx );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::syntax::detach_from_context( pp::context & ctx )
{
	__unreferenced_parameter( ctx );
	return STATUS_SUCCESS;
}

const status_t f1::c_pp::syntax::on_attach_to_parser( pp::Iparser & par )
{
	status_t status;

	// Call base class
	if( super::on_attach_to_parser( par ).failed_status( status ) )
		return status;

	// Append message groups
	par.get_msgout_context().append_group( m_msgout_message_group_fe );
	par.get_msgout_context().append_group( m_msgout_message_group_e );
	par.get_msgout_context().append_group( m_msgout_message_group_w );
	par.get_msgout_context().append_group( m_msgout_message_group_i );
	par.get_msgout_context().append_group( m_msgout_message_group_d );
	par.get_msgout_context().append_group( m_msgout_message_group_r );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::syntax::on_detach_from_parser( pp::Iparser & par )
{
	// Remove message groups
	par.get_msgout_context().remove_group( m_msgout_message_group_fe );
	par.get_msgout_context().remove_group( m_msgout_message_group_e );
	par.get_msgout_context().remove_group( m_msgout_message_group_w );
	par.get_msgout_context().remove_group( m_msgout_message_group_i );
	par.get_msgout_context().remove_group( m_msgout_message_group_d );
	par.get_msgout_context().remove_group( m_msgout_message_group_r );

	// Exit
	return super::on_detach_from_parser( par );
}

const status_t f1::c_pp::syntax::new_parser( f1::core::Tref<pp::Iparser> & res, const core::Iidentifier * ident )
{
	core::Tref<c_pp::parser> par;
	par.createT<c_pp::parser>( ident );

	res = par;
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::syntax::new_macro( f1::core::Tref<pp::Iuser_macro> & res, const core::Iidentifier * ident )
{
	return res.createT<c_pp::macro>( ident );
}

/*END OF syntax.cxx*/
