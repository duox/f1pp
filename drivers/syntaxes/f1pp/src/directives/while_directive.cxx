/*while_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::while_directive::while_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::while_directive::get_name() const
{
	return "while";
}
// {510FDB54-8C23-4714-9737-86A57F5B687E}
const f1::guid_t f1::f1pp::while_directive::m_guid =
{ 0x510fdb54, 0x8c23, 0x4714, { 0x97, 0x37, 0x86, 0xa5, 0x7f, 0x5b, 0x68, 0x7e } };
const f1::guid_t & f1::f1pp::while_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::while_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_hard_edge | cap_invariant | cap_keep_text_eol | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::while_directive::parse( pp::Iparser & parser )
{
	status_t status;

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
			if( parser.find_macro( mac, ident, pp::Iparser::find_macro_flags_t::fl_macro_search_current_namespace_only ).failed() &&
				parser.create_macro( mac, ident ).failed_status( status ) )
			{
				parser.message( syntax::IMSG_ERROR_CANNOT_DEFINE_MACRO_S, ident.c_str() );
			}
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
	frm->m_while_directive = this;
	frm->m_control_macro = mac;
	parser.enter_directive( *frm );

	// Check for disabled statement (fast path)
	if( frm->get_flags( pp::Iframe::fl_initially_disabled ) )
	{
		f1pp_par->skip_to_newline();
		return STATUS_SUCCESS;
	}

	// Store current position of the beginning of the expression
	frm->m_position = par->get_ptr();	//parser.get_parser()->get_pos( frm->m_position );

	// Launch loop
	_update_condition( *f1pp_par, *frm );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::while_directive::_update_condition( f1::c_pp::parser & f1pp_par, while_directive::frame & frm )
{
	// Set current position of the beginning of the expression
	//f1pp_par.get_parser()->set_pos( frm.m_position );
	f1pp_par.get_parser()->set_ptr( frm.m_position );
	f1pp_par.get_parser()->set_location( frm.get_initial_location() );
	frm.allow_exec( true );

	// Check limit
	if( !f1pp_par.check_limit_value_min( pp::limit_t::li_maximum_iteration_count, frm.m_iteration_count ) )
	{
		frm.allow_exec( false );
		return f1pp_par.message( syntax::IMSG_FATALERROR_TOO_MANY_ITERATIONS );
	}
	++ frm.m_iteration_count;

	// Fetch expression string and expand macros
	f1pp_par.modify_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode );
	f1pp_par.capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::while_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> f1pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	// Parse buffer
	if( 0 == out_buf.get_data_size() )
	{
		parser.leave_directive();
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

	// Check for unexpected characters after the statement
	f1pp_par->skip_real_spaces_and_comments( start, end );
	if( start < end )
		parser.message( c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "while" );

	// Get current execution context
	while_directive::frame * while_frm = type_cast<while_directive::frame *>( f1pp_par->get_directive_frame() );
	if( nullptr == while_frm )
		return f1pp_par->message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endwhile" );
	if( while_frm->get_closing_directive_guid() != endwhile_directive::m_guid )
		return f1pp_par->message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endwhile" );

	// Process result
	if( !result.can_cast_to( pp::variant::ty_bool ) )
		return f1pp_par->message( syntax::IMSG_ERROR_INVALID_OPERAND_TYPE );

	if( result.as_bool() )
		// Condition evaluated to true, so another iteration is started
		while_frm->allow_exec( true );
	else
	{
		// Condition evaluated to false, wait for `#endwhile' to come
		while_frm->allow_exec( false );
		while_frm->modify_flags( pp::Iframe::fl_abort_statement );
//		if( nullptr != while_frm->m_end_position.m_source )
		if( nullptr != while_frm->m_end_position )
			// Condition evaluated to false, jump directly to `#endwhile'
//			f1pp_par->get_parser()->set_pos( while_frm->m_end_position );
			f1pp_par->get_parser()->set_ptr( while_frm->m_end_position );
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::while_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
	parser.modify_parser_flags( pp::Iparser::parser_flags_t::null, pp::Iparser::parser_flags_t::fl_expression_mode );
#ifdef _DEBUG
	core::Tref<f1::f1pp::while_directive> drc = type_cast<f1::f1pp::while_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::f1pp::while_directive *>( param )->execute( parser, buf );
#endif
}
f1::pp::Iframe * f1::f1pp::while_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return nullptr;
	return new(std::nothrow) frame( *f1pp_par );
}

f1::f1pp::while_directive::frame::frame( f1pp::parser & parser ):
	super( parser ),
	m_end_position( nullptr ), m_iteration_count( 0 )
{
}
const char * f1::f1pp::while_directive::frame::get_openning_directive_name() const
{
	return "while";
}
const char * f1::f1pp::while_directive::frame::get_closing_directive_name() const
{
	return "endwhile";
}
const f1::guid_t & f1::f1pp::while_directive::frame::get_openning_directive_guid() const
{
	return while_directive::m_guid;
}
const f1::guid_t & f1::f1pp::while_directive::frame::get_closing_directive_guid() const
{
	return endwhile_directive::m_guid;
}

/*END OF while_directive.cxx*/
