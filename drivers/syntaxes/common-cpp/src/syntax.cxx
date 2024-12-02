/*syntax.cxx*/
#include "pch.h"
#include "main.h"

const f1::msgout::group::message_desc f1::c_pp::syntax::m_msg_fatal_error_descs[] =
{
	{	f1::msgout::severity_fatal_error, 0, 0, IMSG_FATAL_ERROR_USER_N,			STATUS_SUCCESS,	IMSG_FATAL_ERROR_USER_N },
};
const f1::msgout::group::message_desc f1::c_pp::syntax::m_msg_error_descs[] =
{
	{	f1::msgout::severity_error, 0, 0, IMSG_ERROR_USER_N,					STATUS_SUCCESS,	IMSG_ERROR_USER_N },
};
const f1::msgout::group::message_desc f1::c_pp::syntax::m_msg_warning_descs[] =
{
	{	f1::msgout::severity_warning, 0, 0, IMSG_WARNING_DIFFERENT_SOURCES,				STATUS_SUCCESS, IMSG_WARNING_DIFFERENT_SOURCES },
};
const f1::msgout::group::message_desc f1::c_pp::syntax::m_msg_info_descs[] =
{
};
const f1::msgout::group::message_desc f1::c_pp::syntax::m_msg_message_descs[] =
{
	{	f1::msgout::severity_none, 0, 0, IMSG_RAW,			STATUS_SUCCESS,	IMSG_RAW	},
	{	f1::msgout::severity_none, 0, 0, IMSG_RAW_N,		STATUS_SUCCESS,	IMSG_RAW_N	},
	{	f1::msgout::severity_none, 0, 0, IMSG_RAW_INDENT,	STATUS_SUCCESS,	IMSG_RAW_INDENT	},
	{	f1::msgout::severity_none, 0, 0, IMSG_RAW_INDENT_N,	STATUS_SUCCESS,	IMSG_RAW_INDENT_N	},
};

static const f1::msgout::source_memory::static_string_desc_t	s_strings[] =
{
	// fatal errors
	{ f1::c_pp::syntax::IMSG_FATAL_ERROR_USER_N,				"%.*s" },
	// errors
	{ f1::c_pp::syntax::IMSG_ERROR_USER_N,						"%.*s" },
	// warnings
	// information
	// raw
	{ f1::c_pp::syntax::IMSG_RAW,			"%s" },
	{ f1::c_pp::syntax::IMSG_RAW_N,			"%.*s" },
	{ f1::c_pp::syntax::IMSG_RAW_INDENT,	"%*s" },
	{ f1::c_pp::syntax::IMSG_RAW_INDENT_N,	"%*.*s" },
};

f1::c_pp::syntax::syntax( const core::Iidentifier * ident ):
	super( ident )
{
	m_msgout_fatal_error_group.append_messages( m_msg_fatal_error_descs, countof(m_msg_fatal_error_descs) );
	m_msgout_error_group.append_messages( m_msg_error_descs, countof(m_msg_error_descs) );
	m_msgout_warning_group.append_messages( m_msg_warning_descs, countof(m_msg_warning_descs) );
	m_msgout_info_group.append_messages ( m_msg_info_descs,  countof(m_msg_info_descs) );
	m_msgout_message_group.append_messages ( m_msg_message_descs,  countof(m_msg_message_descs) );

	m_msgout_string_source.append_static_strings( s_strings, countof(s_strings) );

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// {24E076BA-3E59-4e8c-9D1C-5870ADE75531}
const f1::guid_t f1::c_pp::syntax::m_guid =
{ 0x24e076ba, 0x3e59, 0x4e8c, { 0x9d, 0x1c, 0x58, 0x70, 0xad, 0xe7, 0x55, 0x31 } };
const f1::guid_t & f1::c_pp::syntax::get_guid() const
{
	return m_guid;
}

const char * f1::c_pp::syntax::get_ident_string() const
{
	return "c_pp_common";
}
const char * f1::c_pp::syntax::get_description_string() const
{
	return "C preprocessor common extensions";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::c_pp::syntax::attach_to_context( pp::context & ctx )
{
//	ctx.append_function( m_defined_function );

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::syntax::detach_from_context( pp::context & ctx )
{
//	ctx.remove_function( m_defined_function );

	return STATUS_SUCCESS;
}

const status_t f1::c_pp::syntax::new_parser( f1::core::Tref<pp::Iparser> & res, const core::Iidentifier * ident )
{
	core::Tref<c_pp::parser> par;
	par.createT<c_pp::parser>( ident );

	core::Tref<directive_list> dlist = par->get_directive_list();
	dlist->append_directive( *new(std::nothrow) define_directive );
	dlist->append_directive( *new(std::nothrow) elif_directive );
	dlist->append_directive( *new(std::nothrow) else_directive );
	dlist->append_directive( *new(std::nothrow) endif_directive );
	dlist->append_directive( *new(std::nothrow) error_directive );
	dlist->append_directive( *new(std::nothrow) if_directive );
	dlist->append_directive( *new(std::nothrow) ifdef_directive );
	dlist->append_directive( *new(std::nothrow) ifndef_directive );
	dlist->append_directive( *new(std::nothrow) include_directive );
	dlist->append_directive( *new(std::nothrow) line_directive );
	dlist->append_directive( *new(std::nothrow) pragma_directive );
	dlist->append_directive( *new(std::nothrow) undef_directive );

	par->get_msgout_context().append_group( m_msgout_fatal_error_group );
	par->get_msgout_context().append_group( m_msgout_error_group );
	par->get_msgout_context().append_group( m_msgout_warning_group );
	par->get_msgout_context().append_group( m_msgout_info_group );
	par->get_msgout_context().append_group( m_msgout_message_group );

	par->get_msgout_context().append_source( m_msgout_string_source );

	res = par;
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::syntax::new_macro( f1::core::Tref<pp::Imacro> & res, ns::name * nm )
{
	return res.createT<c_pp::macro>( nm );
}

/*const f1::c_pp::directive_list * f1::c_pp::syntax::get_directive_list() const
{
	return &m_directive_list;
}
f1::c_pp::directive_list * f1::c_pp::syntax::get_directive_list()
{
	return &m_directive_list;
}*/

/*END OF syntax.cxx*/
