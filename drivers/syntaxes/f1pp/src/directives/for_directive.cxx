/*for_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::for_directive::for_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::for_directive::get_name() const
{
	return "for";
}
// {49ED512C-5484-4d75-AA45-5D067B7CDB19}
const f1::guid_t f1::f1pp::for_directive::m_guid =
{ 0x49ed512c, 0x5484, 0x4d75, { 0xaa, 0x45, 0x5d, 0x6, 0x7b, 0x7c, 0xdb, 0x19 } };
const f1::guid_t & f1::f1pp::for_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::for_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_hard_edge | cap_invariant | cap_keep_text_eol | cap_c_extension) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::for_directive::parse( pp::Iparser & parser )
{
	status_t	status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Create execution context and enter new nested block
	frame * frm = new(std::nothrow) frame( *f1par );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;
	frm->m_for_directive = this;
	frm->m_parenthesis = false;
	frm->m_cond_breakpoint_callback.m_f1par = f1par;
	frm->m_cond_breakpoint_callback.m_frm = frm;
	frm->m_init_breakpoint_callback.m_f1par = f1par;
	frm->m_init_breakpoint_callback.m_frm = frm;
	parser.enter_directive( *frm );

	// Check for disabled statement (fast path)
	if( frm->get_flags( pp::Iframe::fl_initially_disabled ) )
	{
		f1par->skip_to_newline();
		return STATUS_SUCCESS;
	}

	// Store current position of the beginning of the expression
	frm->m_position = par->get_ptr();

	// Check for openning parenthesis
	f1par->skip_real_spaces_and_comments();
	if( par->skip_char( '(' ) )
	{
		frm->m_parenthesis = true;
		f1par->skip_real_spaces_and_comments();
	}

	// Evaluate initialization expression
	f1par->skip_real_spaces_and_comments();
	if( par->peek_char( ';' ) )
	{
		frm->m_init_breakpoint_callback.on_breakpoint_reached();
	}
	else
	{
		f1par->get_ns_context().set_current_namespace( &frm->m_own_ns );
		f1par->get_assignment_directive().set_finish_callback( &frm->m_init_breakpoint_callback );
		status = f1par->get_assignment_directive().parse( parser, ';' );
		UNREFERENCED_VALUE(status);	// ignore invalid status in expression
	}

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::f1pp::for_directive::init_breakpoint_callback::on_breakpoint_reached()
{
	return m_frm->m_for_directive->on_init_finished( *m_frm, *m_f1par );
}

const status_t f1::f1pp::for_directive::on_init_finished( frame & frm, f1::f1pp::parser & f1par )
{
	core::Tref<f1::parser::Iparser> par = f1par.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Parse separator
	f1par.skip_real_spaces_and_comments();
	if( !par->skip_char( ';' ) )
	{
		frm.allow_exec( false );
		return f1par.message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, ";" );
	}

	// Find condition expression
	f1par.skip_real_spaces_and_comments();
	frm.m_cond_start = par->get_ptr();
	frm.m_cond_length = 0;
	f1par.skip_to_separator( frm.m_cond_length, ';' );
	if( !par->skip_char( ';' ) )
	{
		frm.allow_exec( false );
		return f1par.message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, ";" );
	}

	// Store location of the step expression
	f1par.skip_real_spaces_and_comments();
	frm.m_step_start = par->get_ptr();
	f1par.skip_to_newline();
	frm.m_step_length = par->get_ptr() - frm.m_step_start;

	// Check for ending characters
	f1par.skip_real_spaces_and_comments();
	if( frm.m_parenthesis && !par->skip_char( ')' ) )
	{
		frm.allow_exec( false );
		f1par.message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, ")" );
	}
	if( !par->is_nl() )
		f1par.message( c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "for.init" );

	// Fetch expression string and expand macros
	_update_condition( frm, f1par );

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::f1pp::for_directive::_update_condition( frame & frm, f1::f1pp::parser & f1par )
{
	// Launch condition parser
	if( 0 == frm.m_cond_length )
	{
		if( !frm.get_flags( pp::Iframe::fl_initially_disabled ) )
			frm.allow_exec( true );
	}
	else
	{
		// Move to the condition part
		f1par.get_parser()->set_ptr( frm.m_cond_start );
		f1par.get_parser()->set_location( frm.get_initial_location() );
	
		f1par.get_ns_context().set_current_namespace( &frm.m_own_ns );

		// Check limit
		if( !f1par.check_limit_value_min( pp::limit_t::li_maximum_iteration_count, frm.m_iteration_count ) )
		{
			frm.allow_exec( false, true );
			return f1par.message( syntax::IMSG_FATALERROR_TOO_MANY_ITERATIONS );
		}
		++ frm.m_iteration_count;

		// Acquire line
		f1par.modify_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode );
		f1par.capture_line( cond_line_capture_callback, this, frm.m_cond_length );
	}

	// Exit
	__unreferenced_parameter( frm );
	return STATUS_SUCCESS;
}

const status_t f1::f1pp::for_directive::cond_line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
	parser.modify_parser_flags( pp::Iparser::parser_flags_t::null, pp::Iparser::parser_flags_t::fl_expression_mode );
#ifdef _DEBUG
	core::Tref<f1::f1pp::for_directive> drc = type_cast<f1::f1pp::for_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute_cond( parser, buf );
#else
	return static_cast<f1::f1pp::for_directive *>( param )->execute_cond( parser, buf );
#endif
}

const status_t f1::f1pp::for_directive::execute_cond( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_INVALID_TYPE;

	for_directive::frame * frm = type_cast<for_directive::frame *>( parser.get_directive_frame() );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;

	if( unsigned(-1) != frm->m_error_count && frm->m_error_count != f1par->get_common_error_count() )
	{
		frm->allow_exec( false );
		return STATUS_SUCCESS;
	}

	if( !f1par->get_parser()->skip_char( ';' ) )
	{
		frm->allow_exec( false );
		return f1par->message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, ";" );
	}

	// Parse buffer
	pp::variant result;
	if( !out_buf.is_empty() )
	{
		const char *	 start = (const char *) out_buf.get_ptr();
		const char * const end = (const char *) out_buf.get_end_ptr();

		// Execute initializing expression
		status = f1par->parse_expression( result, start, end, 0 );
		UNREFERENCED_VALUE(status);	// ignore invalid status in expression

		// Check for ending characters
		f1par->skip_real_spaces_and_comments( start, end );
		if( start < end )
			parser.message( c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS_AFTER, "for.condition" );
	}
	else
		result.set_bool( true );

	// Process result
	if( result.as_bool() )
	{
		frm->allow_exec( true );
	}
	else
	{
		frm->modify_flags( pp::Iframe::fl_abort_statement );
		frm->allow_exec( false, true );
//		if( nullptr != frm->m_end_position )	TODO: m_new_line is cleared so #endfor directive is not processed
//			f1par->get_parser()->set_ptr( frm->m_end_position );
	}
	f1par->get_parser()->set_ptr( frm->m_step_start + frm->m_step_length );
	f1par->get_ns_context().set_current_namespace( &frm->m_locals_ns );

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::f1pp::for_directive::cond_breakpoint_callback::on_breakpoint_reached()
{
	m_frm->m_for_directive->_update_condition( *m_frm, *m_f1par );	// move to the condition expression
	return STATUS_SUCCESS;
}

/*const status_t f1::f1pp::for_directive::set_breakpoint_callback::on_breakpoint_reached()
{
	m_frm->m_for_directive->_update_condition( *m_frm, *m_f1par );	// move to the condition expression
	return STATUS_SUCCESS;
}*/

f1::pp::Iframe * f1::f1pp::for_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return nullptr;
	return new(std::nothrow) frame( *f1pp_par );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


f1::f1pp::for_directive::frame::frame( f1pp::parser & par ):
	super( par ),
	m_position( nullptr ), m_end_position( nullptr ),
	m_cond_start( nullptr ), m_cond_length( 0 ),
	m_step_start( nullptr ), m_step_length( 0 ),
	m_parenthesis( false ), m_iteration_count( 0 ),
	m_error_count( unsigned(-1) )
{
}
f1::f1pp::for_directive::frame::~frame()
{
}
const char * f1::f1pp::for_directive::frame::get_openning_directive_name() const
{
	return "for";
}
const char * f1::f1pp::for_directive::frame::get_closing_directive_name() const
{
	return "endfor";
}
const f1::guid_t & f1::f1pp::for_directive::frame::get_openning_directive_guid() const
{
	return for_directive::m_guid;
}
const f1::guid_t & f1::f1pp::for_directive::frame::get_closing_directive_guid() const
{
	return endfor_directive::m_guid;
}

/*END OF for_directive.cxx*/
