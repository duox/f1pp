/*block_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::block_directive::block_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::block_directive::get_name() const
{
	return "block";
}

// {34242F66-6EBF-44ad-9B06-B7EEF81D1021}
const f1::guid_t f1::f1pp::block_directive::m_guid =
{ 0x34242f66, 0x6ebf, 0x44ad, { 0x9b, 0x6, 0xb7, 0xee, 0xf8, 0x1d, 0x10, 0x21 } };
const f1::guid_t & f1::f1pp::block_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::block_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

// Operations
const status_t f1::f1pp::block_directive::parse( pp::Iparser & parser )
{
	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Start nested operator
	frame * frm = new(std::nothrow) frame( *f1pp_par );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;
	parser.enter_directive( *frm );

	// Exit
	return STATUS_SUCCESS;
}

f1::f1pp::block_directive::frame::frame( f1pp::parser & par ):
	super( par )
{
}
const char * f1::f1pp::block_directive::frame::get_openning_directive_name() const
{
	return "block";
}
const char * f1::f1pp::block_directive::frame::get_closing_directive_name() const
{
	return "endblock";
}
const f1::guid_t & f1::f1pp::block_directive::frame::get_openning_directive_guid() const
{
	return block_directive::m_guid;
}
const f1::guid_t & f1::f1pp::block_directive::frame::get_closing_directive_guid() const
{
	return endblock_directive::m_guid;
}

/*END OF block_directive.cxx*/
