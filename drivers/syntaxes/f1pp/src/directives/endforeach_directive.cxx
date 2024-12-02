/*endforeach_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endforeach_directive::endforeach_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endforeach_directive::get_name() const
{
	return "endforeach";
}
// {A24230F1-0ED7-42F2-8D59-5106EC4078D4}
const f1::guid_t f1::f1pp::endforeach_directive::m_guid =
{ 0xa24230f1, 0x0ed7, 0x42f2, { 0x8d, 0x59, 0x51, 0x06, 0xec, 0x40, 0x78, 0xd4 } };
const f1::guid_t & f1::f1pp::endforeach_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::endforeach_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::endforeach_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	f1::pp::Iframe * i_ctx = parser.get_directive_frame();
	if( nullptr == i_ctx )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endforeach" );

	foreach_directive::frame * frm = type_cast<foreach_directive::frame *>( i_ctx );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endforeach" );
	if( frm->get_closing_directive_guid() != endforeach_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endforeach" );

	frm->check_different_sources();	// warn if sources differ

	// Check the counter
	if( !f1pp_par->check_limit_value_min( pp::limit_t::li_maximum_iteration_count, frm->m_iteration_count + 1 ) )
	{
		f1pp_par->message( syntax::IMSG_FATALERROR_TOO_MANY_ITERATIONS );
	}
	else
	{
		frm->_update( *f1pp_par );
		if( 0 == frm->get_flags( pp::Iframe::fl_abort_statement ) )
		{
			// Move to the part to execute
			parser.get_parser()->set_pos( frm->m_position );
			parser.get_parser()->set_location( frm->get_initial_location() );
			++ frm->m_iteration_count;

			// Exit
			frm->undefine_locals();
			return STATUS_SUCCESS;
		}
	}

	// Pop the last nesting level
	parser.leave_directive();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endforeach_directive.cxx*/
