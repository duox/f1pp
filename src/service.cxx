/*service.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::service::service() __noexcept:
#ifdef _DEBUG
	m_flags( f1::pp::service::fl_debug_output )
#else
	m_flags( 0 )
#endif
{
}
f1::pp::service::~service() __noexcept
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::service::preinitialize()
{
	return super::preinitialize();
}
const status_t f1::pp::service::deinitialize() noexcept
{
	return super::deinitialize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// {50CB3423-0C6B-46cb-9347-1BFA1AEEAF83}
const f1::guid_t f1::pp::service::m_guid =
{ 0x50cb3423, 0xc6b, 0x46cb, { 0x93, 0x47, 0x1b, 0xfa, 0x1a, 0xee, 0xaf, 0x83 } };

const f1::guid_t & f1::pp::service::get_guid() const __noexcept
{
	return m_guid;
}
const status_t f1::pp::service::get_version( f1::core::version & ver ) const __noexcept
{
	ver.m_major = VERSION_MAJOR;
	ver.m_minor = VERSION_MINOR;
	ver.m_build = VERSION_BUILD;
	ver.m_sub	= VERSION_SUBVERSION;

	ver.m_flags = VERSION_FLAGS;
#ifdef _DEBUG
	ver.m_flags |= core::version::fl_debug_build;
#endif // def _DEBUG

	ver.m_start_year = 2022;

	return STATUS_SUCCESS;
}

const status_t f1::pp::service::modify_flags( unsigned flags_to_set, unsigned flags_to_clear, unsigned * flags_prev ) noexcept
{
	m_flags.modify( flags_to_set, flags_to_clear, flags_prev );
	return STATUS_SUCCESS;
}
unsigned f1::pp::service::get_flags( unsigned mask ) const noexcept
{
	return m_flags & mask;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Syntaxes

const status_t f1::pp::service::append_syntax( pp::Isyntax & syntax ) __noexcept
{
	core::Tref<pp::Isyntax> existing_syntax;
	__debugbreak_if( get_syntax( existing_syntax, syntax.get_guid() ).succeeded() )
		return STATUS_ERROR_ALREADY_EXISTS;

	status_t status = m_syntaxes.append_unique( &syntax );
	if( status.failed() )
		return status;

	return append_standard( syntax );
}
const status_t f1::pp::service::remove_syntax( pp::Isyntax & syntax ) __noexcept
{
	status_t status = m_syntaxes.remove( &syntax );
	if( status.failed() )
		return status;

	if( &syntax == m_default_syntax )
		m_default_syntax = nullptr;

	remove_standard( syntax );

	return STATUS_SUCCESS;
}
const status_t f1::pp::service::remove_all_syntaxes() __noexcept
{
	array_foreach( syntax_list_t::iterator, it, m_syntaxes )
		remove_standard( **it );
	m_default_syntax = nullptr;
	return m_syntaxes.remove_all();
}

const f1::pp::Isyntax * f1::pp::service::get_default_syntax() const
{
	if( nullptr == m_default_syntax )
		return m_syntaxes.empty() ? nullptr : *m_syntaxes.rbegin();
	return m_default_syntax;
}
const status_t f1::pp::service::set_default_syntax( pp::Isyntax * syntax )
{
	m_default_syntax = syntax;
	return STATUS_SUCCESS;
}

const status_t f1::pp::service::get_syntax( core::Tconst_ref<pp::Isyntax> & res, const f1::guid_t & guid ) const
{
	array_foreach( syntax_list_t::const_iterator, it, m_syntaxes )
	{
		if( it->get_guid() == guid )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}


const f1::pp::syntax_list_t & f1::pp::service::get_syntax_list() const __noexcept
{
	return m_syntaxes;
}
f1::pp::syntax_list_t & f1::pp::service::get_syntax_list() __noexcept
{
	return m_syntaxes;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Standards

const status_t f1::pp::service::append_standard( pp::Istandard & standard ) __noexcept
{
	return m_standards.append_unique( &standard );
}
const status_t f1::pp::service::remove_standard( pp::Istandard & standard ) __noexcept
{
	status_t status = m_standards.remove( &standard );
	if( status.failed() )
		return status;

	if( &standard == m_default_standard )
		m_default_standard = nullptr;

	return STATUS_SUCCESS;
}
const status_t f1::pp::service::remove_all_standards() __noexcept
{
	m_default_standard = nullptr;
	return m_standards.remove_all();
}

const f1::pp::Istandard * f1::pp::service::get_default_standard() const
{
	if( nullptr == m_default_standard )
		return m_standards.empty() ? nullptr : *m_standards.rbegin();
	return m_default_standard;
}
const status_t f1::pp::service::set_default_standard( pp::Istandard * standard )
{
	m_default_standard = standard;
	return STATUS_SUCCESS;
}

const status_t f1::pp::service::get_standard( core::Tconst_ref<pp::Istandard> & res, const f1::guid_t & guid ) const
{
	array_foreach( standard_list_t::const_iterator, it, m_standards )
	{
		if( it->get_guid() == guid )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}
const status_t f1::pp::service::get_standard( core::Tconst_ref<pp::Istandard> & res, const char * name, ptrdiff_t len ) const
{
	if( -1 == len )
		len = core::string::length( name );
	array_foreach( standard_list_t::const_iterator, it, m_standards )
	{
		if( core::string::equal_nocase( it->get_ident_string(), name, len ) )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}

const f1::pp::standard_list_t & f1::pp::service::get_standard_list() const __noexcept
{
	return m_standards;
}
f1::pp::standard_list_t & f1::pp::service::get_standard_list() __noexcept
{
	return m_standards;
}

const f1::pp::Istandard * f1::pp::service::find_item_standard( const f1::guid_t & item_class_guid, const pp::string & name ) const
{
	array_foreach( standard_list_t::const_iterator, it, m_standards )
	{
		if( it->is_item_supported( item_class_guid, name ) )
			return *it;
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Output classes

const status_t f1::pp::service::create_output( core::Tref<pp::Ioutput> & res, const f1::guid_t & output_class_guid, /*Ipp::context & ctx, */const core::Iidentifier * ident ) __noexcept
{
	status_t status;

	// Find output class
	core::Tref<Ioutput_class> output_class;
	if( get_output_class( output_class, output_class_guid ).failed_status( status ) )
		return status;

	// Exit
	return create_output( res, *output_class, /*ctx, */ident );
}
const status_t f1::pp::service::create_output( core::Tref<pp::Ioutput> & res, pp::Ioutput_class & output_class, /*pp::context & ctx, */const core::Iidentifier * ident ) __noexcept
{
	status_t status;

	core::Tref<pp::Ioutput> output;
	if( output_class.new_output( output, ident ).failed_status( status ) )
		return status;
	if( output->initialize().failed_status( status ) )
		return status;

	res = output;
	return STATUS_SUCCESS;
}

