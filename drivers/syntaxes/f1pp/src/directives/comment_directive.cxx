/*comment_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::comment_directive::comment_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::comment_directive::get_name() const
{
	return "comment";
}

// {90E80F36-7A86-422B-816D-A2D6D309E0D4}
const f1::guid_t f1::f1pp::comment_directive::m_guid =
{ 0x90e80f36, 0x7a86, 0x422b, { 0x81, 0x6d, 0xa2, 0xd6, 0xd3, 0x09, 0xe0, 0xd4 } };
const f1::guid_t & f1::f1pp::comment_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::comment_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

// Operations
const status_t f1::f1pp::comment_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Parse identifier, if it exists
	pp::string ident;
	f1pp_par->skip_real_spaces_and_comments();
	f1pp_par->parse_definition_identifier( ident );

	// Start directive frame
	frame * frm = new(std::nothrow) frame( *f1pp_par );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;
	frm->m_ident.move( ident );
	frm->m_output.initialize();
	frm->m_storage.initialize( *parser.get_context() );
	f1pp_par->modify_c_flags(
		f1::c_pp::parser::c_flags_t::fl_keep_comments |
		f1::c_pp::parser::c_flags_t::fl_execute_exec_top_only |
		f1::c_pp::parser::c_flags_t::fl_do_not_expand_macros,
		f1::c_pp::parser::c_flags_t::null,
		&frm->m_old_c_flags
		);
	parser.set_output( &frm->m_output, &frm->m_old_output );
	parser.set_storage( &frm->m_storage, &frm->m_old_storage );
	parser.enter_directive( *frm );

	// Exit
	return STATUS_SUCCESS;
}
f1::pp::Iframe * f1::f1pp::comment_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return nullptr;
	return new(std::nothrow) frame( *f1pp_par );
}

f1::f1pp::comment_directive::frame::frame( f1pp::parser & par ):
	super( par )
{
}
const char * f1::f1pp::comment_directive::frame::get_openning_directive_name() const
{
	return "comment";
}
const char * f1::f1pp::comment_directive::frame::get_closing_directive_name() const
{
	return "endcomment";
}
const f1::guid_t & f1::f1pp::comment_directive::frame::get_openning_directive_guid() const
{
	return comment_directive::m_guid;
}
const f1::guid_t & f1::f1pp::comment_directive::frame::get_closing_directive_guid() const
{
	return endcomment_directive::m_guid;
}

/*END OF comment_directive.cxx*/
