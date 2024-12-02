/*undef_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::c99::undef_directive::undef_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::c99::undef_directive::get_name() const
{
	return "undef";
}
// {66AD55B4-913C-42a6-A359-6A13F8B63E40}
const f1::guid_t f1::c_pp::c99::undef_directive::m_guid =
{ 0x66ad55b4, 0x913c, 0x42a6, { 0xa3, 0x59, 0x6a, 0x13, 0xf8, 0xb6, 0x3e, 0x40 } };
const f1::guid_t & f1::c_pp::c99::undef_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::c99::undef_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::c99::undef_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch macro identifier
	pp::string ident;
	c_pp_par->skip_real_spaces_and_comments();
	if( c_pp_par->parse_macro_identifier( ident ).failed_status( status ) )
		return status;

	// Exit
	return undefine_macro( parser, ident );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hooks

const status_t f1::c_pp::c99::undef_directive::undefine_macro( pp::Iparser & parser, const pp::string & ident, bool force )
{
	status_t status;

	// Check current state
	if( ident.is_empty() )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );

	// Undefine macro
	core::Tref<pp::Imacro> mac = nullptr;
	core::Tref<ns::name> nm = nullptr;
	parser.find_macro( mac, ident, null, &nm );	// TODO: need to specify argument list
	if( nullptr == mac )
		parser.message( syntax::IMSG_WARNING_UNDEFINED_IDENTIFIER_IN_UNDEF, ident.c_str() );
	else
	{
		mac->on_macro_referenced();
		if( !force )
		{
			if( mac->get_flags( pp::Imacro::macro_flags_t::fl_immutable ) )
				return parser.message( syntax::IMSG_ERROR_MACRO_IS_IMMUTABLE );
			if( mac->get_flags( pp::Imacro::macro_flags_t::fl_disabled ) )
				return parser.message( syntax::IMSG_WARNING_UNDEFINED_IDENTIFIER_IN_UNDEF, ident.c_str() );
		}

		if( !mac->can_undefine() )
			mac->modify_flags( pp::Imacro::macro_flags_t::fl_undefined );
		else
		{
			core::Tref<ns::named_object> nobj = nm->get_object();
			nobj->remove_name( *nm );
			/*if( nobj->get_name_list().empty() )	// TODO: not needed
			{
				if( parser.remove_macro( *mac ).failed_status( status ) )
					return status;
			}*/
		}
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF undef_directive.cxx*/
