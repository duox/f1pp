/*elif_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::c99::elif_directive::elif_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::c99::elif_directive::get_name() const
{
	return "elif";
}
// {3A47F50E-809F-4D82-A7B0-665CA0C3AC08}
const f1::guid_t f1::c_pp::c99::elif_directive::m_guid =
{ 0x3a47f50e, 0x809f, 0x4d82, { 0xa7, 0xb0, 0x66, 0x5c, 0xa0, 0xc3, 0xac, 0x08 } };
const f1::guid_t & f1::c_pp::c99::elif_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::c99::elif_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_keep_text_eol) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::c99::elif_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Pop the last nesting level
	if_directive::frame * frm = type_cast<if_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
	{
		parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#elif" );

		par->skip_to_nl();
		return STATUS_SUCCESS;
	}
	if( frm->m_was_else )
	{
		parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#elif" );

		par->skip_to_nl();
		return STATUS_SUCCESS;
	}
	if( frm->m_allow_parse )
	{
		frm->m_allow_parse = false;
		frm->allow_exec( frm->m_allow_parse );	//frm->update();

		par->skip_to_nl();
		return STATUS_SUCCESS;
	}

	// Fetch expression
//	uint64_t result = 0;
//	if( c_pp_par->parse_expression( result ).failed_status( status ) )
//		return status;

	// Fetch string
	if( isspace( par->peek_char() ) )
		par->skip_char();
//	pp::string	buf;
//	if( c_pp_par->parse_to_newline( buf ).failed_status( status ) )
//		return status;

	// Fetch expression and expand macros in the string
#if 1
	c_pp_par->modify_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode );
	c_pp_par->capture_line( line_capture_callback, this );
#else
	io::buffer_ostream buf_os;
	buf_os.initialize();

	pp::raw_output	str_out;
	str_out.initialize( *parser.get_context(), &buf_os );

	string_parser	new_c_par;
	new_c_par.initialize( *parser.get_context(), *parser.get_syntax(), str_out, buf );
	new_c_par.modify_c_flags( c_pp::parser::c_flags_t::fl_disable_directives );

	if( new_c_par.parse().failed_status( status ) )
		return status;
	const core::buffer & out_buf = buf_os.get_buffer();

	if( 0 == out_buf.get_data_size() )
	{
		parser.message( syntax::IMSG_ERROR_EXPECTED_EXPRESSION );
		return STATUS_SUCCESS;
	}

	// Parse expression
	pp::string out_str( (const char *) out_buf.get_ptr(), out_buf.get_data_size() );

	string_parser	new_c_par2;
	new_c_par2.initialize( parser, out_str );

	bool result = 0;
	new_c_par2.parse_boolean_expression( result );

	// Update context
	frm->m_allow_parse = result;
	frm->update();
	frm->check_different_sources();	// warn if sources differ
#endif

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c99::elif_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;
	bool result = false;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_INVALID_TYPE;
	if_directive::frame * frm = type_cast<if_directive::frame *>( parser.get_directive_frame() );

	// Parse buffer
	if( 0 == out_buf.get_data_size() )
		parser.message( syntax::IMSG_ERROR_EXPECTED_EXPRESSION );
	else if( !frm->m_if_executed )
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
			parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "if" );
	}

	// Update context
	frm->m_allow_parse = result;
	frm->m_if_executed |= result;
	frm->allow_exec( frm->m_allow_parse );	//frm->update();
	frm->check_different_sources();	// warn if sources differ

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c99::elif_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::c_pp::c99::elif_directive> drc = type_cast<f1::c_pp::c99::elif_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::c_pp::c99::elif_directive *>( param )->execute( parser, buf );
#endif
}

/*END OF elif_directive.cxx*/
