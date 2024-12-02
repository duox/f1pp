/*Isuper_pragma.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::Isuper_pragma::Isuper_pragma( const core::Iidentifier * ident ) __noexcept:
	super( ident )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sub-pragmas

const status_t f1::c_pp::Isuper_pragma::append_subpragma( const Ipragma & sub )
{
	return m_subpragma_list.append_unique( &sub );
}
const status_t f1::c_pp::Isuper_pragma::remove_subpragma( const Ipragma & sub )
{
	return m_subpragma_list.remove( &sub );
}
const status_t f1::c_pp::Isuper_pragma::remove_all_subpragmas()
{
	return m_subpragma_list.remove_all();
}
const f1::c_pp::const_pragma_list_t & f1::c_pp::Isuper_pragma::get_subpragma_list() const
{
	return m_subpragma_list;
}
const status_t f1::c_pp::Isuper_pragma::find_subpragma( core::Tconst_ref<Ipragma> & res, const pp::string & name ) const
{
	array_foreach( const_pragma_list_t::const_iterator, it, m_subpragma_list )
	{
		if( name.equal( it->get_name() ) )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}


const status_t f1::c_pp::Isuper_pragma::append_known_subpragma( const char * name, size_t len )
{
	return size_t(-1) == len ?
		m_known_subpragma_list.append_unique( name ):
		m_known_subpragma_list.append_unique( core::string( name, len ) );
}
const status_t f1::c_pp::Isuper_pragma::remove_known_subpragma( const char * name, size_t len )
{
	return size_t(-1) == len ?
		m_known_subpragma_list.remove( name ):
		m_known_subpragma_list.remove( core::string( name, len ) );
}
const status_t f1::c_pp::Isuper_pragma::remove_all_known_subpragmas()
{
	return m_known_subpragma_list.remove_all();
}
const f1::core::const_string_list_t & f1::c_pp::Isuper_pragma::get_known_subpragma_list() const
{
	return m_known_subpragma_list;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::Isuper_pragma::parse( pp::Iparser & parser, const pp::string & param_str ) const
{
	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Get access to parameters
	const char * s = param_str.data();
	const char * const end_s = param_str.data_end();

	// Fetch identifier
	pp::string	ident;
	c_pp_par->skip_real_spaces_and_comments( s, end_s );
	if( c_pp_par->parse_macro_identifier( ident, s, end_s ) )
	{
		// Find supported pragma
		array_foreach( const_pragma_list_t::const_iterator, it, m_subpragma_list )
		{
			if( ident.equal_nocase( it->get_name() ) )
			{
				return it->parse( parser, pp::string( s, end_s ) );
			}
		}

		// Find known pragma
		bool found = false;
		array_foreach( core::const_string_list_t::const_iterator, it, m_known_subpragma_list )
		{
			if( ident.get_stringslice().equal( *it ) )
			{
				found = true;
				break;
			}
		}
		if( !found )	// If pragma wasn't found, issue a warning
			parser.message( syntax::IMSG_WARNING_UNKNOWN_PRAGMA, ident.c_str() );
	}

	// Write out pragma
	core::Tref<pp::Ioutput> out = parser.get_output();
	__debugbreak_if( nullptr == out )
		return STATUS_ERROR_NOT_INITIALIZED;

	if( !c_pp_par->m_new_line )
		out->write( "\n", 1 );

	out->write( "#pragma " );
	out->write( get_name() );
	out->write( " " );
	out->write( ident.get_string().c_str() );
	out->write( " " );
	out->write( s, end_s - s );

	out->write( "\n", 1 );
	c_pp_par->m_new_line = true;

	// Exit
	return STATUS_ERROR_NOT_FOUND;
}

/*END OF Isuper_pragma.cxx*/
