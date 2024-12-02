/*parse_float.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::c_pp::parser::parse_float( pp::variant & res, bool & parsed )
{
	const char * s = m_parser->get_ptr();
	const char * const end_s = m_parser->get_end_ptr();	// capture end pointer for the current patch or source
	parsed = parse_float( res, s, end_s );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		m_parser->set_ptr( s );
	return STATUS_SUCCESS;
}
bool f1::c_pp::parser::parse_float( pp::variant & res, const pp::string & buf )
{
	const char * s = buf.data();
	bool result = parse_float( res, s, buf.data_end() );
	if( !result )
		return false;
	if( s != buf.data_end() )
		return false;
	return true;
}
bool f1::c_pp::parser::parse_float( pp::variant & res, const char *& s, const char * end_s )
{
	// Parse value
	const char * ep = nullptr;
	max_float_t value = _strntold( s, end_s - s, &ep );
	if( ep == s )
		return false;

	s = ep;
	res.set_float( value );

	// Exit
	return true;
}

/*END OF parse_float.cxx*/
