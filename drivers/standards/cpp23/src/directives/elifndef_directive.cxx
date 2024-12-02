/*elifndef_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::cpp23::elifndef_directive::elifndef_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::cpp23::elifndef_directive::get_name() const
{
	return "elifndef";
}
// {D5C4CE48-26D8-4748-838A-03C22247366C}
const f1::guid_t f1::c_pp::cpp23::elifndef_directive::m_guid =
{ 0xd5c4ce48, 0x26d8, 0x4748, { 0x83, 0x8a, 0x03, 0xc2, 0x22, 0x47, 0x36, 0x6c } };
const f1::guid_t & f1::c_pp::cpp23::elifndef_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::cpp23::elifndef_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::cpp23::elifndef_directive::parse( pp::Iparser & parser )
{
	core::Tref<pp::Imacro> mac = nullptr;
	bool res;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	c99::if_directive::frame * frm = type_cast<c99::if_directive::frame *>( parser.get_directive_frame() );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INVALID_STATE;

	// Fetch macro identifier
	c_pp_par->skip_real_spaces_and_comments();

	pp::string ident;
	c_pp_par->parse_macro_identifier( ident );
	if( ident.is_empty() )
	{
		parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		res = false;
	}
	else
	{
		// Find the macro
		parser.find_macro( mac, ident.get_stringslice() );
		res = nullptr == mac;
	}
	if( nullptr != mac )
		parser.on_macro_expanded( *mac );

	// Update context
	frm->m_allow_parse = res;
	frm->allow_exec( res );	//exc->update();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF elifndef_directive.cxx*/
