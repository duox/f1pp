/*output.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::output::linenum::output::output( const core::Iidentifier * ident ) noexcept:
	super( ident ),
	m_last_lineno( 0 ), m_was_space( true ), m_subsection_desc( nullptr )
{
}

const status_t f1::pp::output::linenum::output::begin_subsection( const char * ident, size_t len ) noexcept
{
	size_t prev_length = m_subsections_string.length();
//	m_subsections_string.clear();
	if( nullptr != m_subsection_desc )
	{
		//m_subsections_string.append( ".", 1 );
		char buf[128];
		_ltoa_s( m_lineno - m_subsection_desc->m_start_line + 1, buf, countof(buf), 10 );
		m_subsections_string.append( buf );
	}
	m_subsections_string.append( ".", 1 );
	m_subsections_string.append( ident, size_t(-1) == len ? core::string::length( ident ) : len );
	m_subsections_string.append( ".", 1 );

	subsection_desc * desc = m_subsections.append_new();
	desc->m_start_line = m_lineno;
	desc->m_prev_length = prev_length;
	m_subsection_desc = desc;

	return super::begin_subsection( ident, len );
}
const status_t f1::pp::output::linenum::output::end_subsection() noexcept
{
	__debugbreak_if( m_subsections.empty() )
		return STATUS_ERROR_INVALID_STATE;
	__assert( 0 != m_subsection_count );

	const subsection_desc & desc = *m_subsections.rbegin();
	m_subsections_string.resize( desc.m_prev_length );
	m_subsections.pop_back();

	if( !m_subsections.empty() )
		m_subsection_desc = &*m_subsections.rbegin();
	else
		m_subsection_desc = nullptr;

	return super::end_subsection();
}

const status_t f1::pp::output::linenum::output::write( const char * string, size_t length )
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
					//++ end;						// ... leave a separating space
					++ len;
				}
			}
		}
		else
		{
			if( 0 != len && was_spaces_at_end )
			{
				//++ end;
				++ len;
			}
		}
		m_was_space = was_spaces_at_end;
		string = start;
		length = len;
	}
	if( 0 == length )
		return STATUS_SUCCESS;

	// Update new lines
	if( /*true == m_was_nl || */m_lineno != m_last_lineno )
	{
		m_last_lineno = m_lineno;
		if( 0 == stg->get_newline_flag() )
		{
			stg->write( get_context()->get_current_newline_type_seq() );//"\r\n", 2 );
			++ m_global_lineno;
			stg->set_newline_flag( true );
		}

		// Write preprocessor directive
		//if( '#' != *string )	// uncomment to get `#pragma pack()' instead of `#20:#pragma pack'
		{
			// Write source line
			char buf[128] = "#";
			_ltoa_s( m_lineno, buf + 1, countof(buf) - 1, 10 );
			if( nullptr != m_subsection_desc )
			{
				// Flush previous string
				stg->write( buf, strlen( buf ) );	// TODO: try to avoid call to strlen by using a function other that ltoa*

				// Write subsections
				stg->write( m_subsections_string.c_str(), m_subsections_string.length() );

				_ltoa_s( m_lineno - m_subsection_desc->m_start_line + 1, buf, countof(buf), 10 );
				stg->write( buf, strlen( buf ) );	// TODO: try to avoid call to strlen by using a function other that ltoa*

				// Write separator
				stg->write( ": ", 2 );
			}
			else
			{
				const char * ep = _nstrncatx( buf, countof(buf), ": ", 1 );
				stg->write( buf, ep - buf );
			}
		}
	}

	if( '\n' == *string || '\r' == *string )
		m_last_lineno = 0;
		//m_was_nl = true;

	if( '\n' == string[length - 1] || '\r' == string[length - 1] )
		stg->set_newline_flag( true );
	else
		stg->set_newline_flag( false );

	// Write data
	stg->write( string, length );
	m_global_lineno += get_context()->count_newlines( string, string + length );

	// Exit
	return STATUS_SUCCESS;
}

//static const char file_message_header[] = "#1 \"";
//static const char file_message_footer[] = "\":\r\n";
const status_t f1::pp::output::linenum::output::on_file_changed( const char * filename, f1::parser::lineno_t lineno ) __noexcept
{
	core::Tref<pp::Istorage> stg = m_parser->get_storage();
	__debugbreak_if( nullptr == stg )
		return STATUS_ERROR_NOT_INITIALIZED;
	__assert( nullptr != m_parser->get_context() );

	if( nullcstr == filename )
		return STATUS_SUCCESS;
	__assert( nullptr == m_subsection_desc );

	super::on_file_changed( filename, lineno );
	__assert( nullptr != m_parser->get_context() );

	if( 0 == stg->get_newline_flag() )
	{
		stg->write( get_context()->get_current_newline_type_seq() );	//"\r\n", 2 );
		++ m_global_lineno;
	}
	__assert( nullptr != m_parser->get_context() );

#if 1
	m_buffer.format( "#%d \"%s\":%s", m_lineno, m_filename.c_str(), get_context()->get_current_newline_type_seq() );
	stg->write( m_buffer.c_str(), m_buffer.length() );
	__assert( nullptr != m_parser->get_context() );
#else
	stg->write( file_message_header, countof(file_message_header) - 1 );
	stg->write( m_filename );
	stg->write( file_message_footer, countof(file_message_footer) - 1 );
#endif
	++ m_global_lineno;
	stg->set_newline_flag( true );
	m_was_space = true;
	m_last_lineno = 0;

	__assert( nullptr != m_parser->get_context() );
	__assert( m_subsections.empty() );

	return STATUS_SUCCESS;
}
const status_t f1::pp::output::linenum::output::on_line_changed( f1::parser::lineno_t lineno ) noexcept
{
#if 0
	if( m_last_lineno != lineno )
		super::on_line_changed( lineno );

//	if( !m_was_nl )
//	{
//		core::Tref<pp::Istorage> stg = m_parser->get_storage();
//		__debugbreak_if( nullptr == stg )
//			return STATUS_ERROR_NOT_INITIALIZED;
//		stg->write( get_context()->get_current_newline_type_seq() );	//"\r\n", 2 );
//		m_was_nl = true;
//	}

//	if( m_last_lineno == lineno )
//		-- m_last_lineno;
#else
	super::on_line_changed( lineno );

	if( m_last_lineno == lineno )
		m_last_lineno = 0;
#endif

	return STATUS_SUCCESS;
}

/*END OF output.cxx*/
