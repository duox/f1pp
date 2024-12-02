/*goto_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::goto_directive::goto_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::goto_directive::get_name() const
{
	return "goto";
}

// {7B06604F-F6A6-4808-8546-AFAF01A302A6}
const f1::guid_t f1::f1pp::goto_directive::m_guid =
{ 0x7b06604f, 0xf6a6, 0x4808, { 0x85, 0x46, 0xaf, 0xaf, 0x1, 0xa3, 0x2, 0xa6 } };
const f1::guid_t & f1::f1pp::goto_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::goto_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

// Operations
const status_t f1::f1pp::goto_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	f1pp_par->skip_real_spaces_and_comments();

	// Parse label name
	pp::string ident;
	if( f1pp_par->parse_macro_identifier( ident ).failed_status( status ) )
		return status;
	if( ident.empty() )
	{
		parser.message( syntax::IMSG_ERROR_EXPECTED_LABEL_NAME );
		return STATUS_SUCCESS;
	}

	// Find label
	core::Tconst_ref<label> lab;
	if( f1pp_par->find_label( lab, ident ).failed() )
	{
		f1pp_par->wait_for_label( ident );	// this is a forward label, wait for it to come
		return STATUS_SUCCESS;
	}

	// Go to the label location
	if( lab->jump( parser ).failed_status( status ) )
		return status;

	// Exit
	return STATUS_SUCCESS;
}

/*END OF goto_directive.cxx*/
