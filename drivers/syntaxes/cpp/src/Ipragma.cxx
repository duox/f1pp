/*Ipragma.cxx*/
#include "pch.h"
#include "main.h"

// {E4A599B2-B012-4310-9C6E-5DDB479EA0B8}
const f1::guid_t f1::c_pp::Ipragma::m_item_class_guid =
{ 0xe4a599b2, 0xb012, 0x4310, { 0x9c, 0x6e, 0x5d, 0xdb, 0x47, 0x9e, 0xa0, 0xb8 } };

f1::c_pp::Ipragma::Ipragma( const core::Iidentifier * ident ) __noexcept:
	super( ident )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::Ipragma::get_name() const
{
	DBG_FAIL_ON_PURE_CALL();
	return nullcstr;
}
const f1::guid_t & f1::c_pp::Ipragma::get_guid() const
{
	DBG_FAIL_ON_PURE_CALL();
	return f1::guid_t::invalid;
}

unsigned f1::c_pp::Ipragma::get_caps( unsigned mask ) const
{
	__unreferenced_parameter( mask );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::Ipragma::parse( pp::Iparser & parser, const pp::string & param_str ) const
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( param_str );
	return STATUS_ERROR_PURE_CALL;
}

/*END OF Ipragma.cxx*/
