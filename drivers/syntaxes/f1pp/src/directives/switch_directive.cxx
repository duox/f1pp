/*switch_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::switch_directive::switch_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::switch_directive::get_name() const
{
	return "switch";
}
// {2BFB0440-A74F-48c5-8E24-C5FFD6663EE8}
const f1::guid_t f1::f1pp::switch_directive::m_guid =
{ 0x2bfb0440, 0xa74f, 0x48c5, { 0x8e, 0x24, 0xc5, 0xff, 0xd6, 0x66, 0x3e, 0xe8 } };
const f1::guid_t & f1::f1pp::switch_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::switch_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_keep_text_eol | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::switch_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> f1pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch expression string and expand macros
	f1pp_par->modify_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode );
	f1pp_par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::switch_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	// Parse buffer
	if( 0 == out_buf.get_data_size() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_EXPRESSION );
		return STATUS_SUCCESS;
	}

	// Parse expression
	// Using direct parsing from buffer since the string is read and glued, all macros are substituted
	const char *	 start = (const char *) out_buf.get_ptr();
	const char * const end = (const char *) out_buf.get_end_ptr();
	pp::variant result;
	status = f1pp_par->parse_expression( result, start, end, 0 );
	UNREFERENCED_VALUE(status);	// ignore invalid status in expression

	// Process result
	f1pp_par->skip_real_spaces_and_comments( start, end );
	if( start < end )
		parser.message( c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "switch" );

	// Begin directive even if there were some errors
	switch_directive::frame * frm = new(std::nothrow) switch_directive::frame( *f1pp_par );
	frm->m_switch_value = result;
	frm->m_switch_type = switch_directive::frame::Etype_switch;
	parser.enter_directive( *frm );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::switch_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
	parser.modify_parser_flags( pp::Iparser::parser_flags_t::null, pp::Iparser::parser_flags_t::fl_expression_mode );
#ifdef _DEBUG
	core::Tref<f1::f1pp::switch_directive> drc = type_cast<f1::f1pp::switch_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::f1pp::switch_directive *>( param )->execute( parser, buf );
#endif
}
f1::pp::Iframe * f1::f1pp::switch_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return nullptr;
	return new(std::nothrow) frame( *f1pp_par );
}

f1::f1pp::switch_directive::frame::frame( f1pp::parser & par ):
	super( par )
{
}
f1::f1pp::switch_directive::frame::~frame()
{
}
const char * f1::f1pp::switch_directive::frame::get_openning_directive_name() const
{
	return "switch";
}
const char * f1::f1pp::switch_directive::frame::get_closing_directive_name() const
{
	return "endswitch";
}
const f1::guid_t & f1::f1pp::switch_directive::frame::get_openning_directive_guid() const
{
	return switch_directive::m_guid;
}
const f1::guid_t & f1::f1pp::switch_directive::frame::get_closing_directive_guid() const
{
	return endswitch_directive::m_guid;
}

/*END OF switch_directive.cxx*/
