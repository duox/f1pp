/*undef_directive.cxx*/
/** @file
 * @brief Extension to the C99 standard #undef directive.
 */
#include "../pch.h"
#include "../main.h"

f1::f1pp::undef_directive::undef_directive( unsigned flags ):
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

unsigned f1::f1pp::undef_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return (super::get_caps() | Idirective::cap_override) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::undef_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	const bool force = f1pp_par->exists_directive_attribute( "force" );

	// Fetch all macro identifiers
	for( ; ; )
	{
		// Fetch macro identifier
		pp::string ident;
		f1pp_par->skip_real_spaces_and_comments();
		if( f1pp_par->parse_macro_identifier( ident ).failed_status( status ) )
			return status;

		// Undefine another macro
		undefine_macro( parser, ident, force ).ignore_result();

		// Skip separator
		f1pp_par->skip_real_spaces_and_comments();
		if( par->is_nl() )
			break;
		if( !par->skip_char( ',' ) )
		{
			f1pp_par->skip_to_newline();
			return parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, "," );
		}
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF undef_directive.cxx*/
