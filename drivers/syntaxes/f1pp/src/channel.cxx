/*channel.cxx*/
#include "pch.h"
#include "main.h"

f1::f1pp::channel::channel( const core::Iidentifier * ident ):
	super( ident )
{
	m_write_hook.m_channel = this;
}
f1::f1pp::channel::~channel()
{
	if( nullptr != m_storage )
		m_storage->close();
}

const status_t f1::f1pp::channel::initialize( pp::Iparser & par, const pp::string & name,
	const pp::string & storage_name, const f1::guid_t * output_guid )
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Store parameters
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&par);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	m_parser = &par;
	m_name = name;
	m_storage_name = storage_name;

	// Create storage object
	core::Tref<pp::Istorage> stg;
	const f1::guid_t & guid = nullptr != output_guid ? *output_guid : pp::file_storage_class::m_guid;
	if( pp::service::get().create_storage( stg, guid, *par.get_context(), storage_name.get_string() ).failed_status( status ) )
		return status;
	m_storage = stg;

	stg->append_hook( m_write_hook );

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const f1::pp::Iparser * f1::f1pp::channel::get_parser() const
{
	return m_parser;
}
f1::pp::Iparser * f1::f1pp::channel::get_parser()
{
	return m_parser;
}

const f1::pp::Istorage * f1::f1pp::channel::get_storage() const
{
	return m_storage;
}
f1::pp::Istorage * f1::f1pp::channel::get_storage()
{
	return m_storage;
}


const status_t f1::f1pp::channel::modify_flags( unsigned flags_to_set, unsigned flags_to_clear, unsigned * flags_prev )
{
	m_flags.modify( flags_to_set, flags_to_clear, flags_prev );
	return STATUS_SUCCESS;
}
unsigned f1::f1pp::channel::get_flags( unsigned mask ) const
{
	return m_flags & mask;
}

const f1::pp::string & f1::f1pp::channel::get_name() const
{
	return m_name;
}
const f1::pp::string & f1::f1pp::channel::get_storage_name() const
{
	return m_storage_name;
}

const status_t f1::f1pp::channel::set_definition_location( f1::parser::Isource & src, f1::parser::lineno_t line )
{
	m_definition_source = &src;
	m_definition_line = line;

	return STATUS_SUCCESS;
}

const status_t f1::f1pp::channel::set_header( const pp::string & str )
{
	m_header = str;
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel::set_header_move( pp::string & str )
{
	m_header.move( str );
	return STATUS_SUCCESS;
}
const f1::pp::string & f1::f1pp::channel::get_header() const
{
	return m_header;
}

const status_t f1::f1pp::channel::set_footer( const pp::string & str )
{
	m_footer = str;
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel::set_footer_move( pp::string & str )
{
	m_footer.move( str );
	return STATUS_SUCCESS;
}
const f1::pp::string & f1::f1pp::channel::get_footer() const
{
	return m_footer;
}

const status_t f1::f1pp::channel::set_prefix( const pp::string & str )
{
	m_prefix = str;
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel::set_prefix_move( pp::string & str )
{
	m_prefix.move( str );
	return STATUS_SUCCESS;
}
const f1::pp::string & f1::f1pp::channel::get_prefix() const
{
	return m_prefix;
}

const status_t f1::f1pp::channel::set_suffix( const pp::string & str )
{
	m_suffix = str;
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel::set_suffix_move( pp::string & str )
{
	m_suffix.move( str );
	return STATUS_SUCCESS;
}
const f1::pp::string & f1::f1pp::channel::get_suffix() const
{
	return m_suffix;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::channel::activate()
{
	__debugbreak_if( nullptr == m_parser )
		return STATUS_ERROR_NOT_INITIALIZED;
	__debugbreak_if( nullptr == m_storage )
		return STATUS_ERROR_NOT_INITIALIZED;
	if( nullptr != m_storage_previous )
		return STATUS_SUCCESS;	// already active

	m_storage_previous = m_parser->get_storage();
	m_parser->set_storage( m_storage );
	m_storage->set_newline_flag( true );

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel::deactivate()
{
	__debugbreak_if( nullptr == m_parser )
		return STATUS_ERROR_NOT_INITIALIZED;
	__debugbreak_if( nullptr == m_storage_previous )
		return STATUS_SUCCESS;	// already inactive

	m_parser->set_storage( m_storage_previous );
	m_storage_previous = nullptr;

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel::write( const pp::string & data )
{
	__debugbreak_if( nullptr == m_storage )
		return STATUS_ERROR_NOT_INITIALIZED;

	m_write_hook.on_before_write( data );
	m_storage->write( data );
	m_write_hook.on_after_write( data );

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel::flush()
{
	__debugbreak_if( nullptr == m_storage )
		return STATUS_ERROR_NOT_INITIALIZED;
	return m_storage->flush();
}

const status_t f1::f1pp::channel::display_channel_location( pp::Iparser & par, msgout::msgid_t msgid )
{
	if( nullptr != m_definition_source )
		par.message( m_definition_source->get_filename(), m_definition_line, msgid );
	else
		par.message( c_pp::syntax::IMSG_INFO_DEFINED_IN_COMMAND_LINE );
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

f1::f1pp::channel::write_hook::write_hook() noexcept:
	m_first( true ), m_self( 0 )
{
}
const status_t f1::f1pp::channel::write_hook::on_before_write( const pp::string & str )
{
	__debugbreak_if( nullptr == m_channel )
		return STATUS_ERROR_NOT_INITIALIZED;
	if( 0 != m_self )
		return STATUS_SUCCESS;
	++ m_self;

	if( m_first )
	{
		if( !m_channel->get_header().empty() )
		{
			m_storage->write( m_channel->get_header() );
			m_storage->write( m_channel->get_parser()->get_context()->get_current_newline_type_seq() );
		}
		m_first = false;
	}

	if( !m_channel->get_prefix().empty() )
	{
		m_storage->write( m_channel->get_prefix() );
	}

	-- m_self;
	__unreferenced_parameter( str );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel::write_hook::on_after_write( const pp::string & str )
{
	__debugbreak_if( nullptr == m_channel )
		return STATUS_ERROR_NOT_INITIALIZED;
	if( 0 != m_self )
		return STATUS_SUCCESS;
	++ m_self;

	if( !m_channel->get_suffix().empty() )
	{
		m_storage->write( m_channel->get_suffix() );
	}

	-- m_self;
	__unreferenced_parameter( str );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel::write_hook::on_close()
{
	__debugbreak_if( nullptr == m_channel )
		return STATUS_ERROR_NOT_INITIALIZED;
	if( m_self )
		return STATUS_SUCCESS;
	m_self = true;

	if( !m_first && !m_channel->get_footer().empty() )
	{
		core::Tref<pp::Iparser> parser = m_channel->get_parser();
		core::Tref<pp::Ioutput> output = parser->get_output();

		if( !m_storage->get_newline_flag() && 0 == m_channel->get_flags( fl_no_newline ) )
			m_storage->write( parser->get_context()->get_current_newline_type_seq() );
		m_storage->write( m_channel->get_footer() );
		m_first = true;
	}

	m_self = false;
	return STATUS_SUCCESS;
}

/*END OF channel.cxx*/
