/*output.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::output::gnucpp::output::output( const core::Iidentifier * ident ) noexcept:
	super( ident ),
	m_last_lineno( 0 ), m_was_nl( true ), m_was_space( true )
{
}

const status_t f1::pp::output::gnucpp::output::write( const char * string, size_t length )
{
	// Check current state
	core::Tref<pp::Istorage> stg = m_parser->get_storage();
	__debugbreak_if( nullptr == stg )
		return STATUS_ERROR_NOT_INITIALIZED;

	if( size_t(-1) == length )
		length = strlen( string );
	if( 0 == length )
		return STATUS_SUCCESS;

	if( m_lineno != m_last_lineno )
		m_was_space = true;

	// Compress leading and trailing whitespace if needed
	if( m_output_flags & output_flags_t::fl_compress_whitespace )
	{
		const char * start = string;
		size_t len = length;
		bool was_spaces_at_end;
		pp::service::trim_spaces( start, len, &was_spaces_at_end );
		const char * end = start + len - 1;

		if( !m_was_space )				// if there wasn't a space before
		{
			if( 0 == len )	// only spaces
			{
				end = string + length - 1;
				len = 1;
			}
			else
			{
				// Now step back if we need a space
				if( string != start )			// if we managed to skip some whitespace
				{
					-- start;					// ... leave a separating space
					++ len;
				}

				// Now step forward if we need a space
				if( was_spaces_at_end )			// if we managed to skip some whitespace
				{
					++ end;						// ... leave a separating space
					++ len;
				}
			}
		}
		m_was_space = was_spaces_at_end;
		string = start;
		length = len;
	}
	if( 0 == length )
		return STATUS_SUCCESS;

	// Update new lines
	if( '\n' == *string || '\r' == *string )
		m_was_nl = true;

	if( m_lineno != m_last_lineno )
	{
		if( !m_was_nl )
			stg->write( "\r\n", 2 );

		char * ep;
		char buf[128] = "#line ";
		_ltoa_s( m_lineno, buf + 6, countof(buf) - 6, 10 );
		ep = _nstrncatx( buf, countof(buf), "\r\n", 2 );
		stg->write( buf, ep - buf );
		m_last_lineno = m_lineno;
	}

	if( '\n' == string[length - 1] || '\r' == string[length - 1] )
		m_was_nl = true;
	else
		m_was_nl = false;

	// Write data
	stg->write( string, length );

	// Exit
	return STATUS_SUCCESS;
}

static const char file_message_header[] = "# 1 ";
static const char file_message_footer[] = "\r\n";
const status_t f1::pp::output::gnucpp::output::on_file_changed( const char * filename, f1::parser::lineno_t lineno ) __noexcept
{
	core::Tref<pp::Istorage> stg = m_parser->get_storage();
	__debugbreak_if( nullptr == stg )
		return STATUS_ERROR_NOT_INITIALIZED;

	if( nullcstr == filename )
		return STATUS_SUCCESS;

	super::on_file_changed( filename, lineno );

	if( !m_was_nl )
		stg->write( "\r\n", 2 );

	pp::string printable_filename;
	convert_to_printable( printable_filename, filename );

	stg->write( file_message_header, countof(file_message_header) - 1 );
	stg->write( printable_filename.data(), printable_filename.length() );
	stg->write( file_message_footer, countof(file_message_footer) - 1 );
	m_was_nl = true;
	m_was_space = true;

	return STATUS_SUCCESS;
}

/*END OF output.cxx*/
