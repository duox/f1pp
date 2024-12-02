/*switchndef_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::switchndef_directive::switchndef_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::switchndef_directive::get_name() const
{
	return "switchndef";
}
// {4F57CFF2-1B7A-48D0-B738-5896D8705C85}
const f1::guid_t f1::f1pp::switchndef_directive::m_guid =
{ 0x4f57cff2, 0x1b7a, 0x48d0, { 0xb7, 0x38, 0x58, 0x96, 0xd8, 0x70, 0x5c, 0x85 } };
const f1::guid_t & f1::f1pp::switchndef_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::switchndef_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::switchndef_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	// Begin directive even if there were some errors
	switchndef_directive::frame * frm = new(std::nothrow) switchndef_directive::frame( *f1pp_par );
	frm->m_switch_type = switch_directive::frame::Etype_switchndef;
	parser.enter_directive( *frm );

	// Exit
	return STATUS_SUCCESS;
}

f1::pp::Iframe * f1::f1pp::switchndef_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return nullptr;
	return new(std::nothrow) frame( *f1pp_par );
}

f1::f1pp::switchndef_directive::frame::frame( f1pp::parser & par ):
	super( par )
{
}
const char * f1::f1pp::switchndef_directive::frame::get_openning_directive_name() const
{
	return "switchndef";
}
const char * f1::f1pp::switchndef_directive::frame::get_closing_directive_name() const
{
	return "endswitchndef";
}
const f1::guid_t & f1::f1pp::switchndef_directive::frame::get_openning_directive_guid() const
{
	return switchndef_directive::m_guid;
}
const f1::guid_t & f1::f1pp::switchndef_directive::frame::get_closing_directive_guid() const
{
	return endswitch_directive::m_guid;
}

/*END OF switchndef_directive.cxx*/
