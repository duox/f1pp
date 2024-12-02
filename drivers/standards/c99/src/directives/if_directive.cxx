/*if_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::c99::if_directive::if_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::c99::if_directive::get_name() const
{
	return "if";
}
// {9528E939-00E7-416b-90BE-4AA9AE40E326}
const f1::guid_t f1::c_pp::c99::if_directive::m_guid =
{ 0x9528e939, 0xe7, 0x416b, { 0x90, 0xbe, 0x4a, 0xa9, 0xae, 0x40, 0xe3, 0x26 } };
const f1::guid_t & f1::c_pp::c99::if_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::c99::if_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_structured | cap_keep_text_eol) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::c99::if_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch expression string and expand macros
	if( isspace( par->peek_char() ) )
		par->skip_char();
	c_pp_par->modify_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode );
	c_pp_par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c99::if_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;
	bool result = false;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	// Parse buffer
	if( 0 == out_buf.get_data_size() )
		parser.message( syntax::IMSG_ERROR_EXPECTED_EXPRESSION );
	else
	{
		const char * start = (const char *) out_buf.get_ptr();
		const char * const end = (const char *) out_buf.get_end_ptr();

		// Parse expression
		// Using direct parsing from buffer since the string is read and glued, all macros are substituted
		status = c_pp_par->parse_boolean_expression( result, start, end );
		UNREFERENCED_VALUE(status);	// ignore invalid status in expression

		// Process result
		c_pp_par->skip_real_spaces_and_comments( start, end );
		if( start < end )
			parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "#if" );
	}

	// Begin directive even if there were some errors
	parser.enter_directive( *new(std::nothrow) if_directive::frame( *c_pp_par, result ) );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c99::if_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
	parser.modify_parser_flags( pp::Iparser::parser_flags_t::null, pp::Iparser::parser_flags_t::fl_expression_mode );
#ifdef _DEBUG
	core::Tref<f1::c_pp::c99::if_directive> drc = type_cast<f1::c_pp::c99::if_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::c_pp::c99::if_directive *>( param )->execute( parser, buf );
#endif
}

f1::pp::Iframe * f1::c_pp::c99::if_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::c_pp::parser> cpp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == cpp_par )
		return nullptr;
	return new(std::nothrow) frame( *cpp_par, false );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
f1::c_pp::c99::if_directive::frame::frame( c_pp::parser & parser, bool allow_parse ):
	super( parser ),
	m_allow_parse( allow_parse ), m_if_executed( allow_parse ), m_was_else( false ), m_prev_allow_parse( true )
{
}
void f1::c_pp::c99::if_directive::frame::delete_self()
{
	delete this;
}
const char * f1::c_pp::c99::if_directive::frame::get_openning_directive_name() const
{
	return "if";
}
const char * f1::c_pp::c99::if_directive::frame::get_closing_directive_name() const
{
	return "endif";
}
const f1::guid_t & f1::c_pp::c99::if_directive::frame::get_openning_directive_guid() const
{
	return if_directive::m_guid;
}
const f1::guid_t & f1::c_pp::c99::if_directive::frame::get_closing_directive_guid() const
{
	return endif_directive::m_guid;
}
const status_t f1::c_pp::c99::if_directive::frame::enter()
{
	core::Tref<f1::c_pp::parser> c_pp_par = m_parser.dynamic_pointer_cast<c_pp::parser>();
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	super::enter();

	m_prev_allow_parse = c_pp_par->was_exec_allowed();
	allow_exec( m_allow_parse && m_prev_allow_parse, true );

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c99::if_directive::frame::leave()
{
	core::Tref<f1::c_pp::parser> c_pp_par = m_parser.dynamic_pointer_cast<c_pp::parser>();
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	allow_exec( m_prev_allow_parse );

	return super::leave();
}

/*END OF if_directive.cxx*/
