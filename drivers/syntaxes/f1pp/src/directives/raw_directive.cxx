/*raw_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::raw_directive::raw_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::raw_directive::get_name() const
{
	return "raw";
}

// {36CB5F41-88C4-4f0a-B949-6E69FB10341B}
const f1::guid_t f1::f1pp::raw_directive::m_guid =
{ 0x36cb5f41, 0x88c4, 0x4f0a, { 0xb9, 0x49, 0x6e, 0x69, 0xfb, 0x10, 0x34, 0x1b } };
const f1::guid_t & f1::f1pp::raw_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::raw_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

// Operations
const status_t f1::f1pp::raw_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Start directive frame
	frame * frm = new(std::nothrow) frame( *f1pp_par );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;
	f1pp_par->modify_c_flags(
		f1::c_pp::parser::c_flags_t::fl_keep_comments |
		f1::c_pp::parser::c_flags_t::fl_execute_exec_top_only |
		f1::c_pp::parser::c_flags_t::fl_do_not_expand_macros,
		f1::c_pp::parser::c_flags_t::null,
		&frm->m_old_c_flags
		);
	parser.enter_directive( *frm );

	// Exit
	return STATUS_SUCCESS;
}
f1::pp::Iframe * f1::f1pp::raw_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return nullptr;
	return new(std::nothrow) frame( *f1pp_par );
}

f1::f1pp::raw_directive::frame::frame( f1pp::parser & par ):
	super( par )
{
}
const char * f1::f1pp::raw_directive::frame::get_openning_directive_name() const
{
	return "raw";
}
const char * f1::f1pp::raw_directive::frame::get_closing_directive_name() const
{
	return "endraw";
}
const f1::guid_t & f1::f1pp::raw_directive::frame::get_openning_directive_guid() const
{
	return raw_directive::m_guid;
}
const f1::guid_t & f1::f1pp::raw_directive::frame::get_closing_directive_guid() const
{
	return endraw_directive::m_guid;
}

/*END OF raw_directive.cxx*/
