/*function_frame.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::function_frame::function_frame( f1pp::parser & parser ):
	super( parser )
{
}

// Return resulting value
const status_t f1::f1pp::function_frame::enter()
{
	m_parser.dynamic_pointer_cast<c_pp::parser>()->modify_c_flags( null, parser::c_flags_t::fl_disable_directives, &m_old_c_flags );

	m_returned_value.set_type( f1::pp::variant::ty_invalid );
	return super::enter();
}
const status_t f1::f1pp::function_frame::leave()
{
//	m_parser.dynamic_pointer_cast<c_pp::parser>()->set_c_flags( m_old_c_flags );

	if( !m_returned_value.is_valid() )
		m_parser->message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, "#return" );
	else
	{
		pp::string buf;
		m_returned_value.format( buf );
		//m_parser->output( buf );
		m_parser->get_parser()->insert_patch( buf );
	}

	return super::leave();
}

/*END OF function_frame.cxx*/
