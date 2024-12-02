/*parser_label.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::f1pp::parser::create_label( core::Tref<f1pp::label> & res, const pp::string & name )
{
	status_t status;

	core::Tref<f1pp::label> obj;
	if( obj.createT<f1pp::label>().failed_status( status ) )
		return status;
	if( obj->initialize( *this, name ).failed_status( status ) )
		return status;

	m_label_list.append( obj );

	res = obj;
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::find_label( core::Tconst_ref<label> & res, const pp::string & name ) const
{
#if 1
	core::Tref<ns::name> nm;
	get_ns_context().find_name( nm, name );
	if( nullptr == nm )
		return STATUS_ERROR_NOT_FOUND;

	core::Tref<f1pp::label> lab = type_cast<f1pp::label *>( nm->get_object() );
	if( nullptr == lab )
		return STATUS_ERROR_INVALID_TYPE;

	res = lab;
	return STATUS_SUCCESS;
#else
	array_foreach( const_label_list_t::const_iterator, it, m_label_list )
	{
		if( it->get_name().equal( name ) )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	return STATUS_ERROR_NOT_FOUND;
#endif
}
const status_t f1::f1pp::parser::wait_for_label( const pp::string & name )
{
	m_delayed_label = name;
	set_allow_exec( false );	// this is a forward label, wait for it to come

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::arrived_to_delayed_label( f1pp::label & lab )
{
	m_delayed_label.clear();
	set_allow_exec( true );

	__unreferenced_parameter( lab );
	return STATUS_SUCCESS;
}
const f1::pp::string & f1::f1pp::parser::get_delayed_label_name() const
{
	return m_delayed_label;
}

/*END OF parser_label.cxx*/
