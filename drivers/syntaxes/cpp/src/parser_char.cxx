/*parser_char.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::c_pp::parser::parse_char( max_int_t & val, const char *& s, const char * end_s, const nls::Iencoding * target_encoding )
{
	status_t status;

	// Prepare for parsing
	core::Tconst_ref<nls::Iencoding> enc = nullptr == target_encoding ?
		m_context->get_nls_context().get_default_codepage_encoding():
		target_encoding;
	__debugbreak_if( nullptr == enc )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch all characters to UTF-8 string
	m_char_parse_buffer.clear();
	//size_t sz = 0;
	unsigned char_count = 0;
	while( s < end_s )
	{
		if( '\'' == *s )
			break;
		else if( '\\' == *s )
		{
			++ s;

			// Parse character in the form of multi-byte sequence
			//const char * start = s;
			pp::string buf;
			if( parse_esc_char( buf, s, end_s ).failed_status( status ) )
				return status;
			//sz = s - start;
			m_char_parse_buffer.add_part( buf );
		}
		else
		{
			m_char_parse_buffer.add_part( s, 1 );
			//++ sz;
			++ s;
		}
		char_count ++;
	}

	// Convert to int and exit
	return convert_char_to_integer( val, m_char_parse_buffer, enc );
}
const status_t f1::c_pp::parser::parse_char( max_int_t & val, const nls::Iencoding * target_encoding )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	status_t status = parse_char( val, s, end_s, target_encoding );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}

const status_t f1::c_pp::parser::parse_wchar( max_int_t & ch, const char *& s, const char * end_s )
{
	return parse_char( ch, s, end_s, m_nls_utf16_encoding );
}
const status_t f1::c_pp::parser::parse_wchar( max_int_t & val )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	status_t status = parse_wchar( val, s, end_s );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return status;
}


#if 0
const status_t f1::c_pp::parser::convert_char_to_integer( max_int_t & result, const pp::string & buf, char_encoding_t target_encoding )//const nls::Iencoding * target_encoding )
{
	// Convert to target encoding
	size_t data_size;
	result = 0;
	if( ce_utf8 != target_encoding )
	{
		if( ce_widechar == target_encoding )
			target_encoding = get_c_flags( c_flags_t::fl_long_wchar_t ) ? ce_utf32 : ce_utf16;

		size_t	bytes_passed = 0;
		switch( target_encoding )
		{
		case ce_utf16:{
			unicode::utf16le_string utf16_buf;
			unicode::utf16::convert( utf16_buf, (const unicode::uchar8_t *) buf.data(), buf.size(), nullptr, &bytes_passed );
			data_size = utf16_buf.size();
			__small_memcpy( &result, utf16_buf.data(), __min( data_size, sizeof(result) ) );
			}break;
		case ce_utf32:{
			unicode::utf32le_string utf32_buf;
			unicode::utf32::convert( utf32_buf, (const unicode::uchar8_t *) buf.data(), buf.size(), nullptr, &bytes_passed );
			data_size = utf32_buf.size();
			__small_memcpy( &result, utf32_buf.data(), __min( data_size, sizeof(result) ) );
			}break;
		default:
			DBG_FAIL();
			return STATUS_ERROR_INTERNAL;
		}
		__debugbreak_if( bytes_passed < buf.size() )
			return STATUS_ERROR_INVALID_DATA;
	}
	else
	{
		data_size = buf.size();
		__small_memcpy( &result, buf.data(), __min( data_size, sizeof(result) ) );
	}
	if( data_size > sizeof(result) )
		message( syntax::IMSG_WARNING_CHARACTER_CONSTANT_OVERFLOW );
#else
const status_t f1::c_pp::parser::convert_char_to_integer( max_int_t & result, const pp::string & buf, const nls::Iencoding * target_encoding )
{
	// Convert to target encoding
	const void * data;
	size_t data_size;
	f1::nls::encoding_id_t enc_id;
	if( nullptr != target_encoding )
	{
		core::Tconst_ref<nls::Iencoding> enc = nullptr == target_encoding ?
			m_context->get_nls_context().get_default_codepage_encoding():
			target_encoding;
		__debugbreak_if( nullptr == enc )
			return STATUS_ERROR_NOT_INITIALIZED;

		//m_char_parse_wbuffer.set_data_size( 0 );
		//m_char_parse_wbuffer.write( 0, buf.data(), buf.size() );	// TODO: remove UTF8->UTF16 conversion
		m_char_parse_wbuffer.clear();
		unicode::utf16le::convert( m_char_parse_wbuffer, (const unicode::uchar8_t *) buf.data(), buf.size() );

		m_char_encode_buffer.set_data_size( 0 );
		//enc->encode( m_char_encode_buffer, m_char_parse_wbuffer.get_unicode_ptr(), m_char_parse_wbuffer.get_data_size()*2 );
		enc->encode( m_char_encode_buffer, (const wchar_t *)m_char_parse_wbuffer.data(), m_char_parse_wbuffer.size() );

		enc_id = enc->get_encoding_id();
		data = m_char_encode_buffer.get_mbcs_ptr();
		data_size = m_char_encode_buffer.get_data_size();
	}
	else
	{
		target_encoding = m_nls_utf8_encoding;
		enc_id = f1::nls::eid_mibe_utf8;
		data = buf.data();
		data_size = buf.size();
	}

	// Generate integer constant
	if( data_size > sizeof(result) )
	{
		message( syntax::IMSG_WARNING_CHARACTER_CONSTANT_OVERFLOW );
		data_size = sizeof(result);
	}
	//__small_memcpy( &result, data, data_size );

	const char * s = (const char *) data;
	const char * end_s = s + data_size;
	result = 0;
	for( size_t char_size; s < end_s; s += char_size )
	{
		char_size = target_encoding->get_char_size( s, end_s - s );
		max_int_t val = 0;
		__small_memcpy( &val, s, char_size );
		result = (result << (char_size * 8)) | val;
	}

#endif

	// Warn about multi-character constant
	if( !check_limit_value_min( pp::limit_t::li_maximum_charconst_length, data_size ) )
		message( syntax::IMSG_WARNING_TOO_LONG_CHARACTER_CONSTANT );

	// Convert to signed constant if needed
	if( !get_c_flags( c_flags_t::fl_unsigned_char ) )
	{
		const max_int_t or_mask = max_int_t(-1) << (data_size * 8);
		const max_int_t and_mask = max_int_t(1) << ((data_size * 8) - 1);
		if( result & and_mask )
			result |= or_mask;
	}

	// Swap bytes if endianness differs
	if( 0 != result )
	{
		if( (_is_little_endian_machine() && m_context->get_flags( pp::context::fl_big_endian )) ||
			(_is_big_endian_machine() && m_context->get_flags( pp::context::fl_little_endian )) )
		{
			result = core::Tswap_bytes( result );
		}
	}

	// Exit
	return STATUS_SUCCESS;
}
//const status_t f1::c_pp::parser::convert_char_to_variant( pp::variant & result, const pp::string & buf, char_encoding_t target_encoding )//const nls::Iencoding * target_encoding )
const status_t f1::c_pp::parser::convert_char_to_variant( pp::variant & result, const pp::string & buf, const nls::Iencoding * target_encoding )
{
	status_t status;

	max_int_t int_val;
	if( convert_char_to_integer( int_val, buf, target_encoding ).failed_status( status ) )
		return status;

	status = get_c_flags( c_flags_t::fl_unsigned_char ) ?
		result.set_uint( int_val ):
		result.set_int( int_val );

	return status;
}

/*END OF parser_char.cxx*/
