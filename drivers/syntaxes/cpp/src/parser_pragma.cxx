/*parser_pragma.cxx*/
#include "pch.h"
#include "main.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Supported pragmas

const status_t f1::c_pp::parser::append_pragma( const Ipragma & pragma )
{
	return m_pragma_list.append_unique( &pragma );
}
const status_t f1::c_pp::parser::append_pragmas( const pragma_list_t & list )
{
	array_foreach( pragma_list_t::const_iterator, it, list )
		append_pragma( **it );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::remove_pragma( const Ipragma & pragma )
{
	return m_pragma_list.remove( &pragma );
}
const status_t f1::c_pp::parser::remove_pragmas( const pragma_list_t & list )
{
	array_foreach( pragma_list_t::const_iterator, it, list )
		remove_pragma( **it );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::parser::remove_all_pragmas()
{
	return m_pragma_list.remove_all();
}

const status_t f1::c_pp::parser::find_pragma( core::Tref<Ipragma> & res, const pp::string & ident, const const_pragma_list_t * pragma_list ) const
{
	if( nullptr == pragma_list )
		pragma_list = &get_pragma_list();
	array_foreach( const_pragma_list_t::const_iterator, it, *pragma_list )
	{
		if( ident.equal( it->get_name() ) )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}

const f1::c_pp::const_pragma_list_t & f1::c_pp::parser::get_pragma_list() const
{
	return m_pragma_list;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Known pragmas

const status_t f1::c_pp::parser::append_known_pragma( const char * name, size_t len )
{
	return size_t(-1) == len ?
		m_known_pragma_list.append_unique( name ):
		m_known_pragma_list.append_unique( core::string( name, len ) );
}
const status_t f1::c_pp::parser::remove_known_pragma( const char * name, size_t len )
{
	return size_t(-1) == len ?
		m_known_pragma_list.remove( name ):
		m_known_pragma_list.remove( core::string( name, len ) );
}
const status_t f1::c_pp::parser::remove_all_known_pragmas()
{
	return m_known_pragma_list.remove_all();
}

const f1::core::const_string_list_t & f1::c_pp::parser::get_known_pragma_list() const
{
	return m_known_pragma_list;
}


/*END OF parser_pragma.cxx*/
