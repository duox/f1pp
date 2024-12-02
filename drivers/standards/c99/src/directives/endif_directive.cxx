/*endif_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::c99::endif_directive::endif_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::c99::endif_directive::get_name() const
{
	return "endif";
}
// {CF80A47F-CDFF-4324-B665-E1FDFDC5C63C}
const f1::guid_t f1::c_pp::c99::endif_directive::m_guid =
{ 0xcf80a47f, 0xcdff, 0x4324, { 0xb6, 0x65, 0xe1, 0xfd, 0xfd, 0xc5, 0xc6, 0x3c } };
const f1::guid_t & f1::c_pp::c99::endif_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::c99::endif_directive::get_caps( unsigned mask ) const __noexcept
{
	return cap_invariant & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::c99::endif_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Check end of directive is reached
/*	c_pp_par->skip_real_spaces_and_comments();
	pp::string value;
	if( c_pp_par->parse_to_newline( value ).failed_status( status ) )
		return status;
	if( !value.is_empty() )
	{
		parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "#endif" );
		return STATUS_SUCCESS;
	}
*/
	// Get access to the most nested directive execution context
	if_directive::frame * frm = type_cast<if_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endif" );
	if( frm->get_closing_directive_guid() != endif_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endif" );

	frm->check_different_sources();	// warn if sources differ

	// Pop the last nesting level
	parser.leave_directive();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endif_directive.cxx*/
