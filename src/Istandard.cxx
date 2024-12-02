/*Istandard.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::Istandard::Istandard( const core::Iidentifier * ident ):
	super( ident )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const f1::guid_t & f1::pp::Istandard::get_guid() const
{
	DBG_FAIL_ON_PURE_CALL();
	return f1::guid_t::invalid;
}
const char * f1::pp::Istandard::get_ident_string() const
{
	DBG_FAIL_ON_PURE_CALL();
	return nullptr;
}
const char * f1::pp::Istandard::get_description_string() const
{
	DBG_FAIL_ON_PURE_CALL();
	return nullptr;
}

const f1::guid_t * f1::pp::Istandard::get_base_guid_list( __out size_t & count ) const
{
	count = 0;
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hierarchy

/*const status_t f1::pp::Istandard::append_base_standard( pp::Istandard & standard )
{
	status_t status;

	array_foreach( standard_list_t::const_iterator, it, m_base_standard_list )
	{
		if( it->get_guid() == standard.get_guid() )
			return STATUS_ERROR_ALREADY_EXISTS;
	}

	if( m_base_standard_list.append_unique( &standard ).failed_status( status ) )
		return status;

	standard.on_append_derived_standard( *this );

	return STATUS_SUCCESS;
}
const status_t f1::pp::Istandard::remove_base_standard( pp::Istandard & standard )
{
	status_t status;

	if( m_base_standard_list.remove( &standard ).failed_status( status ) )
		return status;

	standard.on_remove_derived_standard( *this );

	return STATUS_SUCCESS;
}
const status_t f1::pp::Istandard::remove_all_base_standards()
{
	status_t status;

	if( m_base_standard_list.empty() )
		return STATUS_SUCCESS;

	array_foreach( standard_list_t::const_iterator, it, m_base_standard_list )
	{
		it->on_remove_derived_standard( *this );
	}

	if( m_base_standard_list.remove_all().failed_status( status ) )
		return status;

	return STATUS_SUCCESS;
}
const f1::pp::standard_list_t & f1::pp::Istandard::get_base_standard_list() const
{
	return m_base_standard_list;
}

const status_t f1::pp::Istandard::on_append_derived_standard( pp::Istandard & standard )
{
	return m_derived_standard_list.append_unique( &standard );
}
const status_t f1::pp::Istandard::on_remove_derived_standard( pp::Istandard & standard )
{
	return m_derived_standard_list.remove( &standard );
}
const status_t f1::pp::Istandard::on_remove_all_derived_standards()
{
	return m_derived_standard_list.remove_all();
}
const f1::pp::standard_list_t & f1::pp::Istandard::get_derived_standard_list() const
{
	return m_derived_standard_list;
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

static void _detach_from_parser( f1::pp::Iparser & parser, const f1::guid_t * base_guid, size_t index )
{
	f1::core::Tref<f1::pp::Istandard> standard;
	for( ssize_t i = index; i >= 0; -- i )
	{
		if( f1::pp::service::get().get_standard( standard, base_guid[i] ).succeeded() )
		{
			__debugbreak_if( nullptr == standard )
				continue;
			standard->detach_from_parser( parser );
		}
	}
}

const status_t f1::pp::Istandard::attach_to_parser( pp::Iparser & parser ) const
{
	if( parser.is_standard_attached( *this ) )
		return STATUS_SUCCESS;

//	array_foreach( pp::standard_list_t::const_iterator, it, m_base_standard_list )
//		it->on_attach_to_parser( parser );

	// Attach all base standards
	size_t count = 0;
	const f1::guid_t * base_guid = get_base_guid_list( count );
	__assert( ( nullptr != base_guid && 0 != count ) || ( nullptr == base_guid && 0 == count ) );
	if( nullptr != base_guid && 0 != count )
	{
		size_t i;
		status_t status;
		core::Tref<pp::Istandard> standard;
		for( i = 0; i < count; ++ i )
		{
			if( pp::service::get().get_standard( standard, base_guid[i] ).failed_status( status ) )
				break;
			__debugbreak_if( nullptr == standard )
			{
				status = STATUS_ERROR_INTERNAL;
				break;
			}

			standard->attach_to_parser( parser );
		}
		if( i < count )
		{
			if( 0 != i )
				_detach_from_parser( parser, base_guid, i - 1 );
			return status;
		}
	}

	// Notify after everything is prepared
	parser.attach_standard( *this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::pp::Istandard::detach_from_parser( pp::Iparser & parser ) const
{
	parser.detach_standard( *this );

	size_t count = 0;
	const f1::guid_t * base_guid = get_base_guid_list( count );
	__assert( ( nullptr != base_guid && 0 != count ) || ( nullptr == base_guid && 0 == count ) );
	if( nullptr != base_guid && 0 != count )
	{
		_detach_from_parser( parser, base_guid, count - 1 );
	}
//	array_foreach( pp::standard_list_t::const_reverse_iterator, it, m_base_standard_list )
//		parser.on_detach_from_parser( **it );

	return STATUS_SUCCESS;
}

const status_t f1::pp::Istandard::on_attach_to_parser( pp::Iparser & parser )
{
	if( service::get().get_flags( service::fl_debug_output ) )
	{
		DBG_PRINTF(("Attaching to parser: %s\n", get_ident_string() ));
	}
	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}
const status_t f1::pp::Istandard::on_detach_from_parser( pp::Iparser & parser )
{
	if( service::get().get_flags( service::fl_debug_output ) )
	{
		DBG_PRINTF(("Detaching from parser: %s\n", get_ident_string() ));
	}
	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}

bool f1::pp::Istandard::is_item_supported( const f1::guid_t & item_class_guid, const pp::string & name ) const
{
	__unreferenced_parameter( item_class_guid );
	__unreferenced_parameter( name );
	return false;
}

const status_t f1::pp::Istandard::process_command_line_switch( int switchar, const core::static_string & name, const core::static_string & param )
{
	__unreferenced_parameter( switchar );
	__unreferenced_parameter( name );
	__unreferenced_parameter( param );
	return STATUS_INFORMATION_NOT_PROCESSED;
}
const status_t f1::pp::Istandard::process_configuration_param( const core::static_string & name, const core::static_string & param )
{
	__unreferenced_parameter( name );
	__unreferenced_parameter( param );
	return STATUS_INFORMATION_NOT_PROCESSED;
}

/*END OF Istandard.cxx*/
