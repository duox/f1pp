/*break_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::break_directive::break_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::break_directive::get_name() const
{
	return "break";
}

// {A11E1F16-D725-4e31-8659-F65789EE9F1E}
const f1::guid_t f1::f1pp::break_directive::m_guid =
{ 0xa11e1f16, 0xd725, 0x4e31, { 0x86, 0x59, 0xf6, 0x57, 0x89, 0xee, 0x9f, 0x1e } };
const f1::guid_t & f1::f1pp::break_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::break_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

// Operations
const status_t f1::f1pp::break_directive::parse( pp::Iparser & parser )
{
	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;
	if( !f1pp_par->is_exec_allowed() )
		return STATUS_SUCCESS;
	f1pp_par->set_allow_exec( false );

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Check for argument
	unsigned directive_count = 1;
	const f1::guid_t * directive_guid = nullptr;

	f1pp_par->skip_real_spaces_and_comments();
	if( par->skip_char( ':' ) )
	{
		// Parse label name
		pp::string ident;
		f1pp_par->skip_real_spaces_and_comments();
		f1pp_par->parse_macro_identifier( ident );
		if( ident.empty() )
		{
			f1pp_par->skip_to_newline();
			return parser.message( syntax::IMSG_ERROR_EXPECTED_LABEL_NAME );
		}

		// Find directive that marked with this label
		parser.message( syntax::IMSG_ERROR_NOT_IMPLEMENTED );	// TODO
	}
	else if( par->skip_char( '#' ) )
	{
		// Parse directive name
		pp::string ident;
		f1pp_par->skip_real_spaces_and_comments();
		f1pp_par->parse_directive_identifier( ident );
		if( ident.empty() )
		{
			f1pp_par->skip_to_newline();
			return parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_DIRECTIVE_NAME );
		}

		// Find directive
		core::Tref<pp::Idirective> drct = nullptr;
		f1pp_par->find_directive( drct, pp::Iparser::fl_compare_no_case, ident );
		if( nullptr == drct )
		{
			f1pp_par->skip_to_newline();
			return parser.message( c_pp::syntax::IMSG_ERROR_UNKNOWN_DIRECTIVE, ident.c_str() );
		}
		directive_guid = &drct->get_guid();

		// Parse index
		f1pp_par->skip_real_spaces_and_comments();
		if( !par->is_nl() )
		{
			pp::variant res;
			if( !f1pp_par->parse_integer( res ) )
			{
				f1pp_par->skip_to_newline();
				return parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_INTEGER );
			}
			if( res.as_int() < 0 )
			{
				f1pp_par->skip_to_newline();
				return parser.message( f1pp::syntax::IMSG_ERROR_EXPECTED_POSITIVE_INTEGER );
			}
			directive_count = (unsigned) res.as_uint();
		}
	}

	// Find edge to break through
	core::Tref<pp::Idirective> direc;
	core::Tarray<pp::Iframe *>::reverse_iterator it;
	array_foreach_i( core::Tarray<pp::Iframe *>::reverse_iterator, it, f1pp_par->get_frame_stack() )
	{
		pp::Iframe * frm = *it;
		if( nullptr == frm )
			break;
		frm->modify_flags( pp::Iframe::fl_abort_statement );

		f1pp_par->find_directive( direc, frm->get_openning_directive_guid() );
		__debugbreak_if( nullptr == direc )
			continue;
		if( nullptr != directive_guid && *directive_guid != direc->get_guid() )
			continue;
		if( direc->get_caps( pp::Idirective::cap_hard_edge ) )
		{
			-- directive_count;
			if( 0 == directive_count )
				break;
			continue;
		}

		f1pp_par->find_directive( direc, frm->get_closing_directive_guid() );
		__debugbreak_if( nullptr == direc )
			continue;
		if( direc->get_caps( pp::Idirective::cap_hard_edge ) )
			break;
	}
	if( !it.valid() )
		return parser.message( syntax::IMSG_ERROR_INVALID_S, "#break" );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF break_directive.cxx*/