const status_t f1::pp::service::append_output_class( pp::Ioutput_class & output_class ) __noexcept
{
	return m_output_classes.append_unique( &output_class );
}
const status_t f1::pp::service::remove_output_class( pp::Ioutput_class & output_class ) __noexcept
{
	status_t status = m_output_classes.remove( &output_class );
	if( status.failed() )
		return status;

	if( &output_class == m_default_output_class )
		m_default_output_class = nullptr;

	return STATUS_SUCCESS;
}
const status_t f1::pp::service::remove_all_output_classes() __noexcept
{
	m_default_output_class = nullptr;
	return m_output_classes.remove_all();
}

const f1::pp::Ioutput_class * f1::pp::service::get_default_output_class() const
{
	if( nullptr == m_default_output_class )
		return m_output_classes.empty() ? nullptr : *m_output_classes.rbegin();
	return m_default_output_class;
}
const status_t f1::pp::service::set_default_output_class( pp::Ioutput_class * output_class )
{
	m_default_output_class = output_class;
	return STATUS_SUCCESS;
}

const status_t f1::pp::service::get_output_class( core::Tconst_ref<pp::Ioutput_class> & res, const f1::guid_t & guid ) const
{
	array_foreach( output_class_list_t::const_iterator, it, m_output_classes )
	{
		if( it->get_guid() == guid )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}

const f1::pp::output_class_list_t & f1::pp::service::get_output_class_list() const __noexcept
{
	return m_output_classes;
}
f1::pp::output_class_list_t & f1::pp::service::get_output_class_list() __noexcept
{
	return m_output_classes;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Storage classes

const status_t f1::pp::service::create_storage( core::Tref<pp::Istorage> & res,
	const f1::guid_t & storage_class_guid, pp::context & ctx,
	const core::string & output_filename, Istorage::storage_flags_t flags,
	const core::Iidentifier * ident ) __noexcept
{
	status_t status;

	// Find storage class
	core::Tref<Istorage_class> storage_class;
	if( get_storage_class( storage_class, storage_class_guid ).failed_status( status ) )
	{
		if( nullptr != m_default_storage_class && storage_class_guid == m_default_storage_class->get_guid() )
			storage_class = m_default_storage_class;
		else if( storage_class_guid == m_file_storage_class.get_guid() )
			storage_class = &m_file_storage_class;
		else
			return status;
	}

	// Exit
	return create_storage( res, *storage_class, ctx, output_filename, flags, ident );
}
const status_t f1::pp::service::create_storage( core::Tref<pp::Istorage> & res,
	pp::Istorage_class & storage_class, pp::context & ctx,
	const core::string & output_filename, Istorage::storage_flags_t flags,
	const core::Iidentifier * ident ) __noexcept
{
	status_t status;

	core::Tref<pp::Istorage> storage;
	if( storage_class.new_storage( storage, ident ).failed_status( status ) )
		return status;
	if( storage->initialize( ctx, output_filename, flags ).failed_status( status ) )
		return status;

	res = storage;
	return STATUS_SUCCESS;
}

const status_t f1::pp::service::append_storage_class( pp::Istorage_class & storage_class ) __noexcept
{
	return m_storage_classes.append_unique( &storage_class );
}
const status_t f1::pp::service::remove_storage_class( pp::Istorage_class & storage_class ) __noexcept
{
	status_t status = m_storage_classes.remove( &storage_class );
	if( status.failed() )
		return status;

	if( &storage_class == m_default_storage_class )
		m_default_storage_class = &m_file_storage_class;

	return STATUS_SUCCESS;
}
const status_t f1::pp::service::remove_all_storage_classes() __noexcept
{
	m_default_storage_class = &m_file_storage_class;
	return m_storage_classes.remove_all();
}

const f1::pp::Istorage_class * f1::pp::service::get_default_storage_class() const
{
	if( nullptr == m_default_storage_class )
		return &m_file_storage_class;
	return m_default_storage_class;
}
const status_t f1::pp::service::set_default_storage_class( pp::Istorage_class * storage_class )
{
	m_default_storage_class = storage_class;
	return STATUS_SUCCESS;
}

const status_t f1::pp::service::get_storage_class( core::Tconst_ref<pp::Istorage_class> & res, const f1::guid_t & guid ) const
{
	array_foreach( storage_class_list_t::const_iterator, it, m_storage_classes )
	{
		if( it->get_guid() == guid )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}

const f1::pp::storage_class_list_t & f1::pp::service::get_storage_class_list() const __noexcept
{
	return m_storage_classes;
}
f1::pp::storage_class_list_t & f1::pp::service::get_storage_class_list() __noexcept
{
	return m_storage_classes;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper functions

const status_t f1::pp::service::compact_spaces( pp::string & res, const char * str, size_t len )
{
	const char * s = str;
	const char * end_s = s + len;

	// Skip all heading spaces
	for( ; s < end_s && isspace( *s ); ++ s )
		;
	const char * first_nonspace = s;

	// Skip all trailing spaces
	for( ; end_s > s && isspace( end_s[-1] ); -- end_s )
		;

	// Now try to gather entire string as a whole (to avoid allocations)
	bool was_space = false;
	for( ; s < end_s; ++ s )
	{
		if( isspace( *s ) )
		{
			was_space = true;
			if( '\x20' != *s || was_space )	// leave loop if non-space or sequential whitespace are met
				break;
			continue;
		}
		was_space = false;
	}
	res.add_part( first_nonspace, s - first_nonspace );

	// Check whether everything is completed
	if( s >= end_s )
		return STATUS_SUCCESS;

	// Default variant
	const char * start = s;
	for( ; s < end_s; ++ s )
	{
		// Skip spaces
		if( isspace( *s ) )
		{
			if( '\x20' != *s || isspace( s[1] ) )	// note that a whitespace cannot be the last character
			{
				// Store previous part
				if( was_space || (s != start && __verify( nullptr != start )) )
				{
					if( '\x20' != *s )
					{
						res.add_part( start, s - start );
						res.add_part( "\x20", 1 );
					}
					else
						res.add_part( start, s - start + 1 );
				}
				start = nullptr;

				// Skip extra spaces
				while( s + 1 < end_s && isspace( s[1] ) )
					++ s;

				// Continue loop
				continue;
			}
		}

		if( nullptr == start )
			start = s;
	}
	if( nullptr != start )
		res.add_part( start, s - start );

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::pp::service::trim_spaces( pp::string & str, bool * was_space_in_the_end )
{
	status_t status;

	const char * start = str.data();
	const char * s = str.data();
	size_t len = str.length();
	if( trim_spaces( s, len, was_space_in_the_end ).failed_status( status ) )
		return status;

	str.inc_ptr( s - start );
	str.erase_last( str.length() - len );

	return STATUS_SUCCESS;
}
const status_t f1::pp::service::trim_spaces( const pp::string & str, size_t & new_offset, size_t & new_len, bool * was_space_in_the_end )
{
	status_t status;

	const char * start = str.data();
	const char * s = str.data();
	size_t len = str.length();
	if( trim_spaces( s, len, was_space_in_the_end ).failed_status( status ) )
		return status;
	new_offset = s - start;
	new_len = len;

	return STATUS_SUCCESS;
}
const status_t f1::pp::service::trim_spaces( const char * & str, size_t & new_len, bool * was_space_in_the_end )
{
	if( 0 == new_len )
		return STATUS_SUCCESS;
	const char * start = str;
	const char * end = start + new_len - 1;
	const char * const end_str = end;

	if( nullptr != was_space_in_the_end )
		*was_space_in_the_end = false;

	// Remove leading spaces
	if( isspace( *start ) )
	{
		while( start < end && isspace( *start ) )
			++ start;
	}

	// Remove trailing spaces
	if( isspace( *end ) )
	{
		while( start <= end && isspace( *end ) )
			-- end;

		if( nullptr != was_space_in_the_end )
			*was_space_in_the_end = end_str != end;
	}

	// Done
	str = start;
	new_len = end - start + 1;

	// Exit
	return STATUS_SUCCESS;
}

f1::core::md5hash & f1::pp::service::get_hash_function()
{
	return m_hash_function;
}
const status_t f1::pp::service::calc_blob_hash( core::md5hash::digest_t & res, const void * data, size_t size )
{
	__debugbreak_if( nullptr == data )
		return STATUS_ERROR_INVALID_PARAMETER;

	core::md5hash::calc_blob_hash( res, data, size );

	return STATUS_SUCCESS;
}

/*END OF service.cxx*/
