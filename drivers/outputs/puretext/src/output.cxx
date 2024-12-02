/*output.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::output::puretext::output::output( const core::Iidentifier * ident ) noexcept:
	super( ident ),
	m_last_lineno( 0 ), m_was_nl( true ), m_was_space( true )
{
}

const status_t f1::pp::output::puretext::output::write( const char * string, size_t length )
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
		//const char * end = start + len - 1;

		if( !m_was_space )				// if there wasn't a space before
		{
			if( 0 == len )	// only spaces
			{
				//end = string + length - 1;
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
					//++ end;					// ... leave a separating space
					++ len;
				}
			}
		}
		m_was_space = was_spaces_at_end;
		string = start;
		length = len;
	}
	else
	{
//		for( ++ m_last_lineno; m_last_lineno < m_lineno; ++ m_last_lineno )
//			stg->write( "\r\n", 2 );
		m_was_nl = true;
	}
	if( 0 == length )
		return STATUS_SUCCESS;

	// Update new lines
	if( '\n' == *string || '\r' == *string )
		m_was_nl = true;

/*	if( m_lineno != m_last_lineno )
	{
		if( !m_was_nl )
			stg->write( "\r\n", 2 );
	}
*/
	if( '\n' == string[length - 1] || '\r' == string[length - 1] )
		m_was_nl = true;
	else
		m_was_nl = false;

	m_last_lineno = m_lineno;

	// Write data
	stg->write( string, length );

	// Exit
	return STATUS_SUCCESS;
}

/*static const char file_message_header[] = "#file \"";
static const char file_message_footer[] = "\"\r\n";
const status_t f1::pp::output::puretext::output::on_file_changed( const char * filename, f1::parser::lineno_t lineno ) __noexcept
{
	core::Tref<pp::Istorage> stg = m_parser->get_storage();
	__debugbreak_if( nullptr == stg )
		return STATUS_ERROR_NOT_INITIALIZED;

	if( nullcstr == filename )
		return STATUS_SUCCESS;

	super::on_file_changed( filename, lineno );

	if( !m_was_nl )
		stg->write( "\r\n", 2 );

	stg->write( file_message_header, countof(file_message_header) - 1 );
	stg->write( filename, strlen( filename ) );
	stg->write( file_message_footer, countof(file_message_footer) - 1 );
	m_was_nl = true;

	return STATUS_SUCCESS;
}*/
const status_t f1::pp::output::puretext::output::on_line_changed( f1::parser::lineno_t lineno ) noexcept
{
//	core::Tref<pp::Istorage> stg = m_parser->get_storage();
//	__debugbreak_if( nullptr == stg )
//		return STATUS_ERROR_NOT_INITIALIZED;
//	if( 1 != lineno )
//		stg->write( "\r\n", 2 );
	//if( m_last_lineno == lineno )
	//	-- m_last_lineno;
	if( lineno > m_lineno )
	{
		core::Tref<pp::Istorage> stg = m_parser->get_storage();
		__debugbreak_if( nullptr == stg )
			return STATUS_ERROR_NOT_INITIALIZED;

		#define NL_SEQ		"\n\n\n\n\n\n\n\n"
		#define NL_SEQ_LEN	(_countof(NL_SEQ) - 1)
		size_t count_left = (size_t) lineno - m_lineno;
		while( count_left >= NL_SEQ_LEN )
		{
			stg->write( NL_SEQ, NL_SEQ_LEN );
			count_left -= NL_SEQ_LEN;
		}
		if( 0 != count_left )
			stg->write( NL_SEQ, count_left );
	}


	return super::on_line_changed( lineno );
}

/*END OF output.cxx*/
