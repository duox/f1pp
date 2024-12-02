/*option_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::option_directive::option_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::option_directive::get_name() const
{
	return "option";
}
// {2F5B391A-740E-4F30-A4C9-DE70CE4671F3}
const f1::guid_t f1::f1pp::option_directive::m_guid =
{ 0x2f5b391a, 0x740e, 0x4f30, { 0xa4, 0xc9, 0xde, 0x70, 0xce, 0x46, 0x71, 0xf3 } };
const f1::guid_t & f1::f1pp::option_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::option_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::f1pp::option_directive::parse_target_name( f1pp::parser & parser, pp::string & buf )
{
	parser.parse_definition_identifier( buf );
	if( buf.empty() )
		parser.parse_string( buf );
	return STATUS_SUCCESS;
}
bool f1::f1pp::option_directive::exists_target( f1pp::parser & parser, const pp::string & ident )
{
	core::Tref<pp::Ioption> opt;
	return parser.get_context()->find_option( opt, ident.data(), ident.length() ).succeeded();
}
const status_t f1::f1pp::option_directive::create_target( f1pp::parser & parser, const pp::string & ident, const pp::variant & value )
{
	status_t status;
	if( parser.get_context()->create_option( m_option, value.get_type(), f1::guid_t::null, ident.data(), ident.length() ).failed_status( status ) )
		return status;
	return m_option->set_value( value );
}
bool f1::f1pp::option_directive::target_created() const
{
	return nullptr != m_option;
}
bool f1::f1pp::option_directive::can_modify_target() const
{
	return nullptr != m_option;
}
const status_t f1::f1pp::option_directive::get_target_value( pp::variant & value, pp::variant::type_t ty )
{
	value = m_option->get_value();
	if( pp::variant::ty_none != ty && value.get_type() != ty )
		value.set_type( ty );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::option_directive::set_target_value( const pp::variant & value )
{
	return m_option->set_value( value );
}
const status_t f1::f1pp::option_directive::operator_complete( f1pp::parser & parser )
{
	if( parser.get_parser()->skip_char( ';' ) )
		return parse( parser );
	return super::operator_complete( parser );
}

const status_t f1::f1pp::option_directive::push_all_items( f1pp::parser & parser )
{
	return parser.get_context()->push_all_options();
}
const status_t f1::f1pp::option_directive::pop_items( f1pp::parser & parser )
{
	return parser.get_context()->pop_options();
}
const status_t f1::f1pp::option_directive::push_items( f1pp::parser & parser, const core::Tarray<pp::string> & ident_list )
{
	return parser.get_context()->push_options( ident_list );
}
const status_t f1::f1pp::option_directive::delete_items( f1pp::parser & parser, const core::Tarray<pp::string> & ident_list )
{
	core::Tconst_object_array<pp::Ioption> option_list;
	if( parser.get_context()->find_options( option_list, ident_list ).failed() )
		;//parser.message( syntax::IMSG_ERROR_OPTION_NOT_FOUND, "" );
	else
		parser.get_context()->remove_options( option_list );
	return STATUS_SUCCESS;
}

/*END OF option_directive.cxx*/
