/*assert_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::assert_directive::assert_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::assert_directive::get_name() const
{
	return "assert";
}

// {02490050-9049-4ef1-BAAE-56170C40222F}
const f1::guid_t f1::f1pp::assert_directive::m_guid =
{ 0x2490050, 0x9049, 0x4ef1, { 0xba, 0xae, 0x56, 0x17, 0xc, 0x40, 0x22, 0x2f } };
const f1::guid_t & f1::f1pp::assert_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::assert_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

// Operations
const status_t f1::f1pp::assert_directive::parse( pp::Iparser & parser )
{
	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Expand macros in the string
	f1pp_par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::assert_directive::execute( pp::Iparser & parser, const pp::string & buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// If nothing is specified, just bail out
	const char *			s = buf.data();
	const char * const	end_s = buf.data_end();

	f1pp_par->skip_real_spaces_and_comments( s, end_s );
	if( s >= end_s )
		return parser.message( syntax::IMSG_FATALERROR_ASSERTION_TRIGGERED );

	// Parse expression
	if( '(' == *s )
	{
		++ s;
		f1pp_par->skip_real_spaces_and_comments( s, end_s );

		bool result = false;
		if( ')' != *s )
		{
			if( f1pp_par->parse_boolean_expression( result, s, end_s, ')' ).failed_status( status ) )
				return status;
		}
		++ s;

		if( result )
			return STATUS_SUCCESS;
	}

	// Skip separating space characters
	f1pp_par->skip_real_spaces_and_comments( s, end_s );

	// Display message if it's specified
	pp::string val;
	if( s < end_s )
	{
		// Dequotize string if required (this involves exact un-stringification)
		if( f1pp_par->unstringify_chars( val, pp::string( s, end_s - s ) ).failed_status( status ) )
			return status;

		// Generate fatal error to abort execution
		status = parser.message( syntax::IMSG_FATALERROR_ASSERTION_TRIGGERED_S, val.c_str() );
	}
	else
		// Generate fatal error to abort execution
		status = parser.message( syntax::IMSG_FATALERROR_ASSERTION_TRIGGERED );

	// Exit
	return status;
}
const status_t f1::f1pp::assert_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::f1pp::assert_directive> drc = type_cast<f1::f1pp::assert_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, pp::string( buf ) );
#else
	return static_cast<f1::f1pp::assert_directive *>( param )->execute( parser, pp::string( buf ) );
#endif
}

/*END OF assert_directive.cxx*/
