/*parser_attribute_list.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::f1pp::parser::parse_attribute( core::Tconst_ref<pp::attribute> & res )
{
	status_t status;

	// Parse inversion flag
	bool invert = m_parser->skip_char( '!' );
	skip_real_spaces_and_comments();

	// Parse name
	pp::string ident;
	if( parse_macro_identifier( ident ).failed_status( status ) || ident.is_empty() )
	{
		message( syntax::IMSG_ERROR_EXPECTED_ATTRIBUTE_NAME );
		return status;
	}
	skip_real_spaces_and_comments();

	// Parse attributes
	core::Tref<pp::arg_list> args;
	if( m_parser->peek_char( '(' ) )
	{
		if( parse_arg_list( args, nullptr ).failed_status( status ) )
			return status;
	}

#if 1
	// Build bit flags
	unsigned flags = 0;
	if( invert )
		flags |= pp::attribute::fl_inverted;

	// Create attribute
	f1::core::Tref<pp::attribute> obj;
	if( obj.createT<pp::attribute>().failed_status( status ) )
		return status;
	if( obj->initialize( ident, flags, args ).failed_status( status ) )
		return status;
#else
	// Find attribute
	core::Tconst_ref<pp::attribute_class> attr_cls;
	if( m_attribute_manager.find_attribute_class( attr_cls, ident, args ).failed_status( status ) )
	{
		if( STATUS_ERROR_NOT_FOUND != status )
			return status;
		if( on_unknown_attribute( res, ident, args ).failed_status( status ) )
			return STATUS_SUCCESS;//status;
		__debugbreak_if( nullptr == res )
			return STATUS_ERROR_INTERNAL;
	}

	// Build bit flags
	unsigned flags = 0;
	if( invert )
	{
		if( 0 != attr_cls->get_caps( pp::attribute_class::cap_can_invert ) )
			flags |= pp::attribute::fl_inverted;
		else
			message( syntax::IMSG_WARNING_CANNOT_INVERT_ATTRIBUTE_S, ident.c_str() );
	}

	// Create attribute
	core::Tref<pp::attribute> obj;
	if( m_attribute_manager.create_attribute( obj, *attr_cls.ref_const_cast(), flags, args ).failed_status( status ) )
		return status;
#endif
	// Exit
	res = obj;
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::parse_attribute_list( core::Tref<pp::attribute_list> & res )
{
	status_t status;

	res = nullptr;

	// Skip open bracket
	skip_real_spaces_and_comments();
	__debugbreak_if( !m_parser->skip_char( '[' ) )
		return STATUS_SUCCESS;	// no attribute list

	// Parse attributes in the list
	for(;;)
	{
		// Check exit condition
		skip_real_spaces_and_comments();
		if( is_eof() )
		{
			message( syntax::IMSG_ERROR_UNTERMINATED_ATTRIBUTE_LIST );
			break;
		}
		if( m_parser->skip_char( ']' ) )
			break;

		// Parse attribute and its parameters
		core::Tref<pp::attribute> attr;
		if( parse_attribute( attr ).failed_status( status ) )
			return status;
		if( nullptr == attr )
			return STATUS_SUCCESS;

		// Append attribute to list
		if( nullptr == res )
		{
			res.createT<pp::attribute_list>();
			res->initialize();
		}
		res->append_attribute( *attr );

		// Iterate
		skip_real_spaces_and_comments();
		if( m_parser->skip_char( ',' ) )
			continue;
		if( m_parser->skip_char( ';' ) )
			continue;
	}

	// Exit
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::f1pp::parser::parse_attribute( core::Tconst_ref<pp::attribute> & res, const char * & s, const char * end_s )
{
	status_t status;

	// Parse inversion flag
	bool invert = false;
	if( '!' == *s )
	{
		++ s;
		invert = true;
	}
	skip_real_spaces_and_comments( s, end_s );

	// Parse name
	pp::string ident;
	if( !parse_macro_identifier( ident, s, end_s ) || ident.is_empty() )
	{
		message( syntax::IMSG_ERROR_EXPECTED_ATTRIBUTE_NAME );
		return STATUS_ERROR_INVALID_SYNTAX;
	}
	skip_real_spaces_and_comments( s, end_s );

	// Parse attributes
	core::Tref<pp::arg_list> args;
	if( '(' == *s )
	{
		++ s;
		if( parse_arg_list( args, nullptr, s, end_s ).failed_status( status ) )
			return status;
	}

#if 1
	// Build bit flags
	unsigned flags = 0;
	if( invert )
		flags |= pp::attribute::fl_inverted;

	// Create attribute
	f1::core::Tref<pp::attribute> obj;
	if( obj.createT<pp::attribute>().failed_status( status ) )
		return status;
	if( obj->initialize( ident, flags, args ).failed_status( status ) )
		return status;
#else
	// Find attribute
	core::Tconst_ref<pp::attribute_class> attr_cls;
	if( m_attribute_manager.find_attribute_class( attr_cls, ident, args ).failed_status( status ) )
	{
		if( STATUS_ERROR_NOT_FOUND != status )
			return status;
		if( on_unknown_attribute( res, ident, args ).failed_status( status ) )
			return STATUS_ERROR_NOT_FOUND != status ? status : STATUS_SUCCESS;
		__debugbreak_if( nullptr == res )
			return STATUS_ERROR_INTERNAL;
	}

	// Build bit flags
	unsigned flags = 0;
	if( invert )
	{
		if( 0 != attr_cls->get_caps( pp::attribute_class::cap_can_invert ) )
			flags |= pp::attribute::fl_inverted;
		else
			message( syntax::IMSG_WARNING_CANNOT_INVERT_ATTRIBUTE_S, ident.c_str() );
	}

	// Create attribute
	core::Tref<pp::attribute> obj;
	if( m_attribute_manager.create_attribute( obj, *attr_cls.ref_const_cast(), flags, args ).failed_status( status ) )
		return status;
#endif
	// Exit
	res = obj;
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::parse_attribute_list( core::Tref<pp::attribute_list> & res, const char * & s, const char * end_s )
{
	status_t status;

	res = nullptr;

	// Skip open bracket
	skip_real_spaces_and_comments( s, end_s );
	__debugbreak_if( '[' != *s )
		return STATUS_SUCCESS;	// no attribute list
	++ s;

	// Parse attributes in the list
	for(;;)
	{
		// Check exit condition
		skip_real_spaces_and_comments( s, end_s );
		if( s >= end_s )
		{
			message( syntax::IMSG_ERROR_UNTERMINATED_ATTRIBUTE_LIST );
			break;
		}
		if( ']' == *s )
		{
			++ s;
			break;
		}

		// Parse attribute and its parameters
		core::Tref<pp::attribute> attr;
		if( parse_attribute( attr, s, end_s ).failed_status( status ) )
			return status;

		// Append attribute to list
		if( nullptr != attr )
		{
			if( nullptr == res )
			{
				res.createT<pp::attribute_list>();
				res->initialize();
			}
			res->append_attribute( *attr );
		}

		// Iterate
		skip_real_spaces_and_comments( s, end_s );
		if( ',' == *s )
			++ s;
		else if( ';' == *s )
			++ s;
	}

	// Exit
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const f1::pp::attribute * f1::f1pp::parser::get_directive_attribute( const char * name, size_t len ) const
{
	if( nullptr == m_directive_attribute_list )
		return nullptr;

	core::Tref<pp::attribute> attr;
	m_directive_attribute_list->find_attribute( attr, pp::string(name, len), nullptr );
	return attr;
}
const status_t f1::f1pp::parser::find_directive_attribute( core::Tconst_ref<f1::pp::attribute> & res, const pp::string & name ) const
{
	if( nullptr == m_directive_attribute_list )
		return STATUS_ERROR_NOT_FOUND;
	return m_directive_attribute_list->find_attribute( res, name, nullptr );
}
const status_t f1::f1pp::parser::find_directive_attribute( core::Tconst_ref<f1::pp::attribute> & res, const char * name, size_t len ) const
{
	if( nullptr == m_directive_attribute_list )
		return STATUS_ERROR_NOT_FOUND;
	return m_directive_attribute_list->find_attribute( res, pp::string(name, len), nullptr );
}
bool f1::f1pp::parser::exists_directive_attribute( const f1::guid_t & guid ) const
{
	if( nullptr == m_directive_attribute_list )
		return false;
	return m_directive_attribute_list->exists_attribute( guid );
}
bool f1::f1pp::parser::exists_directive_attribute( const pp::string & name ) const
{
	if( nullptr == m_directive_attribute_list )
		return false;
	return m_directive_attribute_list->exists_attribute( name );
}
bool f1::f1pp::parser::exists_directive_attribute( const char * name, size_t len ) const
{
	if( nullptr == m_directive_attribute_list )
		return false;
	return m_directive_attribute_list->exists_attribute( pp::string(name, len) );
}

/*const status_t f1::f1pp::parser::on_unknown_attribute( core::Tconst_ref<pp::attribute> & res, const pp::string & ident, const pp::arg_list * args )
{
	res = nullptr;
	message( syntax::IMSG_ERROR_UNKNOWN_ATTRIBUTE_S, ident.c_str() );

	__unreferenced_parameter( args );
	return STATUS_ERROR_NOT_FOUND;
}*/

/*END OF parser_attribute_list.cxx*/
