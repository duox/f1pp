/*syntax.cxx*/
#include "pch.h"
#include "main.h"

const f1::msgout::Igroup::message_desc f1::f1pp::syntax::m_msg_message_descs_fe[] =
{
	// Fatal errors
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_fatal_error, IMSG_FATALERROR_TOO_MANY_ITERATIONS,	"too many iterations" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_fatal_error, IMSG_FATALERROR_ASSERTION_TRIGGERED,	"ASSERT failed" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_fatal_error, IMSG_FATALERROR_ASSERTION_TRIGGERED_S,	"ASSERT failed: %s" ),
};
// Errors
const f1::msgout::Igroup::message_desc f1::f1pp::syntax::m_msg_message_descs_e[] =
{
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNKNOWN_ATTRIBUTE_S,			"unknown attribute: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNTERMINATED_ATTRIBUTE_LIST,	"unterminated attribute list" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_ATTRIBUTE_NAME,		"expected attribute name" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNEXPECTED_ATTRIBUTES,			"unexpected attributes" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_LABEL_NAME,			"expected label name" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_LABEL_ALREADY_DEFINED_S,		"label has already been defined: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_LABEL_NOT_FOUND_S,				"label is not found: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_POSITIVE_VALUE,		"expected positive value" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_INVALID_S,						"invalid %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_CASE_ALREADY_EXECUTED,			"case has already been executed" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_INVALID_IDENTIFIER,			"invalid identifier" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_SINGLE_IDENTIFIER,	"expected single identifier" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_INVALID_CHARACTER_CODE_X,		"invalid character code: 0x%08x" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_BAD_ARGUMENT,					"invalid argument" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNEXPECTED_DIRECTIVE_END,		"unexpected end of directive" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_CHANNEL_NOT_FOUND_S,			"channel not found: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_CHANNEL_NAME,			"expected channel name" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_CHANNEL_FILENAME,		"expected channel filename" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_CHANNEL_STACK_EMPTY,			"channel stack is empty" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_CHANNEL_ALREADY_DEFINED_S,		"channel already defined: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_INVALID_OPERAND_TYPE,			"invalid expression operand type" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_INVALID_VALUE,					"invalid value" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_INVALID_VALUE_S,				"invalid value: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_TYPE_NAME,			"expected type name" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNKNOWN_TYPE_NAME_S,			"unknown type name: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_INSUFFICIENT_ARGUMENTS,		"insufficient arguments" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_NAME_ALREADY_PRESENT_S,		"name is already present in the current namespace: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_STRING_OR_IDENT,		"expected string or identifier" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_ALREADY_DEFINED_S,				"already defined: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_ALREADY_USED_S,				"already used: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_ENUM_REQUIRES_INITIALIZER,		"member requires initializer" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_ENUM_IS_SEALED_S,				"enum is sealed and cannot be used as base: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_CANNOT_DELETE_ACTIVE_CHANNEL,	"cannot delete active channel" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_POSITIVE_INTEGER,		"expected positive integer" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_COMMAND_LINE,			"expected command line string" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_CANNOT_START_PROCESS_S,		"cannot start process: \"%s\"" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_CANNOT_CREATE_TEMPFILE_S,		"cannot create temporary file: \"%s\"" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_CANNOT_CREATE_TEMPFILE,		"cannot create temporary file" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_CANNOT_OPEN_TEMPFILE_S,		"cannot open temporary file: \"%s\"" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_IDENTIFIER_LIST,		"expected identifier list" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNKNOWN_COMMAND_S,				"unknown command: \"%s\"" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_NOT_IMPLEMENTED,				"not yet implemented" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_UNEXPECTED_IDENTIFIER,			"unexpected identifier" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_error, IMSG_ERROR_EXPECTED_MACRO_EXPRESSION,		"expected macro expression" ),
};
// Warnings
const f1::msgout::Igroup::message_desc f1::f1pp::syntax::m_msg_message_descs_w[] =
{
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_DIRECTIVE_TAG_MISMATCH,	"directive tag mismatch: %s (expected %s)" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_CANNOT_INVERT_ATTRIBUTE_S,	"cannot invert attribute" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_ALREADY_USED_S,			"already used: %s" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_UNUSED_FALLTHROUGH,		"unused #fallthrough" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_PARAM_LIST_DEFINED_WITH_PARENTHESIS,		"parameter list defined with parenthesis" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_PARAM_LIST_DEFINED_WITHOUT_PARENTHESIS,	"parameter list defined without parenthesis" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_PARAM_LISTS_DIFFER,		"parameter lists are different" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_EXPECTED_PARAMETER_LIST,	"expected parameter list" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_NO_CHANNEL_ACTIVE,			"no channel active" ),
	F1_MSGOUT_MESSAGE_SEV_ID_STR( severity_warning, IMSG_WARNING_UNUSED_INCLUDE_ARGUMENTS_M_N,	"unused #include argument(s): %d..%d" ),
};
// Information
// Debug

