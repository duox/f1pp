/*parser_character.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::f1pp::parser::_esc_control( c_pp::parser & par, pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding )
{
	core::Tref<f1pp::parser> f1par = type_cast<f1pp::parser *>( &par );
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_INTERNAL;
	return f1par->on_esc_control( buf, s, end_s, encoding );
}
const status_t f1::f1pp::parser::on_esc_control( pp::string & buf, const char *& s, const char * end_s, char_encoding_t encoding )
{
	if( s >= end_s )
		return message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );
	if( *s < 'A' || 'Z' < *s )
		return message( syntax::IMSG_ERROR_INVALID_ESCAPE_SEQUENCE );

	char ch = *s - 'A' + 1;
	_write_numeric_with_encoding( buf, encoding, ch );

	return STATUS_SUCCESS;
}

/*END OF parser_character.cxx*/
