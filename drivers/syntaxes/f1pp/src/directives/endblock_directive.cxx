/*endblock_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endblock_directive::endblock_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endblock_directive::get_name() const
{
	return "endblock";
}
// {6FBC7AEF-1114-4312-92FB-03AABA815216}
const f1::guid_t f1::f1pp::endblock_directive::m_guid =
{ 0x6fbc7aef, 0x1114, 0x4312, { 0x92, 0xfb, 0x3, 0xaa, 0xba, 0x81, 0x52, 0x16 } };
const f1::guid_t & f1::f1pp::endblock_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::endblock_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

// Operations
const status_t f1::f1pp::endblock_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> cpp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == cpp_par )
		return STATUS_ERROR_INVALID_TYPE;

	block_directive::frame * frm = type_cast<block_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endblock" );
	if( frm->get_closing_directive_guid() != endblock_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endblock" );

	frm->check_different_sources();	// warn if sources differ

	// Close the directive
	parser.leave_directive();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endblock_directive.cxx*/