f1::f1pp::syntax::syntax( const core::Iidentifier * ident ):
	super( ident )
{
}
const status_t f1::f1pp::syntax::initialize()
{
	m_msgout_message_group_fe.initialize();
	m_msgout_message_group_e.initialize();
	m_msgout_message_group_w.initialize();

	m_msgout_message_group_fe.append_messages( m_msg_message_descs_fe, countof(m_msg_message_descs_fe) );
	m_msgout_message_group_e.append_messages( m_msg_message_descs_e, countof(m_msg_message_descs_e) );
	m_msgout_message_group_w.append_messages( m_msg_message_descs_w, countof(m_msg_message_descs_w) );

	m_directives.push_back( new(std::nothrow) abort_directive );
	m_directives.push_back( new(std::nothrow) alias_directive );
	m_directives.push_back( new(std::nothrow) assert_directive );
	m_directives.push_back( new(std::nothrow) assignment_directive );
	m_directives.push_back( new(std::nothrow) block_directive );
	m_directives.push_back( new(std::nothrow) break_directive );
	m_directives.push_back( new(std::nothrow) case_directive );
	m_directives.push_back( new(std::nothrow) channel_directive );
	m_directives.push_back( new(std::nothrow) comment_directive );
	m_directives.push_back( new(std::nothrow) concat_directive );
	m_directives.push_back( new(std::nothrow) continue_directive );
	m_directives.push_back( new(std::nothrow) debugout_directive );
	m_directives.push_back( new(std::nothrow) default_directive );
	m_directives.push_back( new(std::nothrow) define_directive );
	m_directives.push_back( new(std::nothrow) do_directive );
	m_directives.push_back( new(std::nothrow) dump_directive );
	m_directives.push_back( new(std::nothrow) echo_directive );
	m_directives.push_back( new(std::nothrow) elifdef_directive );
	m_directives.push_back( new(std::nothrow) elifndef_directive );
	m_directives.push_back( new(std::nothrow) embed_directive );
	m_directives.push_back( new(std::nothrow) end_directive );
	m_directives.push_back( new(std::nothrow) endblock_directive );
	m_directives.push_back( new(std::nothrow) endcomment_directive );
	m_directives.push_back( new(std::nothrow) endenum_directive );
	m_directives.push_back( new(std::nothrow) endexec_directive );
	m_directives.push_back( new(std::nothrow) endfor_directive );
	m_directives.push_back( new(std::nothrow) endforeach_directive );
	m_directives.push_back( new(std::nothrow) endfunction_directive );
	m_directives.push_back( new(std::nothrow) endmacro_directive );
	m_directives.push_back( new(std::nothrow) endnamespace_directive );
	m_directives.push_back( new(std::nothrow) endregion_directive );
	m_directives.push_back( new(std::nothrow) endraw_directive );
	m_directives.push_back( new(std::nothrow) endrepeat_directive );
	m_directives.push_back( new(std::nothrow) endshell_directive );
	m_directives.push_back( new(std::nothrow) endswitch_directive );
	m_directives.push_back( new(std::nothrow) endswitchdef_directive );
	m_directives.push_back( new(std::nothrow) endswitchndef_directive );
	m_directives.push_back( new(std::nothrow) endwhile_directive );
	m_directives.push_back( new(std::nothrow) enum_directive );
	m_directives.push_back( new(std::nothrow) env_directive );
	m_directives.push_back( new(std::nothrow) exec_directive );
	m_directives.push_back( new(std::nothrow) exit_directive );
	m_directives.push_back( new(std::nothrow) export_directive );
	m_directives.push_back( new(std::nothrow) fallthrough_directive );
	m_directives.push_back( new(std::nothrow) fatalerror_directive );
	m_directives.push_back( new(std::nothrow) for_directive );
	m_directives.push_back( new(std::nothrow) foreach_directive );
	m_directives.push_back( new(std::nothrow) format_directive );
	m_directives.push_back( new(std::nothrow) function_directive );
	m_directives.push_back( new(std::nothrow) goto_directive );
	m_directives.push_back( new(std::nothrow) ifdef_directive );
	m_directives.push_back( new(std::nothrow) ifndef_directive );
	m_directives.push_back( new(std::nothrow) import_directive );
	m_directives.push_back( new(std::nothrow) include_directive );	// TODO: fix memory corruption in dbg
	m_directives.push_back( new(std::nothrow) includeonce_directive );
	m_directives.push_back( new(std::nothrow) local_directive );
	m_directives.push_back( new(std::nothrow) macro_directive );
	m_directives.push_back( new(std::nothrow) message_directive );
	m_directives.push_back( new(std::nothrow) namespace_directive );
	m_directives.push_back( new(std::nothrow) option_directive );
	m_directives.push_back( new(std::nothrow) print_directive );
	m_directives.push_back( new(std::nothrow) param_directive );
	m_directives.push_back( new(std::nothrow) raw_directive );
	m_directives.push_back( new(std::nothrow) redefine_directive );
	m_directives.push_back( new(std::nothrow) region_directive );
	m_directives.push_back( new(std::nothrow) repeat_directive );
	m_directives.push_back( new(std::nothrow) return_directive );
	m_directives.push_back( new(std::nothrow) shell_directive );
	m_directives.push_back( new(std::nothrow) set_directive );
	m_directives.push_back( new(std::nothrow) status_directive );
	m_directives.push_back( new(std::nothrow) switch_directive );
	m_directives.push_back( new(std::nothrow) switchdef_directive );
	m_directives.push_back( new(std::nothrow) switchndef_directive );
	m_directives.push_back( new(std::nothrow) tryinclude_directive );
	m_directives.push_back( new(std::nothrow) until_directive );
	m_directives.push_back( new(std::nothrow) undef_directive );
	m_directives.push_back( new(std::nothrow) using_directive );
	m_directives.push_back( new(std::nothrow) while_directive );
	array_foreach( pp::directive_list_t::iterator, it, m_directives )
		it->initialize();

	m_macros.append( new(std::nothrow) macro_countof );
	m_macros.append( new(std::nothrow) macro_deprecated );
	m_macros.append( new(std::nothrow) macro_deprecated_directive );
	m_macros.append( new(std::nothrow) macro_deprecated_include );
	m_macros.append( new(std::nothrow) macro_Embed );
	m_macros.append( new(std::nothrow) macro_empty );
	m_macros.append( new(std::nothrow) macro_env );
	m_macros.append( new(std::nothrow) macro_GLOBAL_LINE );
	m_macros.append( new(std::nothrow) macro_has_directive );
	m_macros.append( new(std::nothrow) macro_has_include );
	m_macros.append( new(std::nothrow) macro_has_option );
	m_macros.append( new(std::nothrow) macro_option );
	//m_macros.append( new(std::nothrow) macro_REPEAT_COUNT );
	array_foreach( pp::macro_list_t::iterator, it, m_macros )
		it->initialize();

	m_pragmas.append( new(std::nothrow) pragma_debugout );
	m_pragmas.append( new(std::nothrow) pragma_error );
	m_pragmas.append( new(std::nothrow) pragma_fatalerror );
	m_pragmas.append( new(std::nothrow) pragma_includedir );
	m_pragmas.append( new(std::nothrow) pragma_info );
	m_pragmas.append( new(std::nothrow) pragma_option );
	m_pragmas.append( new(std::nothrow) pragma_status );
	m_pragmas.append( new(std::nothrow) pragma_warning );
	array_foreach( c_pp::pragma_list_t::iterator, it, m_pragmas )
		it->initialize();

	return super::initialize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// {D1C1061D-57DB-4ac3-A1F1-4A3A18E9D348}
const f1::guid_t f1::f1pp::syntax::m_guid =
{ 0xd1c1061d, 0x57db, 0x4ac3, { 0xa1, 0xf1, 0x4a, 0x3a, 0x18, 0xe9, 0xd3, 0x48 } };
const f1::guid_t & f1::f1pp::syntax::get_guid() const
{
	return m_guid;
}

const char * f1::f1pp::syntax::get_ident_string() const
{
	return "f1pp";
}
const char * f1::f1pp::syntax::get_description_string() const
{
	return "F1PP preprocessor";
}

static const f1::guid_t base_guids[] =
{
//	f1::c_pp::syntax::m_guid,
	f1::c_pp::clang::standard::m_guid,
	f1::c_pp::gcc::standard::m_guid,
	f1::c_pp::msvc::standard::m_guid,
};
const f1::guid_t * f1::f1pp::syntax::get_base_guid_list( __out size_t & count ) const
{
	count = _countof(base_guids);
	return base_guids;
}
const f1::guid_t * f1::f1pp::syntax::get_preferred_standard_list( __out size_t & count ) const
{
	count = _countof(base_guids);
	return base_guids;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::f1pp::syntax::attach_to_context( pp::context & frm )
{
	__unreferenced_parameter( frm );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::syntax::detach_from_context( pp::context & frm )
{
	__unreferenced_parameter( frm );
	return STATUS_SUCCESS;
}

const status_t f1::f1pp::syntax::on_attach_to_parser( pp::Iparser & par )
{
	status_t status;

	// Call base class
	if( super::on_attach_to_parser( par ).failed_status( status ) )
		return status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&par);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	par.get_msgout_context().append_group( m_msgout_message_group_fe );
	par.get_msgout_context().append_group( m_msgout_message_group_e );
	par.get_msgout_context().append_group( m_msgout_message_group_w );

	// Add all items
	c_pp_par->get_directive_list()->append_directives( m_directives );
	c_pp_par->append_macros( m_macros );
	c_pp_par->append_pragmas( m_pragmas );

	// Set parser parameters
	//c_pp_par->set_limit_value_min( pp::limit_t::li_maximum_block_depth, 63 );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::syntax::on_detach_from_parser( pp::Iparser & par )
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&par);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	par.get_msgout_context().remove_group( m_msgout_message_group_fe );
	par.get_msgout_context().remove_group( m_msgout_message_group_e );
	par.get_msgout_context().remove_group( m_msgout_message_group_w );

	// Remove all items
	c_pp_par->get_directive_list()->remove_directives( m_directives );
	c_pp_par->remove_macros( m_macros );
	c_pp_par->remove_pragmas( m_pragmas );

	// Exit
	return super::on_detach_from_parser( par );
}

bool f1::f1pp::syntax::is_item_supported( const f1::guid_t & item_class_guid, const pp::string & name ) const
{
	if( pp::Imacro::m_item_class_guid == item_class_guid )
	{
		array_foreach( pp::macro_list_t::const_iterator, it, m_macros )
		{
			if( name.equal( it->get_name() ) )
				return true;
		}
	}
	else if( pp::Idirective::m_item_class_guid == item_class_guid )
	{
		array_foreach( pp::directive_list_t::const_iterator, it, m_directives )
		{
			if( name.equal( it->get_name() ) )
				return true;
		}
	}
	else if( c_pp::Ipragma::m_item_class_guid == item_class_guid )
	{
		array_foreach( c_pp::pragma_list_t::const_iterator, it, m_pragmas )
		{
			if( name.equal( it->get_name() ) )
				return true;
		}
	}
	return super::is_item_supported( item_class_guid, name );
}

const status_t f1::f1pp::syntax::new_parser( f1::core::Tref<pp::Iparser> & res, const core::Iidentifier * ident )
{
	core::Tref<f1pp::parser> par;
	par.createT<f1pp::parser>( ident );

//	par->get_msgout_context().append_group( m_msgout_message_group );

	res = par;
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::syntax::new_macro( f1::core::Tref<pp::Iuser_macro> & res, const core::Iidentifier * ident )
{
	return res.createT<f1pp::macro>( ident );
}

const status_t f1::f1pp::syntax::process_command_line_switch( int switchar, const core::static_string & name, const core::static_string & param )
{
	//TODO
	return super::process_command_line_switch( switchar, name, param );
}

/*END OF syntax.cxx*/
