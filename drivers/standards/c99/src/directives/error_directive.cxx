/*error_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::c99::error_directive::error_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::c99::error_directive::get_name() const
{
	return "error";
}
// {FADEB2D4-FDAA-4f12-9D3A-93EC1AEAD00F}
const f1::guid_t f1::c_pp::c99::error_directive::m_guid =
{ 0xfadeb2d4, 0xfdaa, 0x4f12, { 0x9d, 0x3a, 0x93, 0xec, 0x1a, 0xea, 0xd0, 0xf } };
const f1::guid_t & f1::c_pp::c99::error_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::c99::error_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::c99::error_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch string
	if( isspace( par->peek_char() ) )
		par->skip_char();
//	pp::string	buf;
//	if( c_pp_par->parse_to_newline( buf ).failed_status( status ) )
//		return status;

	// Get appropriate message id
//	int msg_id = 0 != c_pp_par->get_c_flags( c_pp::parser::c_flags_t::fl_multiple_error_directives ) ?
//		syntax::IMSG_ERROR_USER_N:
//		syntax::IMSG_FATAL_ERROR_USER_N;

	// Expand macros in the string
	if( parser.get_context()->get_flags( pp::context::fl_message_macro_expansion ) )
	{
#if 1
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

		// Display error and set fatal (user) error flag
		parser.message( msg_id, out_buf.get_data_size(), (const char *) out_buf.get_ptr() );
#endif
	}
	else
	{
		pp::string	buf;
		if( c_pp_par->parse_to_newline( buf ).failed_status( status ) )
			return status;
		execute( parser, buf );
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c99::error_directive::execute( pp::Iparser & parser, const pp::string & buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Dequotize string if required (this involves exact un-stringification)
	pp::string val;
	if( c_pp_par->get_c_flags( parser::c_flags_t::fl_dequotize_messages ) )
	{
		if( c_pp_par->unstringify_chars( val, buf ).failed_status( status ) )
			return status;
	}
	else
		val = buf;

	// Get appropriate message id
	int msg_id = 0 != c_pp_par->get_c_flags( c_pp::parser::c_flags_t::fl_multiple_error_directives ) ?
		syntax::IMSG_ERROR_USER_N:
		syntax::IMSG_FATAL_ERROR_USER_N;

	// Display error and set fatal (user) error flag
	parser.message( msg_id, val.length(), val.data() );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c99::error_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::c_pp::c99::error_directive> drc = type_cast<f1::c_pp::c99::error_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, pp::string( buf ) );
#else
	return static_cast<f1::c_pp::c99::error_directive *>( param )->execute( parser, pp::string( buf ) );
#endif
}

/*END OF error_directive.cxx*/
