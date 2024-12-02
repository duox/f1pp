/*else_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::c99::else_directive::else_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::c99::else_directive::get_name() const
{
	return "else";
}
// {26DE2D18-0284-44f9-9BBF-90ED4E9A4C03}
const f1::guid_t f1::c_pp::c99::else_directive::m_guid =
{ 0x26de2d18, 0x284, 0x44f9, { 0x9b, 0xbf, 0x90, 0xed, 0x4e, 0x9a, 0x4c, 0x3 } };
const f1::guid_t & f1::c_pp::c99::else_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::c99::else_directive::get_caps( unsigned mask ) const __noexcept
{
	return cap_invariant & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::c99::else_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Pop the last nesting level
	if_directive::frame * frm = type_cast<if_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
	{
		parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#else" );
		return STATUS_SUCCESS;
	}
	if( frm->m_was_else )
	{
		parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#else" );
		return STATUS_SUCCESS;
	}
	frm->m_was_else = true;

	if( frm->m_allow_parse )
		frm->m_allow_parse = false;
	else if( frm->m_if_executed )
		frm->m_allow_parse = false;
	else
		frm->m_allow_parse = true;

	frm->allow_exec( frm->m_allow_parse );	//frm->update();
	frm->check_different_sources();	// warn if sources differ

	// Exit
	return STATUS_SUCCESS;
}

/*END OF else_directive.cxx*/
