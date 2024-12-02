/*Ioutput.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::Ioutput::Ioutput( const core::Iidentifier * ident ) noexcept:
	super( ident ),
	m_output_flags( output_flags_t::null ),
	m_lineno( 1 ), m_global_lineno( 1 ), m_subsection_count( 0 )
{
}
f1::pp::Ioutput::~Ioutput() noexcept
{
	if( nullptr != m_parser )
		on_detach_from_parser( *m_parser );
//	__assert( 0 == m_subsection_count );
}

const status_t f1::pp::Ioutput::initialize( /*const pp::context & context, f1::io::Iostream * str, */output_flags_t output_flags )
{
	status_t status;

	if( super::initialize().failed_status( status ) )
		return status;

	//m_context = &context;
	//m_stream = str;
	m_output_flags = output_flags;

	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::Ioutput::move( core::object * obj ) noexcept
{
	return move( type_cast<self *>(obj) );
}
const status_t f1::pp::Ioutput::move( self * obj )
{
	status_t status;

	if( super::move( obj ).failed_status( status ) )
		return status;

	/*if( nullptr != obj->m_stream )
	{
		if( nullptr != m_stream && m_stream->move( obj->m_stream ).failed_status( status ) )
			return status;
	}
	else
		m_stream = nullptr;*/

	m_output_flags = obj->m_output_flags;
	obj->m_output_flags = output_flags_t::null;

	m_buffer.move( &obj->m_buffer );
	m_filename.move( obj->m_filename );
	m_lineno = obj->m_lineno; obj->m_lineno = 0;

	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::Ioutput::modify_flags( output_flags_t flags_to_set, output_flags_t flags_to_clear, output_flags_t * prev_flags )
{
	if( nullptr != prev_flags )
		*prev_flags = m_output_flags;
	m_output_flags = (m_output_flags | flags_to_set) & ~flags_to_clear;
	return STATUS_SUCCESS;
}
f1::pp::Ioutput::output_flags_t f1::pp::Ioutput::get_flags( output_flags_t mask ) const
{
	return m_output_flags & mask;
}

const f1::pp::context * f1::pp::Ioutput::get_context() const noexcept
{
	return m_parser->get_context();
//	return m_context;
}

const status_t f1::pp::Ioutput::on_attach_to_parser( pp::Iparser & parser ) noexcept
{
	__debugbreak_if( &parser == m_parser )
		return STATUS_WARNING_ALREADY_OWNED;
	__debugbreak_if( nullptr != m_parser )
		return STATUS_ERROR_INVALID_OWNER;

	m_parser = &parser;

	return STATUS_SUCCESS;
}
const status_t f1::pp::Ioutput::on_detach_from_parser( pp::Iparser & parser ) noexcept
{
	__debugbreak_if( nullptr == m_parser )
		return STATUS_ERROR_NOT_OWNED;
	__debugbreak_if( &parser != m_parser )
		return STATUS_ERROR_INVALID_OWNER;

	m_parser = nullptr;

	return STATUS_SUCCESS;
}
const f1::pp::Iparser * f1::pp::Ioutput::get_parser() const noexcept
{
	return m_parser;
}
f1::pp::Iparser * f1::pp::Ioutput::get_parser() noexcept
{
	return m_parser;
}

/*const status_t f1::pp::Ioutput::set_output_filename( const char * filename, size_t length )
{
	core::Tref<f1::io::file_ostream> outp = new(std::nothrow) io::file_ostream(
		size_t(-1) == length ? filename : core::string( filename, length ).c_str() );
	if( nullptr == outp || outp->bad() )
		return STATUS_ERROR_CANNOT_OPEN;
	return set_output_stream( *outp );
}
const status_t f1::pp::Ioutput::set_output_stream( f1::io::Iostream & str )
{
	m_stream = &str;
	return STATUS_SUCCESS;
}
f1::io::Iostream * f1::pp::Ioutput::get_output_stream()
{
	return m_stream;
}*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::Ioutput::begin_output() noexcept
{
//	__debugbreak_if( nullptr == m_stream )
//		return STATUS_ERROR_NOT_INITIALIZED;

	return STATUS_SUCCESS;
}
const status_t f1::pp::Ioutput::end_output() noexcept
{
//	__debugbreak_if( nullptr == m_stream )
//		return STATUS_ERROR_NOT_INITIALIZED;

//	m_stream->flush();
//	m_stream = nullptr;

	return STATUS_SUCCESS;
}

const status_t f1::pp::Ioutput::begin_subsection( const char * ident, size_t len ) noexcept
{
	++ m_subsection_count;
	__unreferenced_parameter( ident );
	__unreferenced_parameter( len );
	return STATUS_SUCCESS;
}
const status_t f1::pp::Ioutput::begin_subsection( const core::static_string & ident ) noexcept
{
	return begin_subsection( ident.data(), ident.length() );
}
const status_t f1::pp::Ioutput::begin_subsection( const pp::string & ident ) noexcept
{
	return begin_subsection( ident.data(), ident.length() );
}
const status_t f1::pp::Ioutput::end_subsection() noexcept
{
	__assert( 0 != m_subsection_count );
	-- m_subsection_count;
	return STATUS_SUCCESS;
}

const status_t f1::pp::Ioutput::write( const char * string, size_t len )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( string );
	__unreferenced_parameter( len );
	return STATUS_ERROR_PURE_CALL;
}
const status_t f1::pp::Ioutput::writefv( const char * format, va_list ap )
{
	f1::core::string str;
	status_t status = str.formatV( format, ap );
	if( status.failed() )
		return status;
	return write( str.data(), str.size() );
}
const status_t f1::pp::Ioutput::writef( const char * format, ... )
{
	va_list ap;
	va_start( ap, format );
	status_t status = writefv( format, ap );
	va_end( ap );
	return status;
}

const status_t f1::pp::Ioutput::on_file_changed( const char * filename, f1::parser::lineno_t lineno ) noexcept
{
	const f1::pp::context * ctx = get_context();
	if( nullptr != ctx && ctx->get_flags( pp::context::fl_output_file_names_only ) )
	{
		m_filename = PathFindFileName( filename );
	}
	else
	{
		const fs::path & base_path = m_parser->get_base_directory();
		if( !base_path.is_empty() )
			base_path.make_relative( m_filename, filename );
		else
			m_filename = filename;
	}
	m_lineno = lineno;
	return STATUS_SUCCESS;
}
const status_t f1::pp::Ioutput::on_line_changed( f1::parser::lineno_t lineno ) noexcept
{
	if( 0 == m_subsection_count )
		m_lineno = lineno;

	return STATUS_SUCCESS;
}
const f1::core::string & f1::pp::Ioutput::get_filename() const
{
	return m_filename;
}
f1::parser::lineno_t f1::pp::Ioutput::get_line() const
{
	return m_lineno;
}
f1::parser::lineno_t f1::pp::Ioutput::get_global_line() const
{
	return m_global_lineno;
}

const f1::core::buffer & f1::pp::Ioutput::get_buffer() const noexcept
{
	return m_buffer;
}
f1::core::buffer & f1::pp::Ioutput::get_buffer() noexcept
{
	return m_buffer;
}

/*END OF Ioutput.cxx*/
