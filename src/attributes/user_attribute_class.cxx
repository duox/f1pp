/*user_attribute_class.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::user_attribute_class::user_attribute_class( const core::Iidentifier * ident ):
	super( ident )
{
}
const status_t f1::pp::user_attribute_class::initialize( const f1::guid_t & guid, const pp::string & name )
{
	status_t status;

	if( super::initialize().failed_status( status ) )
		return status;

	m_guid = guid;
	m_name.add_part_alloc( name );

	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const f1::guid_t & __nodiscard f1::pp::user_attribute_class::get_guid() const
{
	return m_guid;
}
const f1::pp::string & __nodiscard f1::pp::user_attribute_class::get_name() const
{
	return m_name;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::user_attribute_class::new_attribute( core::Tref<pp::Iattribute> & res, const core::Iidentifier * ident )
{
	return res.createT<pp::user_attribute>( ident );
}

/*END OF user_attribute_class.cxx*/
