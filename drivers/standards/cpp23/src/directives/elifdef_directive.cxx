/*elifdef_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::cpp23::elifdef_directive::elifdef_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::cpp23::elifdef_directive::get_name() const
{
	return "elifdef";
}
// {DD9DCBD5-201B-446c-AECC-23896E7CFD76}
const f1::guid_t f1::c_pp::cpp23::elifdef_directive::m_guid =
{ 0xdd9dcbd5, 0x201b, 0x446c, { 0xae, 0xcc, 0x23, 0x89, 0x6e, 0x7c, 0xfd, 0x76 } };
const f1::guid_t & f1::c_pp::cpp23::elifdef_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::cpp23::elifdef_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::cpp23::elifdef_directive::parse( pp::Iparser & parser )
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
		res = nullptr != mac;
		if( nullptr != mac )
			parser.on_macro_expanded( *mac );
	}

	// Update context
	frm->m_allow_parse = res;
	frm->allow_exec( res );	//exc->update();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF elifdef_directive.cxx*/
