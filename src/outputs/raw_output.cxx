/*raw_output.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::raw_output_class::raw_output_class( const core::Iidentifier * ident ) __noexcept:
	super( ident )
{
}

// {9E3EAC83-EAD5-4148-904C-FB2B1A7B55A4}
const f1::guid_t f1::pp::raw_output_class::m_guid = 
{ 0x9e3eac83, 0xead5, 0x4148, { 0x90, 0x4c, 0xfb, 0x2b, 0x1a, 0x7b, 0x55, 0xa4 } };
const f1::guid_t & f1::pp::raw_output_class::get_guid() const __noexcept
{
	return m_guid;
}

const status_t f1::pp::raw_output_class::new_output( core::Tref<pp::Ioutput> & res, const core::Iidentifier * ident ) __noexcept
{
	return res.createT<raw_output>( ident );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

f1::pp::raw_output::raw_output( const core::Iidentifier * ident ) __noexcept:
	super( ident )
{
}

const status_t f1::pp::raw_output::write( const char * string, size_t len ) __noexcept
{
	core::Tref<pp::Istorage> stg = m_parser->get_storage();
	__debugbreak_if( nullptr == stg )
		return STATUS_ERROR_NOT_INITIALIZED;

	return stg->write( string, len );
}

const status_t f1::pp::raw_output::on_line_changed( f1::parser::lineno_t lineno ) noexcept
{
	/*if( lineno > m_lineno )
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
	}*/

	return super::on_line_changed( lineno );
}

/*END OF raw_output.cxx*/
