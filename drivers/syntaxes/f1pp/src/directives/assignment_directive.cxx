/*assignment_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::assignment_directive::assignment_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::assignment_directive::get_name() const
{
	return "=";
}

// {183F41FA-A56B-4393-A893-E14B002BF865}
const f1::guid_t f1::f1pp::assignment_directive::m_guid =
{ 0x183f41fa, 0xa56b, 0x4393, { 0xa8, 0x93, 0xe1, 0x4b, 0x00, 0x2b, 0xf8, 0x65 } };
const f1::guid_t & f1::f1pp::assignment_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::assignment_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::f1pp::assignment_directive::parse_target_name( f1pp::parser & parser, pp::string & buf )
{
	return parser.parse_macro_identifier( buf );
}
bool f1::f1pp::assignment_directive::exists_target( f1pp::parser & parser, const pp::string & ident )
{
	core::Tref<pp::Imacro> mac;
	bool exists = parser.find_macro( mac, ident.get_stringslice() ).succeeded();
	m_macro = mac;
	return exists;
}
const status_t f1::f1pp::assignment_directive::create_target( f1pp::parser & parser, const pp::string & ident, const pp::variant & value )
{
	status_t status;

	// Gather macro attributes
	pp::Imacro::macro_flags_t macro_flags = null;
	if( parser.exists_directive_attribute( "deprecated" ) )
		macro_flags |= pp::Imacro::macro_flags_t::fl_deprecated;
	if( parser.exists_directive_attribute( "disabled" ) )
		macro_flags |= pp::Imacro::macro_flags_t::fl_disabled;
	if( parser.exists_directive_attribute( "protected" ) )
		macro_flags |= pp::Imacro::macro_flags_t::fl_immutable;
	if( parser.exists_directive_attribute( "weak" ) )
		macro_flags |= pp::Imacro::macro_flags_t::fl_weak;

	f1pp::macro::ex_flags_t macro_ex_flags = null;
	if( parser.exists_directive_attribute( "const" ) )
		macro_ex_flags |= f1pp::macro::ex_flags_t::fl_const;

	unsigned def_flags = 0;
	if( parser.exists_directive_attribute( "global" ) )
		def_flags |= Idefinition_frame::fl_def_global;
	if( parser.exists_directive_attribute( "local" ) )
		def_flags |= Idefinition_frame::fl_def_local;
	if( parser.exists_directive_attribute( "public" ) )
		def_flags |= Idefinition_frame::fl_def_public;

	// Get correct namespace
	core::Tref<ns::name_space> previous_ns = parser.get_ns_context().get_current_namespace();
	if( def_flags & macro_directive::frame::fl_def_global )
		parser.get_ns_context().set_current_namespace( &parser.get_ns_context().get_root_namespace() );
	else if( def_flags & macro_directive::frame::fl_def_public )
		parser.get_ns_context().move_to_parent_namespace();

	// Create macro
	if( parser.create_macro( m_macro, ident, nullptr, nullptr, nullptr, macro_flags ).failed_status( status ) )
		return status;
	m_macro->set_value_variant( value );
	m_macro->modify_ex_flags( macro_ex_flags );

	if( previous_ns != parser.get_ns_context().get_current_namespace() )
		parser.get_ns_context().set_current_namespace( previous_ns );

	// Exit
	return STATUS_SUCCESS;
}
bool f1::f1pp::assignment_directive::target_created() const
{
	return nullptr != m_macro;
}
bool f1::f1pp::assignment_directive::can_modify_target() const
{
	__debugbreak_if( nullptr == m_macro )
		return false;
	if( 0 != m_macro->get_ex_flags( f1pp::macro::ex_flags_t::fl_const ) )
		return false;
	if( 0 != m_macro->get_flags( pp::Imacro::macro_flags_t::fl_immutable ) )
		return false;
	return true;
}
const status_t f1::f1pp::assignment_directive::get_target_value( pp::variant & value, pp::variant::type_t ty )
{
	value = m_macro->get_value_variant();
	if( pp::variant::ty_none != ty && value.get_type() != ty )
		value.set_type( ty );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::assignment_directive::set_target_value( const pp::variant & value )
{
	status_t	status;
	pp::string	str_value;
	switch( value.get_type() )
	{
	case pp::variant::ty_none:
		status = STATUS_SUCCESS;
		break;	// leave str empty
	case pp::variant::ty_bool:{
		core::string	str;
		status = str.format( "%d", value.as_int() );
		str_value.add_part( str );
		}break;
	default:
		status = value.format( str_value );
		break;
	}
	if( status.failed() )
		return status;

	// Set the value
	m_macro->set_value_variant( value, str_value );

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::f1pp::assignment_directive::push_all_items( f1pp::parser & parser )
{
	return parser.message( syntax::IMSG_ERROR_UNSUPPORTED_S, "push all" );
}
const status_t f1::f1pp::assignment_directive::push_items( f1pp::parser & parser, const core::Tarray<pp::string> & ident_list )
{
	return parser.push_macros( ident_list );
}
const status_t f1::f1pp::assignment_directive::pop_items( f1pp::parser & parser )
{
	return parser.pop_macros();
}
const status_t f1::f1pp::assignment_directive::delete_items( f1pp::parser & parser, const core::Tarray<pp::string> & ident_list )
{
	__unreferenced_parameter( ident_list );
	return parser.message( syntax::IMSG_ERROR_UNSUPPORTED_S, "delete" );
}

/*END OF assignment_directive.cxx*/
