/*pragma_once.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::comext::pragma_once::pragma_once( const core::Iidentifier * ident ):
	super( ident )
{
}

// Properties
const char * f1::c_pp::comext::pragma_once::get_name() const
{
	return "once";
}
// {2794BF56-606B-4148-9C36-39129AD54CE9}
const f1::guid_t f1::c_pp::comext::pragma_once::m_guid =
{ 0x2794bf56, 0x606b, 0x4148, { 0x9c, 0x36, 0x39, 0x12, 0x9a, 0xd5, 0x4c, 0xe9 } };
const f1::guid_t & f1::c_pp::comext::pragma_once::get_guid() const
{
	return m_guid;
}

// Operations
const status_t f1::c_pp::comext::pragma_once::parse( pp::Iparser & parser, const pp::string & param_str ) const
{
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	if( !param_str.is_empty() )
		return parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );

	c_pp_par->register_once_include();

	return STATUS_SUCCESS;
}

/*END OF pragma_once.cxx*/
