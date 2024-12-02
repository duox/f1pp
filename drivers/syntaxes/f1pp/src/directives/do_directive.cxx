/*do_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::do_directive::do_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::do_directive::get_name() const
{
	return "do";
}
// {6FECC5C7-18AA-48d8-90D9-091F520F865C}
const f1::guid_t f1::f1pp::do_directive::m_guid =
{ 0x6fecc5c7, 0x18aa, 0x48d8, { 0x90, 0xd9, 0x9, 0x1f, 0x52, 0xf, 0x86, 0x5c } };
const f1::guid_t & f1::f1pp::do_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::do_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_hard_edge | cap_invariant | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::do_directive::parse( pp::Iparser & parser )
{
	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;
	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Check whether identifier is specified
	// Macro specified in such a way belongs to the upper namespace in order to be examined
	// after the loop finishes
	f1pp_par->skip_real_spaces_and_comments();
	par->push_pos();

	core::Tref<pp::Imacro> mac = nullptr;
	pp::string ident;
	f1pp_par->parse_macro_identifier( ident );
	if( !ident.is_empty() )
	{
		f1pp_par->skip_real_spaces_and_comments();
		if( par->skip_char( ':' ) )
		{
			if( parser.find_macro( mac, ident ).failed() && parser.create_macro( mac, ident ).failed() )
				parser.message( syntax::IMSG_ERROR_CANNOT_DEFINE_MACRO_S, ident.c_str() );
			else
				mac->set_value_variant( pp::variant( false ) );
		}
	}

	if( nullptr == mac )
		par->pop_pos();
	else
		par->pop_discard_pos();

	// Create execution context and enter new nested block
	frame * frm = new(std::nothrow) frame( *f1pp_par );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;
	frm->m_control_macro = mac;
	parser.enter_directive( *frm );

	// Store current position of the beginning of the expression
	parser.get_parser()->get_pos( frm->m_position );

	// Exit
	return STATUS_SUCCESS;
}
f1::pp::Iframe * f1::f1pp::do_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return nullptr;
	return new(std::nothrow) frame( *f1pp_par );
}

f1::f1pp::do_directive::frame::frame( f1pp::parser & par ):
	super( par ),
	m_iteration_count( 0 )
{
}
const char * f1::f1pp::do_directive::frame::get_openning_directive_name() const
{
	return "do";
}
const char * f1::f1pp::do_directive::frame::get_closing_directive_name() const
{
	return "until";
}
const f1::guid_t & f1::f1pp::do_directive::frame::get_openning_directive_guid() const
{
	return do_directive::m_guid;
}
const f1::guid_t & f1::f1pp::do_directive::frame::get_closing_directive_guid() const
{
	return until_directive::m_guid;
}

/*END OF do_directive.cxx*/
