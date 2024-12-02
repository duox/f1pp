/*switchdef_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::switchdef_directive::switchdef_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::switchdef_directive::get_name() const
{
	return "switchdef";
}
// {AE66AE9C-6C6F-4B27-BBA7-E04F4D85CAE5}
const f1::guid_t f1::f1pp::switchdef_directive::m_guid =
{ 0xae66ae9c, 0x6c6f, 0x4b27, { 0xbb, 0xa7, 0xe0, 0x4f, 0x4d, 0x85, 0xca, 0xe5 } };
const f1::guid_t & f1::f1pp::switchdef_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::switchdef_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::switchdef_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	// Begin directive even if there were some errors
	switchdef_directive::frame * frm = new(std::nothrow) switchdef_directive::frame( *f1pp_par );
	frm->m_switch_type = switch_directive::frame::Etype_switchdef;
	parser.enter_directive( *frm );

	// Exit
	return STATUS_SUCCESS;
}

f1::pp::Iframe * f1::f1pp::switchdef_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return nullptr;
	return new(std::nothrow) frame( *f1pp_par );
}

f1::f1pp::switchdef_directive::frame::frame( f1pp::parser & par ):
	super( par )
{
}
const char * f1::f1pp::switchdef_directive::frame::get_openning_directive_name() const
{
	return "switchdef";
}
const char * f1::f1pp::switchdef_directive::frame::get_closing_directive_name() const
{
	return "endswitchdef";
}
const f1::guid_t & f1::f1pp::switchdef_directive::frame::get_openning_directive_guid() const
{
	return switchdef_directive::m_guid;
}
const f1::guid_t & f1::f1pp::switchdef_directive::frame::get_closing_directive_guid() const
{
	return endswitch_directive::m_guid;
}

/*END OF switchdef_directive.cxx*/
