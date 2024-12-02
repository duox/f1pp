/*attribute.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::attribute::attribute( const core::Iidentifier * ident ):
	super( ident )
{
}
const status_t f1::pp::attribute::initialize( const pp::string & name, unsigned flags, pp::arg_list * args )
{
	status_t status;

	if( super::initialize().failed_status( status ) )
		return status;

	m_name = name;
	m_arg_list = args;
	m_flags = flags;

	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const f1::pp::string & f1::pp::attribute::get_name() const
{
	return m_name;
}

const status_t f1::pp::attribute::modify_flags( unsigned flags_to_set, unsigned flags_to_clear, unsigned * flags_prev ) noexcept
{
	m_flags.modify( flags_to_set, flags_to_clear, flags_prev );
	return STATUS_SUCCESS;
}
unsigned __nodiscard f1::pp::attribute::get_flags( unsigned mask ) const noexcept
{
	return m_flags & mask;
}

const f1::pp::arg_list * f1::pp::attribute::get_arg_list() const noexcept
{
	return m_arg_list;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::pp::attribute::output( pp::Iparser & parser ) const
{
	parser.output( core::stringslice( get_name() ) );

	const f1::pp::arg_list * args = get_arg_list();
	if( nullptr != args )
		args->output( parser );

	return STATUS_SUCCESS;
}

/*END OF attribute.cxx*/
